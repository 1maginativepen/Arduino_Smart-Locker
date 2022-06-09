
# Code Documentation 

```cpp

// Start of Arduino Library
// Make sure this library is installed to your Arduino IDE 
#include <Adafruit_Fingerprint.h>
#include <Keypad.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>  
// End of Arduino Library

// Start for Fingerprint Module Preparations
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__) 
SoftwareSerial mySerial(2, 3); 
#else 
#define mySerial Serial1 
#endif 
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
// End of Fingerprint Module Preparations

// Preparation for Keyboard Module
uint8_t process_id = 0; 
const int ROW_NUM = 4;  
const int COLUMN_NUM = 4;   
char keys[ROW_NUM][COLUMN_NUM] = {
      {'1','2','3', 'A'},
      {'4','5','6', 'B'},
      {'7','8','9', 'C'},
      {'*','0','#', 'D'}
};  
byte pin_rows[ROW_NUM] = {4, 5, 6, 7}; 
byte pin_column[COLUMN_NUM] = {8, 9, 10, 11};  
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM ); 
const String password = "1234";  
String input_password; 
// End of Keyboard Module

// Start of Display Module
LiquidCrystal_I2C lcd(0x27, 16, 2);  
// End of Display Module

void setup()
{
  Serial.begin(9600); 
  lcd.init(); 
  lcd.clear();
  lcd.init();
  lcd.backlight();
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Smart Locker"); 
  lcd.setCursor(0,1);
  lcd.print("[A]Reg [B]Scan");  
} 

// Reset Arduino Board Function
void(* resetFunc) (void) = 0;   
// End of Reset Function

void loop()                    
{    
  char key = keypad.getKey();  
  if (key){ 
    if(key =='A'){ 
      while (!Serial);   
      delay(100); 
      finger.begin(57600); 
      finger.getParameters();   
      input_password = ""; 
      lcd.setCursor(0,0);
      lcd.print("Register mode      "); 
      lcd.setCursor(0,1);
      lcd.print("Type Pass          "); 
    } 
    else if(key == 'B'){    
      while (!Serial);   
      delay(100);   
      finger.begin(57600); 
      finger.getParameters();  
      finger.getTemplateCount();  
      while (!  ScanCommand(200) ); 
      input_password += key; 
      delay(50);       
    }else if(key == '*') {
      input_password = "";  
    }  
      else if(key == '#') {
        lcd.setCursor(0,0); 
        lcd.print("Status:          ");  
        lcd.setCursor(0,1);  
        if(password == input_password) {  
        lcd.print("ACCESS GRANTED   ");  
        while (!  ScanCommand(1) ); 
        input_password += key; 
        delay(2000);

      } else {  
        lcd.print("ACCESS DENIED   ");   
        input_password = ""; 
        delay(2000);
        resetFunc();
      } 
    } 
    else {
      input_password += key; 
    }
  }
}

uint8_t ScanCommand(int id) {
  if(id == 200){    
    getFingerprintID(); 
    delay(50); 
  }else{
    getFingerprintEnroll(id);
    delay(50); 
  } 
  return false;
} 

// START OF FINGERPRINT REGISTRATION FUNCTION
uint8_t getFingerprintEnroll(int id) { 
  lcd.setCursor(0,0);
  lcd.print("Status:        ");   
  lcd.setCursor(0,1); 
  int p = -1; 
  while (p != FINGERPRINT_OK) {
    p = finger.getImage(); 
    char key = keypad.getKey();  
    if (key){ 
        if(key =='D'){ 
          resetFunc();
        } 
    } 
    switch (p) {
    case FINGERPRINT_OK: 
      lcd.print("Image Taken         ");  
      break;
    case FINGERPRINT_NOFINGER: 
      lcd.print("Waiting input....   ");  
      break; 
    default: 
      lcd.print("Unknown Error       ");  
      break;
    }
  } 

  lcd.setCursor(0,1); 
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.print("Image converted      ");  
      break;  
    default:  
      lcd.print("Unknown Error        ");  
      return p;
  }

  lcd.setCursor(0,1); 
  lcd.print("Remove Finger         ");  
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  } 
  p = -1;
  lcd.setCursor(0,0); 
  lcd.print("Status:        ");  
  lcd.setCursor(0,1); 
  lcd.print("Place same finger       ");   
  while (p != FINGERPRINT_OK) { 
  char key = keypad.getKey();  
  if (key){ 
      if(key =='D'){ 
        resetFunc();
      } 
  }
    p = finger.getImage();
    lcd.setCursor(0,1); 
    switch (p) {
    case FINGERPRINT_OK:
      lcd.print("Image taken        ");  
      break;
    case FINGERPRINT_NOFINGER: 
      lcd.print("Waiting input...   ");  
      break; 
    default: 
      lcd.print("Unknown Error             ");  
      break;
    }
  }  
  p = finger.image2Tz(2);
  lcd.setCursor(0,1); 
  switch (p) {
    case FINGERPRINT_OK:
      lcd.print("Image converted     ");   
      break;
    case FINGERPRINT_IMAGEMESS: 
      lcd.print("Messy image       ");  
      return p; 
    default:  
      lcd.print("Unknown error    ");    
      return p;
  }   
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    lcd.setCursor(0,0); 
    lcd.print("SUCCESS        ");  
    lcd.setCursor(0,1); 
    lcd.print("Finger Enrolled             ");   
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.setCursor(0,1); 
    lcd.print("Communication Er            ");  
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    return p;
  } else {
    lcd.setCursor(0,1); 
    lcd.print("Not matched        ");   
    return p;
  } 
  lcd.setCursor(0,0); 
  lcd.print("Status:         "); 
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  lcd.setCursor(0,1); 
  if (p == FINGERPRINT_OK) { 
    lcd.print("Refreshing Device   ");  
    delay(5000); 
  } 
  resetFunc();     
}  
// END OF FINGERPRINT REGISTRATION FUNCTION

// START OF FINGERPRINT SCAN FUNCTION
uint8_t getFingerprintID() {   
  lcd.setCursor(0,1);
  lcd.print("               ");   
  uint8_t p = finger.getImage(); 
  lcd.setCursor(0,0);
  lcd.print("Scan mode        ");
  char key = keypad.getKey();  
  if (key){ 
      if(key =='D'){ 
        resetFunc();
      } 
  }
  lcd.setCursor(0,1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.print("Image Taken...    "); 
      break; 
    case FINGERPRINT_NOFINGER:
      lcd.setCursor(0,1);
      if(process_id == 0){ 
        lcd.print("Waiting input...      "); 
      }
      process_id = 1;
      return p; 
    case FINGERPRINT_IMAGEFAIL:
      lcd.print("Imaging Er     ");  
      return p; 
  } 
  delay(1000);      
  p = finger.image2Tz();
      lcd.setCursor(0,0);
      lcd.print("Status:       ");  
      lcd.setCursor(0,1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.print("Image converted     ");   
      break;
    case FINGERPRINT_IMAGEMESS: 
      lcd.print("Messy Image");   
      return p; 
    default:
      Serial.println("Unknown error");
      return p;
  } 
  delay(2000);       
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {   
    lcd.setCursor(0,1);
    lcd.print("Found Match       "); 
    delay(3000);           
    resetFunc(); 
    return p;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.setCursor(0,1);
    lcd.print("Communmication Er    ");    
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) { 
    delay(3000);            
    lcd.setCursor(0,1);
    lcd.print("Not found        "); 
    delay(3000);           
    return p;
  } else {
    lcd.setCursor(0,1);
    lcd.print("Error Unknown     ");    
    return p;
  }     
  return finger.fingerID;
}  
// END OF FINGERPRINT SCAN FUNCTION
```
