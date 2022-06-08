# Smart Locker - Arduino Project
IOT Device that uses fingerprint module and solenoids to set the security for the lockers. 

## Connections 
Connection from Fingerprint Module 1 to Arduino Uno

| Fingerprint Module  | Arduino |
| ------------- | ------------- |
| TX  | 1  |
| RX  | 2  |
| VCC  | 3.3V  |
| GND  | GND  |

Connection of Display Module to Arduino Uno

| Display Module  | Arduino |
| ------------- | ------------- |
| SCL  | A5  |
| SDA  | A4  |
| VCC  | 5V  |
| GND  | GND  | 

Connection of Keypad to Arduino 
Just pin the pins like in the schematic design, but start to pin 4 to pin 11

![keypad to arduino](https://user-images.githubusercontent.com/28371423/172602264-808fa301-6cd1-4e2a-b9f3-b48b056d7a05.png)


Connection of Button Reset to Arduino Uno  (You can ignore this part)

| Slot  | Button |
| ------------- | ------------- |
| Reset  | Reset Button  |
| 13  | Registration Button  |
| 12  | Scan Button  | 

You can follow this schematic for button, For reset button, Just connect 1 foot at Reset then GND for the other

![download](https://user-images.githubusercontent.com/28371423/172420948-8963761a-04c7-443e-a635-87ecfc8eec77.png)
