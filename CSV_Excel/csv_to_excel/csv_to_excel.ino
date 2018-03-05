/*
  LiquidCrystal Library - Hello World

  Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
  library works with all LCD displays that are compatible with the
  Hitachi HD44780 driver. There are many of them out there, and you
  can usually tell them by the 16-pin interface.

  This sketch prints "Hello World!" to the LCD
  and shows the time.

  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008
  by David A. Mellis
  library modified 5 Jul 2009
  by Limor Fried (http://www.ladyada.net)
  example added 9 Jul 2009
  by Tom Igoe
  modified 22 Nov 2010
  by Tom Igoe
  modified 7 Nov 2016
  by Arturo Guadalupi

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/
/*
  SD card file dump

  This example shows how to read a file from the SD card using the
  SD library and send it over the serial port.

  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created  22 December 2010
  by Limor Fried
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 39, en = 40, d4 = 41, d5 = 42, d6 = 43, d7 = 44;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int chipSelect = 4;


void setup() {
  pinMode(25, INPUT);   // Pin connected to DTx button

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  lcd.print("Press DTx");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    lcd.print("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  //  Serial.println("card initialized.");

}

int buttonState = 0;         // variable for reading the pushbutton status

void loop() {
  buttonState = digitalRead(25);

  if (buttonState)
  {
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("VOTE.CSV");
    lcd.clear();
    lcd.setCursor(0, 1);

    // if the file is available, write to it:
    if (dataFile) {
      while (dataFile.available()) {
        Serial.write(dataFile.read());
      }
      dataFile.close();
      lcd.print(".csv read done");

    }
    // if the file isn't open, pop up an error:
    else {
      lcd.print("error opening VOTE.CSV");
    }

  }

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //  lcd.print(millis() / 1000);
}

