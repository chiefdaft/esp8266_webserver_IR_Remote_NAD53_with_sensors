# esp8266_webserver_IR_Remote_and_sensors
ESP8266 sketch (for Arduino IDE) to be used as an infrared remote transmitter combined with temperature, humidity and air pressure sensors. De webserver on the ESP8266 creates an interactive bootstrap themed webpage. This particular remote control is designed for a NAD L53 DVD receiver. The IR codes are the Ponto HEX codes as made available by NAD Electronics. See Fritzing hardware scheme for the physical layout.

Necessary hardware ingedredients:
* ESP8266 board, for example a Node MCU 1.0
* IR LED
Optional sensors: 
* DHT-22 
* Bosch BMP085

Webpage on the ESP8266:
![screenshot of webpage on mobile phone](https://www.mcfly.nl/Screenshot_20200517-123118_Chrome_240x494.jpg)
