#pragma GCC diagnostic ignored "-Werror=unused-but-set-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"

#include <Sipeed_ST7789.h>
#include <lcd.h>
#include "dmtx_lcd.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define GPIO_OUTPUT 4


SPIClass spi_(SPI0); // MUST be SPI0 for Maix series on board LCD
Sipeed_ST7789 lcd(SCREEN_WIDTH, SCREEN_HEIGHT, spi_);


uint8_t* data;
int depth = 2;

static int count = 0;
char dm_data[128];



void genDatamatrix() {
  snprintf(dm_data, sizeof(char)*128, "TEST%08d", count);
  count = count + 1;

  dmtx_update_buffer(dm_data, data, SCREEN_WIDTH, SCREEN_HEIGHT, depth);
  lcd.drawImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (uint16_t*)data);

  lcd.setTextSize(2);
  lcd.setTextColor(COLOR_RED);
  lcd.setCursor(10, 220);
  lcd.println(dm_data);
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(15000000, COLOR_RED);
  // put your setup code here, to run once:
  randomSeed(analogRead(0));
  data = (uint8_t*) malloc(SCREEN_WIDTH * SCREEN_HEIGHT * depth * sizeof(uint8_t));
  delay(30);
}

void loop() {
  // put your main code here, to run repeatedly:
  genDatamatrix();
  delay(1000);
}
