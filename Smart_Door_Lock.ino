#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h> 
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
#define relay 3
#define SERVO_PIN 2
Servo myservo;
String input;
String code;
String password = "1235A";
char keypressed;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
byte accessUID[4] = {0x05, 0xD6, 0x76, 0x76};

char keymap[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {16, 15, 14, 17}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad


Keypad myKeypad = Keypad( makeKeymap(keymap), rowPins, colPins, ROWS, COLS); 

void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  digitalWrite (relay, HIGH);
  myservo.attach(SERVO_PIN);
  myservo.write( 70 );
  delay(1000);
  myservo.write( 0 );
  Serial.println("Put your card to the reader...");
  Serial.println();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Smart Door Lock");
  delay(1000);
  lcd.clear();
}

void loop() {
  readKeypad();
  
  
  if ((code == password) || (mfrc522.uid.uidByte[0] == accessUID[0] && 
  mfrc522.uid.uidByte[1] == accessUID[1] && 
  mfrc522.uid.uidByte[2] == accessUID[2] && mfrc522.uid.uidByte[3] == accessUID[3])){
    Serial.println ("Access Granted");
    lcd.setCursor(0,1);
    lcd.print ("Access Granted");
    digitalWrite (relay, LOW);
    myservo.write(70);
    delay(3000);
    code = "";
    mfrc522.uid.uidByte[0] = 0;
    mfrc522.uid.uidByte[1] = 0;
    mfrc522.uid.uidByte[2] = 0;
    mfrc522.uid.uidByte[3] = 0;
    lcd.setCursor(0,1);
    lcd.clear();
    digitalWrite (relay, HIGH);
    myservo.write(0);
    
  }if ((code!=""&&code!=password)||(mfrc522.uid.uidByte[0]!=0&&
  mfrc522.uid.uidByte[0]!= accessUID[0] && mfrc522.uid.uidByte[1]!=0&&
  mfrc522.uid.uidByte[1]!=accessUID[1] && mfrc522.uid.uidByte[2]!=0&&
  mfrc522.uid.uidByte[2]!=accessUID[2] && mfrc522.uid.uidByte[3]!=0&&
  mfrc522.uid.uidByte[3]!=accessUID[3])){
    lcd.setCursor(0,1);
    lcd.print ("Access Denied");
    delay(2500);
    code = "";
    mfrc522.uid.uidByte[0] = 0;
    mfrc522.uid.uidByte[1] = 0;
    mfrc522.uid.uidByte[2] = 0;
    mfrc522.uid.uidByte[3] = 0;
    lcd.setCursor(0,1);
    lcd.clear();
  }
  
  // Reset the loop if no new card present on the sensor/reader. 
  // This saves the entire process when idle.
  // Select one of the cards
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() || ! keypressed) {
    return;
  }
  
  mfrc522.PICC_HaltA();
}
void readKeypad(){
  keypressed = myKeypad.getKey();
  lcd.setCursor(0,0);
  lcd.print(input);
  if (keypressed != '#'){
    String value = String(keypressed);
    input += value;
  }
  else{
    code = input;
    input = "";
    lcd.clear();
  }
  if (keypressed == '*'){
    input = "";
    lcd.clear();
  } 
}
