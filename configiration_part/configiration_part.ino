#include <LiquidCrystal.h>

#define CMK_HARDWARE
#ifdef CMK_HARDWARE
#define RS 8
#define En 9 
#define D4 4 
#define D5 5 
#define D6 6 
#define D7 7 
#else
#define RS 12 
#define En 11 
#define D4 5
#define D5 4 
#define D6 3 
#define D7 2 
#endif

LiquidCrystal lcd(RS, En, D4, D5, D6, D7);  // Initialize LCD

char mystr[10];  // Buffer to hold incoming serial data

void setup() { 
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
  lcd.begin(16, 2);     // Initialize the LCD with 16 columns and 2 rows
  lcd.setCursor(0, 0);  // Set the cursor to the first row, first column
  lcd.print("Waiting for message...");
}

void loop() {
  // Check if there is any data available in the serial buffer
    // Read the incoming data into the mystr buffer
    Serial.readBytes(mystr, 8);  
    Serial.println(mystr);
    // Null-terminate the string (ensure it's properly printed)
    mystr[8] = '\0';

    // Print the message to the LCD
    lcd.clear();  // Clear the LCD screen before printing new message
    lcd.setCursor(0, 0);  // Reset the cursor to the first row, first column
    lcd.print(mystr);  // Print the received message
    delay(1000);  // Wait for 1 second to view the message

}