/*
  Software serial multple serial test

  Receives from the hardware serial, sends to software serial.
  Receives from software serial, sends to hardware serial.

  The circuit:
   RX is digital pin 0 (connect to TX of other device)
   TX is digital pin 1 (connect to RX of other device)

  Note:
  Not all pins on the Mega and Mega 2560 support change interrupts,
  so only the following can be used for RX:
  10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

  Not all pins on the Leonardo support change interrupts,
  so only the following can be used for RX:
  8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

  created back in the mists of time
  modified 25 May 2012
  by Tom Igoe
  based on Mikal Hart's example

  This example code is in the public domain.

*/
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <SWTFT.h> // Hardware-specific library
//#include <TouchScreen.h>
//#include <SD.h>
#define CS   10
#define DC   9
#define RESET  8
#define SD_CS 10
#define touch_threshold 25
// In the SD card, place 24 bit color BMP files (be sure they are 24-bit!)
// There are examples in the sketch folder
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
int a = 1;
// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
SWTFT tft;
#define BOXSIZE 40
#define PENRADIUS 3

SoftwareSerial mySerial(0, 1); // RX, TX
int oldcolor, currentcolor;

void setup()
{
  // Open serial communications and wait for port to open:
  mySerial.begin(9600);
  while (!mySerial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
//  mySerial.println("Hello, world?");

  tft.reset();    // Reset tft LCD
  uint16_t identifier = tft.readID(); // Read ID of the TFT module
//  Serial.print(F("LCD driver chip: ")); // Check if this gets printed on serial monitor of Arduino
//  Serial.println(identifier, HEX);

  tft.begin(identifier);    // Initializing tft LCD

  tft.setRotation(1);   // Change this if display is coming rotated
  tft.fillScreen(BLACK);
  tft.setCursor(30, 190);  //Side, Top
  tft.setTextColor(CYAN); tft.setTextSize(2.5); tft.println("CSIR-CSIO");
  tft.setCursor(190, 170);  //Side, Top
  tft.setTextColor(YELLOW); tft.setTextSize(2); tft.println("Health");
  tft.setCursor(190, 190);  //Side, Top
  tft.setTextColor(YELLOW); tft.setTextSize(2); tft.println("Monitoring");
  tft.setCursor(190, 210);  //Side, Top
  tft.setTextColor(YELLOW); tft.setTextSize(2); tft.println("Panel");

  tft.drawRect(0, 0, 320, 150, GREEN);
  tft.setCursor(10, 65);  //Side, Top
  tft.setTextColor(BLUE); tft.setTextSize(3); tft.println("Waiting for Input");

  currentcolor = RED;
  pinMode(13, OUTPUT);    // For enabling read from touchscreen
}

#define MINPRESSURE 5   // Reduce this if you want softer touch. Softer touch may cause more unintentional change of display
#define MAXPRESSURE 1000

byte SerialInput;
unsigned char caseVariable;
//unsigned char SerialInputPast;

void loop() // run over and over
{
  if (mySerial.available())
  {
    SerialInput = mySerial.read();
//    mySerial.println(SerialInput, HEX);
    // a = BIT failed, b = Test in Progress, c = Power up in Progress, d = BIT report
    if (bitRead(SerialInput, 7) == 1)
      caseVariable = 'c';     // Power up in progress
    else if (bitRead(SerialInput, 6) == 1)
      caseVariable = 'b';     // Test in progress
    else if (bitRead(SerialInput, 0) == 0)
      caseVariable = 'a';     // BIT failed
    else
      caseVariable = 'd';     // BIT report

    tft.fillRect(1, 1, 318, 148, BLACK); //side,top,width,height
  }

  //  mySerial.write(SerialInput);

  switch (caseVariable)
  {
    case 'a':
      tft.setRotation(1);   // Change this if display is coming rotated
      tft.setCursor(50, 50);  //Side, Top
      tft.setTextColor(RED); tft.setTextSize(3); tft.println("Built In Test");
      tft.setCursor(60, 80);  //Side, Top
      tft.setTextColor(RED); tft.setTextSize(3); tft.println("(BIT) failed");
      delay(500);
      tft.fillRect(1, 1, 318, 148, BLACK); //side,top,width,height
      break;
    case 'b':
      tft.setRotation(1);   // Change this if display is coming rotated
      tft.setCursor(90, 50);  //Side, Top
      tft.setTextColor(MAGENTA); tft.setTextSize(3); tft.println("Test in");
      tft.setCursor(85, 80);  //Side, Top
      tft.setTextColor(MAGENTA); tft.setTextSize(3); tft.println("Progress");
      delay(500);
      tft.fillRect(1, 1, 318, 148, BLACK); //side,top,width,height
      break;
    case 'c':
      tft.setRotation(1);   // Change this if display is coming rotated
      tft.setCursor(50, 50);  //Side, Top
      tft.setTextColor(MAGENTA); tft.setTextSize(3); tft.println("Power Up in");
      tft.setCursor(85, 80);  //Side, Top
      tft.setTextColor(MAGENTA); tft.setTextSize(3); tft.println("Progress");
      delay(500);
      tft.fillRect(1, 1, 318, 148, BLACK); //side,top,width,height
      break;
    case 'd':
      tft.setRotation(1);   // Change this if display is coming rotated
      tft.setCursor(20, 5);  //Side, Top
      tft.setTextColor(YELLOW); tft.setTextSize(2); tft.println("HUD Built in Test Report");
      
      tft.setTextColor(WHITE);tft.setCursor(40, 40);  //Side, Top
      tft.println("Video");
      tft.setCursor(220, 40);  //Side, Top
      if (bitRead(SerialInput, 1) == 1)
        {tft.setTextColor(GREEN); tft.println("OK");}
      else
        {tft.setTextColor(RED); tft.println("Error");}
      tft.setTextColor(WHITE);tft.setCursor(40, 60);  //Side, Top
      tft.println("X-Deflection");
      tft.setCursor(220, 60);  //Side, Top
      if (bitRead(SerialInput, 2) == 1)
        {tft.setTextColor(GREEN); tft.println("OK");}
      else
        {tft.setTextColor(RED); tft.println("Error");}
      tft.setTextColor(WHITE);tft.setCursor(40, 80);  //Side, Top
      tft.println("Y-Deflection");
      tft.setCursor(220, 80);  //Side, Top
      if (bitRead(SerialInput, 3) == 1)
        {tft.setTextColor(GREEN); tft.println("OK");}
      else
        {tft.setTextColor(RED); tft.println("Error");}
      tft.setTextColor(WHITE);tft.setCursor(40, 100);  //Side, Top
      tft.println("EHT");
      tft.setCursor(220, 100);  //Side, Top
      if (bitRead(SerialInput, 4) == 1)
        {tft.setTextColor(GREEN); tft.println("OK");}
      else
        {tft.setTextColor(RED); tft.println("Error");}
      tft.setTextColor(WHITE);tft.setCursor(40, 120);  //Side, Top
      tft.println("6.3V/60V");
      tft.setCursor(220, 120);  //Side, Top
      if (bitRead(SerialInput, 5) == 1)
        {tft.setTextColor(GREEN); tft.println("OK");}
      else
        {tft.setTextColor(RED); tft.println("Error");}

      delay(500);
      break;
    default:
      break;
  }

}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  start = micros();
  for (i = min(cx, cy); i > 10; i -= 5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     tft.color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     tft.color565(i, i, 0));
  }

  return t;
}

