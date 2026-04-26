#include <SPI.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
//#include <MFRC522DriverI2C.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
#include <ESP32Servo.h>

int servoPin = 17;
#define CUSTOM_SS 5     // Keep this

Servo myservo;  // create servo object to control a servo
MFRC522DriverPinSimple ss_pin(CUSTOM_SS);
MFRC522DriverSPI driver{ss_pin};
MFRC522 mfrc522{driver};

String allowedUID = "d1896906";   // lowercase to match HEX output
void setup() {
 Serial.begin(115200);
 delay(1000);
 Serial.println("ESP32-S3 is connected successfully!");
 SPI.begin(36, 37, 35);  + // SCK, MISO, MOSI, SS ,SPI =laguage to communicate between esp32 and RFRID
 mfrc522.PCD_Init();

 myservo.setPeriodHertz(50); 
 myservo.attach(servoPin);
 myservo.write(90); 
 delay(1000);

 Serial.println("ESP32 RFID + Servo Ready");
 Serial.println("Scan your card...");
}

void loop() {

  // Wait for a new card
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  // Read UID
  // uid make from 4 byte,so we need check at least 4 time (each time 1 byte ) to get a complete uid
  // ensure each byte <16 add a leading "0" first before convert them to hexadecimal
  String uidString = "";
 
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }
  uidString.toLowerCase();
  Serial.print("Card UID: ");
  Serial.println(uidString);

  if (uidString == allowedUID){
    Serial.print("E-waste");
    myservo.write(180);
    delay(1000);
    myservo.write(90); 
  }else{
    Serial.print("Normal waste");
    myservo.write(0);
    delay(1000);
    myservo.write(90); 
  }
}
