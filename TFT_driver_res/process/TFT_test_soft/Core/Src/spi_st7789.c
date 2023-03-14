#include "spi_st7789.h"

/*static functions declare*/
static uint8_t ST7789_WriteCommand(uint8_t cmd);
static uint8_t ST7789_WriteSmallData(uint8_t data);

void LCD_Writ_Bus(char dat) {
  uint8_t i;

  for (i = 0; i < 8; i++) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, RESET);
    if (dat & 0x80)
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, SET);
    else
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, SET);
    dat <<= 1;
  }
}

/**
 * @brief Write command to ST7789 controller
 * @param cmd -> command to write
 * @return none
 */
static uint8_t ST7789_WriteCommand(uint8_t cmd) {
  //   uint8_t res;
  ST7789_DC_Clr();
  //   return HAL_SPI_TransmitReceive(&ST7789_SPI_PORT, &cmd, &res, sizeof(cmd),
  //                                  HAL_MAX_DELAY);
  //   return HAL_SPI_Transmit(&ST7789_SPI_PORT, &cmd, sizeof(cmd),
  //   HAL_MAX_DELAY);
  LCD_Writ_Bus(cmd);
}

/**
 * @brief Write data to ST7789 controller, simplify for 8bit data.
 * data -> data to write
 * @return none
 */
// static uint8_t ST7789_WriteSmallData(uint8_t data) {
//   uint8_t res;
//   ST7789_DC_Set();
//   res = HAL_SPI_TransmitReceive(&ST7789_SPI_PORT, &data, &res, sizeof(data),
//                                 HAL_MAX_DELAY);
//   return res;
// }
static uint8_t ST7789_WriteSmallData(uint8_t data) {
  //   uint8_t res;
  ST7789_DC_Set();
  //   return HAL_SPI_TransmitReceive(&ST7789_SPI_PORT, &data, &res,
  //   sizeof(data),
  //                                  HAL_MAX_DELAY);
  //   return HAL_SPI_Transmit(&ST7789_SPI_PORT, &data, sizeof(data),
  //   HAL_MAX_DELAY);
  LCD_Writ_Bus(data);
}

/**
 * @brief A Simple test function for ST7789
 * @param  none
 * @return  none
 */
void ST7789_Test(void) {}

// turn off backlight
void ST7789_Backlight_Clr(void) {
  HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_RESET);
}
// turn on backlight
void ST7789_Backlight_Set(void) {
  HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
}

void st7789_init(void) {
  ST7789_RST_Clr();
  HAL_Delay(120);
  ST7789_RST_Set();
  HAL_Delay(20);
  ST7789_Backlight_Set();  // 打开背光
                           //   ST7789_Backlight_Clr();

  //************* Start Initial Sequence **********//

  ST7789_WriteCommand(0x36);
  ST7789_WriteSmallData(0x00);

  ST7789_WriteCommand(0x3A);
  ST7789_WriteSmallData(0x55);

  ST7789_WriteCommand(0xB2);
  ST7789_WriteSmallData(0x0C);
  ST7789_WriteSmallData(0x0C);
  ST7789_WriteSmallData(0x00);
  ST7789_WriteSmallData(0x33);
  ST7789_WriteSmallData(0x33);

  ST7789_WriteCommand(0xB7);
  ST7789_WriteSmallData(0x35);

  ST7789_WriteCommand(0xBB);
  ST7789_WriteSmallData(0x19);

  ST7789_WriteCommand(0xC0);
  ST7789_WriteSmallData(0x2C);

  ST7789_WriteCommand(0xC2);
  ST7789_WriteSmallData(0x01);

  ST7789_WriteCommand(0xC3);
  ST7789_WriteSmallData(0x12);

  ST7789_WriteCommand(0xC4);
  ST7789_WriteSmallData(0x20);

  ST7789_WriteCommand(0xC6);
  ST7789_WriteSmallData(0x0F);

  ST7789_WriteCommand(0xD0);
  ST7789_WriteSmallData(0xA4);
  ST7789_WriteSmallData(0xA1);

  ST7789_WriteCommand(0xE0);
  ST7789_WriteSmallData(0xD0);
  ST7789_WriteSmallData(0x04);
  ST7789_WriteSmallData(0x0D);
  ST7789_WriteSmallData(0x11);
  ST7789_WriteSmallData(0x13);
  ST7789_WriteSmallData(0x2B);
  ST7789_WriteSmallData(0x3F);
  ST7789_WriteSmallData(0x54);
  ST7789_WriteSmallData(0x4C);
  ST7789_WriteSmallData(0x18);
  ST7789_WriteSmallData(0x0D);
  ST7789_WriteSmallData(0x0B);
  ST7789_WriteSmallData(0x1F);
  ST7789_WriteSmallData(0x23);

  ST7789_WriteCommand(0xE1);
  ST7789_WriteSmallData(0xD0);
  ST7789_WriteSmallData(0x04);
  ST7789_WriteSmallData(0x0C);
  ST7789_WriteSmallData(0x11);
  ST7789_WriteSmallData(0x13);
  ST7789_WriteSmallData(0x2C);
  ST7789_WriteSmallData(0x3F);
  ST7789_WriteSmallData(0x44);
  ST7789_WriteSmallData(0x51);
  ST7789_WriteSmallData(0x2F);
  ST7789_WriteSmallData(0x1F);
  ST7789_WriteSmallData(0x1F);
  ST7789_WriteSmallData(0x20);
  ST7789_WriteSmallData(0x23);

  ST7789_WriteCommand(0x21);

  ST7789_WriteCommand(0x11);

  ST7789_WriteCommand(0x29);
}