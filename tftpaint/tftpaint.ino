#include <SPI.h>



#include <Adafruit_GFX.h>    // Core graphics library
#include <SWTFT.h> // Hardware-specific library
#include <TouchScreen.h>
#include <SD.h>

#define SD_CS 10

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
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
  int a = 1;



// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


SWTFT tft;

#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("Paint!"));
  
  tft.reset();
  
  uint16_t identifier = tft.readID();

  Serial.print(F("LCD driver chip: "));
  Serial.println(identifier, HEX);
    

  tft.begin(identifier);


  tft.fillScreen(BLACK);

  tft.setRotation(1);
  
//  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
//  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
//  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, GREEN);
//  tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, CYAN);
//  tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, BLUE);
//  tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, MAGENTA);
  //tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, WHITE);
 
//  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
   unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextSize(2); tft.println();
  tft.setTextSize(2); tft.println();
  tft.setTextColor(GREEN); tft.setTextSize(4); tft.println("   HUD-LCA");
  tft.setTextSize(2); tft.println();
  tft.setTextColor(YELLOW); tft.setTextSize(2); tft.println("      Designed by:");
  tft.setTextSize(1); tft.println();
  tft.setTextColor(RED); tft.setTextSize(3); tft.println("    CSIR-CSIO");
  tft.setTextSize(2); tft.println();
  tft.setTextColor(YELLOW); tft.setTextSize(2); tft.println("      Manufactured by:");
  tft.setTextSize(1); tft.println();
  tft.setTextColor(RED); tft.setTextSize(3); tft.println("    BEL");
  int x = 2*8+2*8+4*8+2*8+2*8+1*8 + 4;
  int y = 3*3*8;
  int w = 3*8*7;
  int h = 3*9;
//  tft.drawRect(x, y, w, h, WHITE);

  tft.println();

  currentcolor = RED;
 
  pinMode(13, OUTPUT);

}

#define MINPRESSURE 5
#define MAXPRESSURE 1000

void loop()
{

  digitalWrite(13, HIGH);
  // Recently Point was renamed TSPoint in the TouchScreen library
  // If you are using an older version of the library, use the
  // commented definition instead.
  // Point p = ts.getPoint();
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
switch (a)
{
  case 1:
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    /*
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z);
    */
  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles());
  delay(50);
   

    digitalWrite(13, HIGH);
  // Recently Point was renamed TSPoint in the TouchScreen library
  // If you are using an older version of the library, use the
  // commented definition instead.
  // Point p = ts.getPoint();
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  a = 2;
  }
  break;
  case 2:
if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {

  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(2); tft.println();
  tft.setTextSize(2); tft.println();
  tft.setTextColor(GREEN); tft.setTextSize(4); tft.println("   HUD-LCA");
  tft.setTextSize(2); tft.println();
  tft.setTextColor(YELLOW); tft.setTextSize(2); tft.println("      Designed by:");
  tft.setTextSize(1); tft.println();
  tft.setTextColor(RED); tft.setTextSize(3); tft.println("    CSIR-CSIO");
  tft.setTextSize(2); tft.println();
  tft.setTextColor(YELLOW); tft.setTextSize(2); tft.println("      Manufactured by:");
  tft.setTextSize(1); tft.println();
  tft.setTextColor(RED); tft.setTextSize(3); tft.println("    BEL");

a = 1;
  }
  break;
}
/*    if (p.y < (TS_MINY-5)) {
      Serial.println("erase");
      // press the bottom of the screen to erase 
      tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, BLACK);
    }
    // scale from 0->1023 to tft.width
//    p.x = tft.width()-(map(p.x, TS_MINX, TS_MAXX, tft.width(), 0));
//    p.y = tft.height()-(map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = tft.height()-(map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
    /*
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");
    
    if (p.y < BOXSIZE) {
       oldcolor = currentcolor;

       if (p.x < BOXSIZE) { 
         currentcolor = RED; 
         tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*2) {
         currentcolor = YELLOW;
         tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*3) {
         currentcolor = GREEN;
         tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*4) {
         currentcolor = CYAN;
         tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*5) {
         currentcolor = BLUE;
         tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*6) {
         currentcolor = MAGENTA;
         tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, WHITE);
       }

       if (oldcolor != currentcolor) {
          if (oldcolor == RED) tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
          if (oldcolor == YELLOW) tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
          if (oldcolor == GREEN) tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, GREEN);
          if (oldcolor == CYAN) tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, CYAN);
          if (oldcolor == BLUE) tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, BLUE);
          if (oldcolor == MAGENTA) tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, MAGENTA);
       }
    }
    if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height())) {
      tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
    }
  */
  

}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(i, i, 0));
  }

  return t;
}

