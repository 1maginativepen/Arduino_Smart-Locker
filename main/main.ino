#include <LiquidCrystal.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int registerButton = 13;     
int registerButtonState = 0;    
  
const int scanButton = 12;     
int scanButtonState = 0;      

void setup()
{
Serial.begin(9600);
  // initialize the LCD
  lcd.init(); 
  lcd.clear();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  Display_Startup(); 
  delay(2000);
  Selection(); 
  
  pinMode(registerButton, INPUT);
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
  lcd.print("[FINGER IN]                    "); 
}

void loop()
{
  registerButtonState = digitalRead(registerButton);
  scanButtonState = digitalRead(scanButton);
  
  if (registerButtonState == HIGH) { 
    RegisterProcess();  
  }   
  if (scanButtonState == HIGH) { 
    ScanProcess();  
  }  
}