/*
  // This function opens a Windows Bitmap (BMP) file and
  // displays it at the given coordinates.  It's sped up
  // by reading many pixels worth of data at a time
  // (rather than pixel by pixel).  Increasing the buffer
  // size takes more of the Arduino's precious RAM but
  // makes loading a little faster.  20 pixels seems a
  // good balance.

  #define BUFFPIXEL 20

  void bmpDraw(char *filename, int x, int y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3 * BUFFPIXEL]; // pixel in buffer (R+G+B per pixel)
  uint16_t lcdbuffer[BUFFPIXEL];  // pixel out buffer (16-bit per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
  uint8_t  lcdidx = 0;
  boolean  first = true;

  if ((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');
  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.println(F("File not found"));
    return;
  }

  // Parse BMP header
  if (read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.println(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if (read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if ((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if (bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if ((x + w - 1) >= tft.width())  w = tft.width()  - x;
        if ((y + h - 1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);

        for (row = 0; row < h; row++) { // For each scanline...
          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if (bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col = 0; col < w; col++) { // For each column...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              // Push LCD buffer to the display first
              if (lcdidx > 0) {
                tft.pushColors(lcdbuffer, lcdidx, first);
                lcdidx = 0;
                first  = false;
              }
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            lcdbuffer[lcdidx++] = tft.color565(r, g, b);
          } // end pixel
        } // end scanline
        // Write any remaining data to LCD
        if (lcdidx > 0) {
          tft.pushColors(lcdbuffer, lcdidx, first);
        }
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if (!goodBmp) Serial.println(F("BMP format not recognized."));
  }

  // These read 16- and 32-bit types from the SD card file.
  // BMP data is stored little-endian, Arduino is little-endian too.
  // May need to reverse subscript order if porting elsewhere.

  uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
  }

  uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
  }
*/
