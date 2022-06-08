#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Adafruit_Fingerprint.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Keyboard Rows 
const int ROW_NUM = 4;  
const int COLUMN_NUM = 4; 

char keys[ROW_NUM][COLUMN_NUM] = {
      {'1','2','3', 'A'},
      {'4','5','6', 'B'},
      {'7','8','9', 'C'},
      {'*','0','#', 'D'}
};

//connect to the row pinouts of the keypad
byte pin_rows[ROW_NUM] = {4, 5, 6, 7};

//connect to the column pinouts of the keypad
byte pin_column[COLUMN_NUM] = {8, 9, 10, 11}; 

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

//Default Password
const String password = "1234";  
String input_password;

//Display Module Setup for 0x27 Pattern
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Fingerprint Module Setup 
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
  SoftwareSerial mySerial(3,2);
#else
#define mySerial Serial1
#endif
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial); 
uint8_t id = 1;

void setup(){
      Serial.begin(9600);
      // maximum input characters is 33, change if needed
      input_password.reserve(32); 

      // Setup the Display 
      lcd.init(); 
      lcd.clear();
      lcd.init();
      lcd.backlight();
      lcd.clear();
      Display_Startup(); 
      delay(2000);
      Selection(); 
}

void loop(){
  char key = keypad.getKey(); 

  if (key){
      Serial.println(key);
      
      if(key == '*') {
          input_password = ""; 
      } else if(key == '#') {
          if(password == input_password) { 
              lcd.setCursor(0,0);
              lcd.print("*ACCESS GRANTED*             "); 
              lcd.setCursor(0,1);
              lcd.print("Thank You                    ");  
              // while (!  ScanCommand() );
          } else {
              lcd.setCursor(0,0);
              lcd.print("*ACCESS DENIED*             "); 
              lcd.setCursor(0,1);
              lcd.print("Try Again                    ");  
              // while (!  ScanCommand() );
          } 
          input_password = ""; // clear input password
      }  
      
      // Press D if you want to register new Fingerprint (Admin Rights) 
      // Press A if you want to scan Fingerprint (Client Side) 
      else if(key =='D'){ 
          RegisterProcess();    
      } else if(key == 'A'){ 
          ScanProcess();  
          id=200; 
      }
      else {
          input_password += key; // append new character to input password string 
          lcd.setCursor(0,1);
          lcd.print(input_password); 
      }
  }
}

uint8_t ScanCommand() {
  if(id == 200){ 
//    getFingerprintID();
    delay(50); 
  }else{
//    registerFingerprint();
  } 
  return false;
} 

void Display_Startup(){ 
  lcd.setCursor(0,0);
  lcd.print("Smart Locker");
  lcd.setCursor(0,1);
  lcd.print("Thesis Project");
}

void Selection(){
  lcd.setCursor(0,0);
  lcd.print("PLEASE SELECT");
  lcd.setCursor(0,1);
  lcd.print("Scan or Register"); 
}

void ScanProcess(){ 
  lcd.setCursor(0,0);
  lcd.print("Scan mode                   ");
  lcd.setCursor(0,1);
  lcd.print("[FINGER IN]                    "); 
}

void RegisterProcess(){
  lcd.setCursor(0,0);
  lcd.print("Register mode                   ");
  lcd.setCursor(0,1);
  lcd.print("Enter Password                    ");  
  delay(2000); 
  input_password = ""; // clear key memory
  lcd.setCursor(0,0);
  lcd.print("Password                   "); 
  lcd.setCursor(0,1);
  lcd.print("                            "); 
}
