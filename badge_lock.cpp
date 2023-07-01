/*This is the code for three elements used with Arduino
 *to create a lock.
 *badge UID: Indentification from badge 
 *carte UID: Indentification from badge
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>


#define RST_PIN         5         
#define SS_PIN          53       
#define SERVO_PIN       9          
const int pinR = 11;  // Pin for the red component (R)
const int pinG = 12; // Pin for the green component (G)

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
Servo myServo;                     // Create Servo instance

byte desiredUID1[] = {"put hexadecimal identifiant for badge 1"};  // Badge 1
byte desiredUID2[] = {"0x00, 0x00, 0x00, 0x00"};  // Badge 2

void setup() {
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  myServo.attach(SERVO_PIN);     // Attach the servo to the appropriate pin
  myServo.write(0);              // Initialize position to servo (0)
  Serial.begin(9600);    // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(5);
}

void loop() {
  while (true) {
    // Reset the loop if no new badge is present on the sensor/reader. This saves processing when the system is idle.
    if (mfrc522.PICC_IsNewCardPresent()) {
      // Read a badge
      if (mfrc522.PICC_ReadCardSerial()) {
        // Verifying the badge
        if (memcmp(mfrc522.uid.uidByte, desiredUID1, sizeof(desiredUID1)) == 0) {
          // If the identifier corresponds to badge 1
          Serial.println("Badge 2 accepté");
          myServo.write(180);       // Rotate the servo 180 degrees
          analogWrite(pinR, 0);   
          analogWrite(pinG, 255);   // Maximum value for green light
          delay(5000);              // Wait 5 seconds
          myServo.write(0);         // Return to init servo position
        } else if (memcmp(mfrc522.uid.uidByte, desiredUID2, sizeof(desiredUID2)) == 0) {
          // If the identifier corresponds to badge 2
          Serial.println("Badge 2 accepté");
          myServo.write(180);       // Rotate the servo 180 degrees
          analogWrite(pinR, 0);   
          analogWrite(pinG, 255);   // Maximum value for green light
          delay(5000);              // Wait 5 seconds
          myServo.write(0);         // Return to init servo position
        } else {
          // If the identifier does not match any of the badges
          Serial.println("Le badge n'est pas bon");
          analogWrite(pinR, 225);   // Red light
          analogWrite(pinG, 0);     // No green light
          delay(5000);
        }

        // Print infos to debug on the badge ; PICC_HaltA() is automatically called
        mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
        continue;
      }
    }
    
    analogWrite(pinR, 0);   // waiting state, no light
    analogWrite(pinG, 0);  
  }
  
  // Reset the RFID to new reading
  mfrc522.PCD_Reset();
}