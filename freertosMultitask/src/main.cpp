#include <Arduino.h>

#define BTN_PIN      0          // Boot button (active low)
#define LED_PIN     38          // onboard WS2812 (GPIO48)
#define BRIGHTNESS   32         // scale 0..255

QueueHandle_t queue;

void handle_error() {
  // Solid red if error
  neopixelWrite(LED_PIN, BRIGHTNESS, 0, 0);
  for (;;);
}

// Blink red
void task_blink01(void *args) {
  for (;;) {
    neopixelWrite(LED_PIN, BRIGHTNESS, 0, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    neopixelWrite(LED_PIN, 0, 0, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Blink green
void task_blink02(void *args) {
  for (;;) {
    neopixelWrite(LED_PIN, 0, BRIGHTNESS, 0);
    vTaskDelay(700 / portTICK_PERIOD_MS);
    neopixelWrite(LED_PIN, 0, 0, 0);
    vTaskDelay(700 / portTICK_PERIOD_MS);
  }
}

// Blink blue
void task_blikk03(void *args) {
  for (;;) {
    neopixelWrite(LED_PIN, 0, 0, BRIGHTNESS);
    vTaskDelay(900 / portTICK_PERIOD_MS);
    neopixelWrite(LED_PIN, 0, 0, 0);
    vTaskDelay(900 / portTICK_PERIOD_MS);
  }
}

// Blink blue on queue event
void task_output(void *args) {
  for (;;) {
    bool trigger = false;
    if (xQueueReceive(queue, &trigger, portMAX_DELAY) == pdPASS) {
      if (trigger) {
        neopixelWrite(LED_PIN, 0, 0, BRIGHTNESS);
        vTaskDelay(300 / portTICK_PERIOD_MS);
        neopixelWrite(LED_PIN, 0, 0, 0);
      }
    } else {
      handle_error();
    }
  }
}

// Button handling
void task_button(void *args) {
  pinMode(BTN_PIN, INPUT_PULLUP);
  uint8_t last = digitalRead(BTN_PIN);

  for (;;) {
    uint8_t now = digitalRead(BTN_PIN);
    if (now == LOW && last == HIGH) {
      bool data = true;
      if (xQueueSend(queue, &data, portMAX_DELAY) != pdPASS)
        handle_error();
    }
    last = now;
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  queue = xQueueCreate(5, sizeof(bool));
  if (queue == NULL) handle_error();

  xTaskCreate(task_blink01, "Task Blink 01", 2048, NULL, 1, NULL);
  xTaskCreate(task_blink02, "Task Blink 02", 2048, NULL, 1, NULL);
  xTaskCreate(task_blikk03, "Task Blink 03", 2048, NULL, 1, NULL);
  xTaskCreate(task_button, "Task Button", 2048, NULL, 1, NULL);
  xTaskCreate(task_output, "Task Output", 2048, NULL, 1, NULL);
  /*
    if (xTaskCreate(task_blink01, "Task Blink 01", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
        handle_error();
    if (xTaskCreate(task_blink02, "Task Blink 02", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
        handle_error();
    if (xTaskCreate(task_blikk03, "Task Blink 03", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
    if (xTaskCreate(task_button, "Task Button", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
        handle_error();
    if (xTaskCreate(task_output, "Task Output", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
        handle_error();
        */
}

void loop() {
  // FreeRTOS scheduler runs tasks
}
 