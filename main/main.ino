#include <Adafruit_Fingerprint.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__) 
SoftwareSerial mySerial(2, 3);

#else 
#define mySerial Serial1

#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id = 1;
uint8_t process_id = 0;

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

void setup()
{
  Serial.begin(9600);
  lcd.init(); 
  lcd.clear();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  Display_Startup(); 
  delay(2000);
  Selection(); 
}

void loop()               
{ 
  char key = keypad.getKey();  
  if (key){
      Serial.println(key);
      if(key =='D'){  
        while (!  ScanCommand() );
        delay(50); 
      }
      
      if(key == 'A'){    
        while (!Serial); 
        delay(100);   
        finger.begin(57600);
        delay(5);  
        finger.getParameters();  
        finger.getTemplateCount(); 
        id=200;
        while (!  ScanCommand() );
        delay(50); 
      }
      else {
          input_password += key; 
          lcd.setCursor(0,1);
          lcd.print(input_password); 
      }
  }
}

uint8_t ScanCommand() {
  if(id == 200){    
    lcd.setCursor(0,1);
    lcd.print("                         ");   
    getFingerprintID();
    delay(50); 
  }else{
    // REGISTRATION PROCESS HERE
  } 
  return false;
}

//***********************************
//This is for reading the Fingerprint
//***********************************

uint8_t getFingerprintID() { 
  uint8_t p = finger.getImage();  
  lcd.setCursor(0,0);
  lcd.print("Scan mode                   "); 
  lcd.setCursor(0,1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.print("Image Taken...                    "); 
      break; 
    case FINGERPRINT_NOFINGER:
      lcd.setCursor(0,1);
      if(process_id == 0){ 
        lcd.print("Waiting input...                   "); 
      }
      process_id = 1;
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.print("Communication Er                    "); 
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      lcd.print("Imaging Er                    ");  
      return p; 
  }
  
   delay(1000);           
  // SUCCESS

  p = finger.image2Tz();
      lcd.setCursor(0,0);
      lcd.print("Status:                    ");  
      lcd.setCursor(0,1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.print("Image converted                    ");   
      break;
    case FINGERPRINT_IMAGEMESS: 
      lcd.print("Messy Image");   
      return p;
    case FINGERPRINT_PACKETRECIEVEERR: 
      lcd.print("Communmication Er           ");    
      return p;
    case FINGERPRINT_FEATUREFAIL: 
      lcd.print("Not Found1                    ");    
      return p;
    case FINGERPRINT_INVALIDIMAGE: 
      lcd.print("Not Found2                   ");     
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  delay(2000);           
  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) { 
    lcd.setCursor(0,1);
    lcd.print("Found Match                    "); 
    // SOLENOID FUNCTIONS HERE  
    delay(4000);
    id = 1;
    setup();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.setCursor(0,1);
    lcd.print("Communmication Er                   ");    
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    lcd.setCursor(0,1);
    lcd.print("Not Found                   ");   
    return p;
  } else {
    lcd.setCursor(0,1);
    lcd.print("Error Unknown                   ");   
    delay(4000);
    id = 1;
    setup();
    return p;
  } 

   delay(3000);       
  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1; 
   
  return finger.fingerID;
}


// **************************
// DESIGNERS
// **************************

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

void RegisterProcess(){
  lcd.setCursor(0,0);
  lcd.print("Register mode                   ");
  lcd.setCursor(0,1);
  lcd.print("Enter Password                    ");  
  delay(2000); 
  input_password = "";  
  lcd.setCursor(0,0);
  lcd.print("Password                   "); 
  lcd.setCursor(0,1);
  lcd.print("                            "); 
}