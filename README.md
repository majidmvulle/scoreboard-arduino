# scoreboard-arduino

A scoreboard implementation for a football match between a **Black** team and **White** team. It supports adding, subtracting and resetting scores, pausing, resuming and resetting the timer.


## 🧰 Hardware Requirements

* ESP-32S - [Amazon](https://amzn.eu/d/89fk7Xl)
* RGB LED Matrix Panel - [Amazon](https://amzn.eu/d/gCOoe4x)
* YK04 Wireless 4 Channel 315Mhz Remote Control - [Microless UAE](https://uae.microless.com/product/generic-wireless-4-channel-315mhz-remote-control-kits-receiver-module-for-arduino-4-channel-315mhz/)
* A 5v power source for the display panel and a 3.3v power source for the ESP-32S.

## 🧑‍💻 Software Requirements
* Arduino IDE - https://www.arduino.cc/en/software/
* Adafruit Bus IO Library - https://github.com/adafruit/Adafruit_BusIO
* Adafruit GFX Library - https://github.com/adafruit/Adafruit-GFX-Library
* HUB75 RGB LED matrix library utilizing ESP32 DMA Engine - https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA
* YK04 Controller Library - https://github.com/YuriiSalimov/YK04_Module
* Unit Test Library - https://github.com/bxparks/AUnit


## 📄 Documentation & Circuit Diagrams
- Waveshare RGB LED Matrix power connection - [Link](https://learn.adafruit.com/32x16-32x32-rgb-led-matrix/powering)

<img alt="Waveshare RGB LED Matrix and Arduino Mega" src="https://cdn-learn.adafruit.com/assets/assets/000/002/941/original/led_matrix_power-cable.jpg"/>

- Waveshare RGB LED Matrix and ESP-32S - [Link](https://www.waveshare.com/wiki/RGB-Matrix-P3-64x64#Hardware_Connection_2)

<img alt="Waveshare RGB LED Matrix and ESP-32S" src="https://www.waveshare.com/wiki/RGB-Matrix-P3-64x64#/media/File:RGB-Matrix-P4-64x32_ESP32.jpg"/>


- YK04 Module - [Link](https://github.com/YuriiSalimov/YK04_Module)

<img alt="YK04 Module" src="https://github.com/YuriiSalimov/YK04_Module/blob/master/CircuitDiagram.png"/>



## 🔧 Installation
- After checking out the repo, open the Arduino IDE and open the file `scoreboard.ino`.
- Connect your hardware according to the circuit diagrams above.
- Run it!


## 🎭 Demo


[![Watch the video](demo.png)](https://www.youtube.com/shorts/wUx_zdTx2Pw)


## 🔗 Useful links

- https://www.arduino.cc


## 💻 Developers

- Majid Mvulle <majidmvulle@me.com>


## 🧪 Testing <a name="testing"></a>

```shell
$ make
$ ./scoreboard_test.out
```
