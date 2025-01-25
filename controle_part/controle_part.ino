#include "SPI.h"
#include "MFRC522.h"
#include <Stepper.h> 
#include <EEPROM.h>

#define CMK_HARDWARE
const int steps = 2048;
Stepper myStepper = Stepper(steps, 3, 5, 4, 6);

#define RST_PIN 9    // RES pin
#define SS_PIN 10    // SDA (SS) pin
#define LED_PIN 13   // Pin where LED is connected

MFRC522 mfrc522(SS_PIN, RST_PIN);
const int UID_EEPROM_ADDRESS = 0;
const int UID_LENGTH = 4;

char mymsg[8] = "success";  // Message to send on success
char rej[8] = "rejected";   // Message to send on rejection

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  storeCardUID();

  // Ensure a valid card is stored in EEPROM
  if (EEPROM.read(UID_EEPROM_ADDRESS) == 255) {
    Serial.println("No valid card found in EEPROM. Please scan the card to store its UID.");
  } else {
    Serial.println("Card UID stored in EEPROM. Ready to scan.");
  }
}

void loop() {
  // Reset the cycle if no card is detected
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Check if the card is valid
  if (isCardValid()) {
    Serial.write(mymsg, 8);  // Send "success" message to Serial
    myStepper.setSpeed(10); 
    myStepper.step(steps);   // Activate stepper motor
    delay(4000);  // Wait for 4 seconds
  } else {
    Serial.write(rej, 8);  // Send "rejected" message to Serial
  }
  
  mfrc522.PICC_HaltA();  // Halt the RFID card
}

// Function to store the UID of the valid card in EEPROM
void storeCardUID() {
  Serial.println("Place your valid RFID card to store its UID.");
  
  while (!mfrc522.PICC_IsNewCardPresent()) {
    delay(500);
  }

  if (mfrc522.PICC_ReadCardSerial()) {
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      EEPROM.write(UID_EEPROM_ADDRESS + i, mfrc522.uid.uidByte[i]);
    }
    Serial.println("Card UID stored in EEPROM.");
  }
}

// Function to check if the scanned card matches the stored UID
bool isCardValid() {
  byte storedUID[UID_LENGTH];
  
  // Read the stored UID from EEPROM
  for (int i = 0; i < UID_LENGTH; i++) {
    storedUID[i] = EEPROM.read(UID_EEPROM_ADDRESS + i);
  }

  // Compare the stored UID with the scanned card's UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] != storedUID[i]) {
      return false;  // Card UID does not match
    }
  }

  return true;  // Card UID matches
}