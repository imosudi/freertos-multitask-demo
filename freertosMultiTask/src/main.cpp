
#include <Arduino.h>

#define BTN_PIN      0          // Boot button (active low)
#define LED_PIN     38          // onboard WS2812 (GPIO38)
#define BRIGHTNESS   32         // scale 0..255

QueueHandle_t queue_button;   // from Task 3 (button count)
QueueHandle_t queue_color;    // to Task 2 (colour control)

// Error handler
void handle_error() {
  neopixelWrite(LED_PIN, BRIGHTNESS, 0, 0); // Solid red
  for (;;);
}

// Dispatcher task: Serial I/O + Button feedback
void task_01(void *args) {
  uint32_t count = 0;

  for (;;) {
    // --- Handle serial input ---
    if (Serial.available() > 0) {
      char c = Serial.read();
      Serial.write(c); // echo back

      if (c == 'r' || c == 'g' || c == 'b') {
        if (xQueueSend(queue_color, &c, portMAX_DELAY) != pdPASS) {
          handle_error();
        }
      }
    }

    // --- Handle button press count ---
    if (xQueueReceive(queue_button, &count, 0) == pdPASS) {
      Serial.printf("Hello, the button was clicked %lu times.\n", count);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS); // Small yield
  }
}

// Task 2: LED controlled by colour queue
void task_02(void *args) {
  char color = 'r'; // default = red at startup

  // Turn LED red initially
  neopixelWrite(LED_PIN, BRIGHTNESS, 0, 0);

  for (;;) {
    // Wait until new colour command arrives
    if (xQueueReceive(queue_color, &color, portMAX_DELAY) == pdPASS) {
      switch (color) {
        case 'r':
          neopixelWrite(LED_PIN, BRIGHTNESS, 0, 0); // red
          break;
        case 'g':
          neopixelWrite(LED_PIN, 0, BRIGHTNESS, 0); // green
          break;
        case 'b':
          neopixelWrite(LED_PIN, 0, 0, BRIGHTNESS); // blue
          break;
        default:
          neopixelWrite(LED_PIN, 0, 0, 0); // off for invalid data
          break;
      }
    }
  }
}

// Task 3: Button handling with counter
void task_03(void *args) {
  pinMode(BTN_PIN, INPUT_PULLUP);
  uint8_t last = digitalRead(BTN_PIN);
  uint32_t press_count = 0;

  for (;;) {
    uint8_t now = digitalRead(BTN_PIN);
    if (now == LOW && last == HIGH) {
      press_count++;
      if (xQueueSend(queue_button, &press_count, portMAX_DELAY) != pdPASS) {
        handle_error();
      }
    }
    last = now;
    vTaskDelay(50 / portTICK_PERIOD_MS); // debounce
  }
}

void setup() {
  Serial.begin(115200);

  queue_button = xQueueCreate(5, sizeof(uint32_t));
  queue_color  = xQueueCreate(5, sizeof(char));
  if (queue_button == NULL || queue_color == NULL) handle_error();

  xTaskCreate(task_01, "Task: Serial feedback", 4096, NULL, 1, NULL);
  xTaskCreate(task_02, "Task: INBUILT LED Control", 2048, NULL, 1, NULL);
  xTaskCreate(task_03, "Task: Counter Button", 2048, NULL, 1, NULL);
}

void loop() {
  // FreeRTOS scheduler runs tasks
}
 