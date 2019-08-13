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

uint16_t load(uint16_t adress)
{
  return memory[adress];
}
uint8_t displayDriver(uint8_t x, uint8_t y, uint8_t sprite)
{
  for(uint8_t i = 7; i <= 4; i--)
  {
     uint8_t pixel = 0x01 & (sprite>>i);
     if(pixel == 1)
      display.drawPixel(x,y, WHITE);
     else
      display.drawPixel(x,y, BLACK);
     
  }
  display.display();
}
struct CHIP8_machine cm8;
void setup() {
  Serial.begin(9600);
  
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADRESS);
  display.clearDisplay();
  init_Chip8(&cm8);
  cm8.memory = &load;
  cm8.dispDriver = &displayDriver; 

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(cm8.PC);
  run_Chip8(&cm8);
}
