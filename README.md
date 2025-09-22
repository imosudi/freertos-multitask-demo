# freertos-multitask-demo
FreeRTOS demo with 3 tasks: serial I/O with echo (115200 8N1), RGB LED control via color commands ('r','g','b'), and button press counting. Shows inter-task communication using queues - Task 1 handles serial/display, Task 2 controls LED colors, Task 3 monitors button presses.
