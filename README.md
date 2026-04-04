
````markdown
# STM32 ADC + DMA + Low-pass Filter System

This project is an embedded system designed to use the STM32 microcontroller to acquire analog data through the ADC (Analog-to-Digital Converter) and DMA (Direct Memory Access). The collected data is then processed using a low-pass filter, and the results are output over a serial interface. This system allows real-time control of the filter through a simple CLI (Command Line Interface).

## Features

- **ADC + DMA Data Acquisition**: Uses ADC in continuous mode with DMA for efficient data collection.
- **Low-pass Filtering**: Real-time filtering of ADC data to smooth the input signal.
- **CLI Control**: Real-time control of the system's filter parameters and enabling/disabling the filter via the command line.
- **Real-time Data Output**: Filtered and raw data output via serial communication.
- **Interrupt-Driven**: Uses DMA and interrupt mechanisms to handle data efficiently.

## Hardware Requirements

- **STM32F10x Microcontroller**: This project is based on STM32F103C8T6 or any STM32F10x microcontroller with ADC and DMA support.
- **ADC Sensor**: Analog sensors connected to the microcontroller's analog pins (e.g., potentiometer or any analog input).
- **USB-to-Serial Adapter**: For serial communication between STM32 and a PC or terminal.

## Software Requirements

- **Keil uVision IDE**: The project is developed using Keil MDK and STM32 Standard Peripheral Libraries.
- **STM32 Standard Peripheral Library**: Necessary for setting up and using STM32 peripherals like ADC, DMA, GPIO.
- **Serial Terminal Software**: A serial terminal like PuTTY or Tera Term to interact with the system over UART.

## Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/stm32-adc-dma-filter-system.git
cd stm32-adc-dma-filter-system
````

### 2. Hardware Setup

1. **Connect the ADC**: Connect an analog sensor to GPIO pin **PA0** (or another analog pin on your STM32 board).
2. **Connect to Serial**: Connect the STM32's TX and RX pins to a USB-to-Serial adapter to enable communication with your computer.

### 3. Software Setup

1. Open the project in **Keil uVision IDE**.
2. Set the **device** and **toolchain** for STM32 in the IDE.
3. Make sure to configure the **STM32 Standard Peripheral Library** in the project settings.
4. Build the project and upload it to your STM32 device.

### 4. CLI Commands

Once the system is running, you can interact with it using the following CLI commands through your serial terminal:

* **help**: Displays a list of available commands.
* **led on**: Turns the LED on (if connected).
* **led off**: Turns the LED off (if connected).
* **filter on**: Enables the low-pass filter for the ADC data.
* **filter off**: Disables the low-pass filter.
* **set alpha x**: Sets the low-pass filter's alpha value. The value `x` should be between 0.01 and 0.99.

Example CLI commands:

```bash
> help
Available Commands:
help      - show command list
led on    - turn LED on
led off   - turn LED off
filter on - enable ADC filtering
filter off - disable ADC filtering
set alpha x - set low-pass alpha (0.01..0.99)

> filter on
Filter Enabled

> set alpha 0.5
Alpha set to: 0.50

> filter off
Filter Disabled
```

### 5. Serial Output

The system will print both the raw and filtered ADC data over the serial port at a frequency of 50ms. Example output:

```text
3498, 3492
3496, 3493
3495, 3491
```

The format is:

```
raw_data, filtered_data
```

---

## Project Structure

```text
/STM32_ADC_DMA_LowPass_Filter
│
├── ADC.c                # ADC initialization and configuration
├── Filter.c             # Low-pass filter implementation
├── SysTick.c            # SysTick initialization for timing
├── main.c               # Main application logic
├── task.c               # Task scheduling and processing logic
├── task.h               # Task interface definitions
├── Serial.c             # Serial communication functions
├── Serial.h             # Serial communication header
├── Makefile             # Keil uVision build file
└── README.md            # Project documentation
```

---

## Future Improvements

* **Advanced Filters**: Implement more sophisticated filters like Kalman filters for more dynamic data processing.
* **Wireless Communication**: Add support for wireless data transmission (e.g., via Bluetooth, Wi-Fi, or LoRa).
* **Web Interface**: Implement a web interface for controlling the system and viewing real-time data remotely.

---

## License

This project is open-source and released under the MIT License. See the [LICENSE](LICENSE) file for more details.

```

