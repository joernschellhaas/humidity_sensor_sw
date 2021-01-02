#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans12pt7b.h>

Adafruit_SSD1306 display(128, 64, &Wire);

void setup_display()   {                
  Wire.begin(/* SDA */ D2, /* SCL */ D1);
  // initialize with the I2C addr 0x3C / mit I2C-Adresse 0x3c initialisieren
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  // random start seed / zufälligen Startwert für Random-Funtionen initialisieren
  randomSeed(analogRead(0));
  
  // set text color / Textfarbe setzen
  display.setTextColor(WHITE);
  // set text size / Textgroesse setzen
  display.setTextSize(1);
  // set text cursor position / Textstartposition einstellen
}

#define DRAW_DELAY 0
#define D_NUM 47

int i;

void update_display(float temp, float humidity) {
  display.clearDisplay();
  
  display.setCursor(0,0);
  display.setFont(0);
  display.print("Not connected");
  
  display.setCursor(0,39);
  printTemp(temp);
  
  display.setCursor(69, 39);
  printHum(humidity);

  display.setCursor(0,56);
  // show text / Text anzeigen
  display.setFont(0);
  display.print("built " __DATE__);

  // display.drawRect(0, 0, display.width(), display.height(), WHITE);
  display.display();
}

void printTemp(float temp) {
  display.setFont(&FreeSans12pt7b);
  display.setCursor(display.getCursorX(), display.getCursorY() - 6); // No idea why that is necessary, but otherwise, it will be too low.
  display.print(String(temp, 0));

  // Sub-degree part
  display.setFont(0);
  display.setCursor(display.getCursorX() - 1, display.getCursorY());
  display.print(".");
  display.print(String(((int)(temp * 10)) % 10));
  display.setCursor(display.getCursorX() - 3, display.getCursorY());

  // degree sign
  display.setFont(&FreeSans12pt7b);
  display.drawCircle(display.getCursorX() + 4, display.getCursorY() - 14, 2, WHITE);
  display.drawCircle(display.getCursorX() + 4, display.getCursorY() - 14, 3, WHITE);
  display.setCursor(display.getCursorX() + 2, display.getCursorY());
  
  display.print(" C");
}

void printHum(float humidity) {
  display.print(String(humidity, 0) + "%");
  display.setFont(0);
  display.setCursor(display.getCursorX(), display.getCursorY() - 10);
  display.print("RH");
}
