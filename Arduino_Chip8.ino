#define DEBUG 1
#include"rom.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DISPLAY_ADRESS 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
extern "C" {
  #include"chip8.h"
}

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
struct CHIP8_machine cm8;
uint16_t load(uint16_t adress)
{
  uint16_t internalAdress = (adress-0x200);
  uint16_t cache = (memory[internalAdress]<<8)+memory[internalAdress+1];
  Serial.println(String(internalAdress, HEX)+String(" | ")+String(cache, HEX)+String(" | ")+String(adress, HEX)+String(" | ")+String(cm8.ucRegister[0], HEX)+String(" | ")+String(cm8.ucRegister[1], HEX));
  return cache;
}
uint8_t displayDriver(uint8_t x, uint8_t y, uint8_t sprite)
{
  Serial.println(String(sprite, BIN));
  for(uint8_t i = 8; i != 0; i--)
  {
     uint8_t pixel = 0x01 & (sprite>>(i-1));
     //Serial.println(String(sprite, BIN)+"<"+String(x+(8-i), BIN)+"<"+String(y, BIN));
     if(pixel == 1)
      display.drawPixel(x+(8-i)+32,y+16, WHITE);
     else
      display.drawPixel(x+(8-i)+32,y+16, BLACK);

//     display.drawBitmap(x+(8-i), y, sprite, 1, 1, WHITE);
     
  }
  display.display();
}
void cls(void)
{
    display.clearDisplay();
}

void setup() {
  Serial.begin(9600);
  
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADRESS);
  display.clearDisplay();

 
  
  cm8.memory = &load;
  cm8.dispDriver = &displayDriver; 
  cm8.cls = &cls;
  init_Chip8(&cm8);

}

void loop() {
  run_Chip8(&cm8);
}
