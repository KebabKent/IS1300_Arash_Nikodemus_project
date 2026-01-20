#ifndef __SSD1306_CONF_H__
#define __SSD1306_CONF_H__

#include "main.h"

#define STM32L4
#define SSD1306_USE_SPI

// SPI Configuration
#define SSD1306_SPI_PORT        hspi2
#define SSD1306_CS_Port         Disp_CS_GPIO_Port
#define SSD1306_CS_Pin          Disp_CS_Pin
#define SSD1306_DC_Port         Disp_Data_Instr_GPIO_Port
#define SSD1306_DC_Pin          Disp_Data_Instr_Pin
#define SSD1306_Reset_Port      Disp_Reset_GPIO_Port
#define SSD1306_Reset_Pin       Disp_Reset_Pin

// Mirror the screen if needed
// #define SSD1306_MIRROR_VERT
// #define SSD1306_MIRROR_HORIZ

// Set inverse color if needed
// # define SSD1306_INVERSE_COLOR

// Include only needed fonts
#define SSD1306_INCLUDE_FONT_6x8
#define SSD1306_INCLUDE_FONT_7x10
#define SSD1306_INCLUDE_FONT_11x18
#define SSD1306_INCLUDE_FONT_16x26

#define SSD1306_INCLUDE_FONT_16x24

#define SSD1306_INCLUDE_FONT_16x15

// The width of the screen can be set using this
// define. The default value is 128.
// #define SSD1306_WIDTH           64

// If your screen horizontal axis does not start
// in column 0 you can use this define to
// adjust the horizontal offset
// #define SSD1306_X_OFFSET

// The height can be changed as well if necessary.
// It can be 32, 64 or 128. The default value is 64.
// #define SSD1306_HEIGHT          64

#endif /* __SSD1306_CONF_H__ */
