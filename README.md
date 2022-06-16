# Smart Locker - Arduino Project
IOT Device that uses fingerprint module and solenoids to set the security for the lockers. 

## Running Arduino Controls
Connection from Fingerprint Module 1 to Arduino Uno

| Keypad  | Function |
| ------------- | ------------- |
| A  | Start Fingerprint Registration  |
| B  | Start Fingerprint Client Scan  |
| #  | Enter (For Fingerprint Password)  |
| *  | Clear Input Memory  |
| D  | Reset  |

## Connections
Follow this connections for single device 

![Artboard 1](https://user-images.githubusercontent.com/28371423/174103317-fda46036-f3eb-4fb4-9538-f7e52da353d5.png)



Connection of Button Reset to Arduino Uno  (You can ignore this part)

| Slot  | Button |
| ------------- | ------------- |
| Reset  | Reset Button  |
| 13  | Registration Button  |
| 12  | Scan Button  | 

You can follow this schematic for button, For reset button, Just connect 1 foot at Reset then GND for the other  (Optional)

![download](https://user-images.githubusercontent.com/28371423/172420948-8963761a-04c7-443e-a635-87ecfc8eec77.png)
