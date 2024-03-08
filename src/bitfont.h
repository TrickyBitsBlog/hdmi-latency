#ifndef BIT_FONT_H
#define BIT_FONT_H
#pragma once

#include <stdint.h>
#include "display.h"

//These fonts are a frame buffer version of https://github.com/PaulStoffregen/ILI9341_fonts
typedef struct
{
	const uint8_t *index;
	const uint8_t *unicode;
	const uint8_t *data;
	uint8_t version;
	uint8_t reserved;
	uint8_t index1_first;
	uint8_t index1_last;
	uint8_t index2_first;
	uint8_t index2_last;
	uint8_t bits_index;
	uint8_t bits_width;
	uint8_t bits_height;
	uint8_t bits_xoffset;
	uint8_t bits_yoffset;
	uint8_t bits_delta;
	uint8_t line_space;
	uint8_t cap_height;
} BitFont;

extern const BitFont Arial_16;

void DrawBitFont(hvs_plane* plane,const BitFont* font, uint32_t x,uint32_t y,const char *str,uint32_t color);

#endif 


