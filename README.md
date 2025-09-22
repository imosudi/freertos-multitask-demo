# FreeRTOS Multitask Demo

FreeRTOS demo with 3 tasks: serial I/O with echo (115200 8N1), RGB LED control via color commands ('r','g','b'), and button press counting. Shows inter-task communication using queues - Task 1 handles serial/display, Task 2 controls LED colors, Task 3 monitors button presses.

## Features

- **Multi-task Architecture**: Three concurrent tasks demonstrating FreeRTOS capabilities
- **Serial Communication**: UART echo functionality with 115200 8N1 settings
- **RGB LED Control**: Color switching via serial commands
- **Button Monitoring**: Boot button press counting with real-time feedback
- **Inter-task Communication**: Queue-based message passing between tasks

## System Architecture

### Task 1: Serial Handler
- Echoes all received serial data
- Forwards color commands ('r', 'g', 'b') to Task 2 via queue
- Displays button press count messages from Task 3
- Message format: "Hello, the button was clicked n times"

### Task 2: LED Controller
- Initial state: LED1 red
- Receives color commands from Task 1
- Controls LED colors:
  - 'r' → Red
  - 'g' → Green
  - 'b' → Blue

### Task 3: Button Monitor
- Monitors boot button presses
- Maintains local press counter
- Sends updated count to Task 1 via queue

## Hardware Requirements

- Microcontroller with FreeRTOS support
- RGB LED (or separate R/G/B LEDs)
- Boot button (pull-up/pull-down as needed)
- UART interface for serial communication

## Serial Configuration

- **Baud Rate**: 115200
- **Data Bits**: 8
- **Parity**: None
- **Stop Bits**: 1
- **Flow Control**: None

## Usage

1. Connect to the serial port at 115200 8N1
2. Type any characters to see them echoed back
3. Send 'r', 'g', or 'b' to change LED color
4. Press the boot button to increment counter
5. Observe button press notifications in serial output

## Building and Flashing

```bash
# Clone the repository
git clone https://github.com/imosudi/freertos-multitask-demo.git
cd freertos-multitask-demo

# Build instructions will depend on your specific toolchain and target
# Add your build commands here
```

## Dependencies

- FreeRTOS kernel
- Hardware abstraction layer for your target platform
- UART driver
- GPIO driver

## Project Structure

```
├── src/
│   ├── main.cpp            # Main application entry point
│   ├── task_serial.cpp     # Task 1 implementation
│   ├── task_led.cpp        # Task 2 implementation
│   ├── task_button.cpp     # Task 3 implementation
│   └── freertos_config.h   # FreeRTOS configuration
├── include/
│   └── tasks.h             # Task definitions and prototypes
├── README.md
└── LICENSE
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the BSD 2-Clause License - see the [LICENSE](LICENSE) file for details.

## Author

**imosudi** - [GitHub Profile](https://github.com/imosudi)

---

*This project demonstrates fundamental FreeRTOS concepts including task creation, queue-based communication, and real-time multitasking in an embedded environment.*