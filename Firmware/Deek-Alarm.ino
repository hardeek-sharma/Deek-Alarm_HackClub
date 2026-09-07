#include <Adafruit_GFX.h> // graphics library
#include <Adafruit_ST7789.h> // driver for the ST7789 screen
#include <SPI.h> 


//
// SCREEN
//
// Defining pins for the display, change according to your setup!!! Uses the white numbers on the ESP
#define TFT_SCLK 9 // labeled SCL on the screen
#define TFT_MOSI 10 // labeled SDA on the screen
#define TFT_RST 8
#define TFT_DC 4
#define TFT_CS 5
#define TFT_BL 6

// Fix setColRowStart() by exposing it via a subclass
class MyST7789 : public Adafruit_ST7789 {
public:
  MyST7789(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst)
    : Adafruit_ST7789(cs, dc, mosi, sclk, rst) {}
  void setOffsets(uint8_t col, uint8_t row) {
    _colstart = _colstart2 = col;
    _rowstart = _rowstart2 = row;
  }
};

MyST7789 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);



//
// TIME TRACKER
//
unsigned long previousMillis = 0;
const int interval = 1000;
unsigned int second = 0;
int minute = 0;
int hour = 12;
String meridiem = "AM"; // PM




// setup() runs ONCE when the board powers on
void setup() {
  Serial.begin(115200); // lets the board talk to your computer

  pinMode(TFT_BL, OUTPUT); // Set the backlight pin mode, or just wire it to 3.3V
  digitalWrite(TFT_BL, LOW); // Turns the backlight ON, for some reason this screen is active Low, so setting it to LOW is really HIGH

  tft.init(76, 284); // Our panel size (portrait)
  tft.setOffsets(82, 18); // Offsets for the weird resolution
  tft.invertDisplay(false); // Invert the colors (This display is flipped from normal)
  tft.setRotation(1); // Landscape, if it's upside down use 3!
  Serial.println("TFT Initialized!");

  tft.fillScreen(ST77XX_BLACK); // clear the screen

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(6);
  tft.setCursor(0, 0); // Where the text is drawn, 0,0 is top left
  tft.print("TEST"); // Show whatever you want! Draws from the top left of the text/number/shape 
}

// loop() runs OVER and OVER, forever
void loop() {
  // add your code here!
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    second++;
    
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK)
    tft.setCursor(0, 0); // Set back to start
    
    Serial.print("Seconds elapsed: ");
    Serial.println(second);
  }

  if (second >= 60) {
    minute++;
    second = 0;
  }

  if (minute >= 60) {
    hour++;
    minute = 0;
  }

  if (hour > 12) {
    meridiem = (meridiem == "AM") ? "PM" : "AM";

    hour = 1;
  }

  tft.print(hour);
  tft.print(":");
  tft.print(minute);
  tft.print(":");
  tft.print(second);
  tft.print(" ");
  tft.print(meridiem);
}