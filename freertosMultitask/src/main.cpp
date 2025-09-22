#include <Arduino.h>

#define BTN_PIN      0          // Boot button (active low)
#define LED_PIN     38          // onboard WS2812 (GPIO48)
#define BRIGHTNESS   32         // scale 0..255

QueueHandle_t queue;

// Error handler
void handle_error() {
  neopixelWrite(LED_PIN, BRIGHTNESS, 0, 0); // Solid red
  for (;;);
}

// Blink red according to button press count
void task_blink01(void *args) {
  uint32_t count = 0;
  for (;;) {
    // Wait for new value from the queue
    if (xQueueReceive(queue, &count, portMAX_DELAY) == pdPASS) {
      Serial.printf("Button pressed %lu times\n", count);

      // Blink red "count" times
      for (uint32_t i = 0; i < count; i++) {
        neopixelWrite(LED_PIN, BRIGHTNESS, 0, 0);
        vTaskDelay(200 / portTICK_PERIOD_MS);
        neopixelWrite(LED_PIN, 0, 0, 0);
        vTaskDelay(200 / portTICK_PERIOD_MS);
      }
    } else {
      handle_error();
    }
  }
}

// Blink green (independent task)
void task_blink02(void *args) {
  for (;;) {
    neopixelWrite(LED_PIN, 0, BRIGHTNESS, 0);
    vTaskDelay(700 / portTICK_PERIOD_MS);
    neopixelWrite(LED_PIN, 0, 0, 0);
    vTaskDelay(700 / portTICK_PERIOD_MS);
  }
}

// Blink blue (independent task)
void task_blikk03(void *args) {
  for (;;) {
    neopixelWrite(LED_PIN, 0, 0, BRIGHTNESS);
    vTaskDelay(900 / portTICK_PERIOD_MS);
    neopixelWrite(LED_PIN, 0, 0, 0);
    vTaskDelay(900 / portTICK_PERIOD_MS);
  }
}

// Button handling with counter
void task_button(void *args) {
  pinMode(BTN_PIN, INPUT_PULLUP);
  uint8_t last = digitalRead(BTN_PIN);
  uint32_t press_count = 0;

  for (;;) {
    uint8_t now = digitalRead(BTN_PIN);
    if (now == LOW && last == HIGH) {
      press_count++;
      if (xQueueSend(queue, &press_count, portMAX_DELAY) != pdPASS) {
        handle_error();
      }
    }
    last = now;
    vTaskDelay(50 / portTICK_PERIOD_MS); // debounce
  }
}

void setup() {
  Serial.begin(115200);

  queue = xQueueCreate(5, sizeof(uint32_t));  // Queue holds counts now
  if (queue == NULL) handle_error();

  xTaskCreate(task_blink01, "Task Blink 01", 4096, NULL, 1, NULL);
  xTaskCreate(task_blink02, "Task Blink 02", 2048, NULL, 1, NULL);
  xTaskCreate(task_blikk03, "Task Blink 03", 2048, NULL, 1, NULL);
  xTaskCreate(task_button, "Task Button", 2048, NULL, 1, NULL);
}

void loop() {
  // FreeRTOS scheduler runs tasks
}
