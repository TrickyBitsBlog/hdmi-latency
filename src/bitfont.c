//https://github.com/PaulStoffregen/ILI9341_fonts
#include "bitfont.h"
#include "display.h"

static int32_t cursor_x = 50;
static int32_t cursor_y = 50;
static uint32_t textcolor = 0xffff;


//---------------------------------------------------------------------------------------------------
static inline uint32_t fetchbit(const uint8_t *p, uint32_t index)
{
	if (p[index >> 3] & (1 << (7 - (index & 7)))) 
    return 1;

	return 0;
}

//---------------------------------------------------------------------------------------------------
static uint32_t fetchbits_unsigned(const uint8_t *p, uint32_t index, uint32_t required)
{
	uint32_t val = 0;
	do 
  {
		uint8_t b = p[index >> 3];
		uint32_t avail = 8 - (index & 7);
		if (avail <= required) 
    {
			val <<= avail;
			val |= b & ((1 << avail) - 1);
			index += avail;
			required -= avail;
		} 
    else 
    {
			b >>= avail - required;
			val <<= required;
			val |= b & ((1 << required) - 1);
			break;
		}
	} while (required);

	return val;
}

//---------------------------------------------------------------------------------------------------
static inline uint32_t fetchbits_signed(const uint8_t *p, uint32_t index, uint32_t required)
{
	uint32_t val = fetchbits_unsigned(p, index, required);
	if (val & (1 << (required - 1))) 
  {
		return (int32_t)val - (1 << required);
	}
	return (int32_t)val;
}

//---------------------------------------------------------------------------------------------------
//HARDCODED for 16bit color
static inline void  drawFastHLine(hvs_plane* plane,int32_t x, int32_t y, int32_t w, uint32_t textcolor)
{
  uint32_t offset = (plane->pitch*y)+(x*2);
  uint16_t* adr = (uint16_t*)(((uint8_t*)plane->framebuffer)+offset);
  while (w>0)
  {
    *adr=textcolor;
    adr++;
    w--;
  }
}

//---------------------------------------------------------------------------------------------------
static void drawFontBits(hvs_plane* plane, uint32_t bits, uint32_t numbits, uint32_t x, uint32_t y, uint32_t repeat)
{
	if (bits == 0) return;
		
	int w = 0;
	do 
  {
		uint32_t x1 = x;
		uint32_t n = numbits;		
		
		do 
    {
			n--;
			if (bits & (1 << n)) 
      {
				w++;
			}
			else if (w > 0) 
      {
 				drawFastHLine(plane,x1 - w, y, w, textcolor);
        w=0;
			}
						
			x1++;
		} while (n > 0);

		if (w > 0) 
    {
				drawFastHLine(plane,x1 - w, y, w, textcolor);
        w=0;
		}
		
		y++;
		repeat--;
	} while (repeat);
}


void drawFontChar(hvs_plane* plane, const BitFont* font, unsigned int c)
{
	uint32_t bitoffset;
	const uint8_t *data;

	if (c >= font->index1_first && c <= font->index1_last) 
	{
		bitoffset = c - font->index1_first;
		bitoffset *= font->bits_index;
	} 
	else if (c >= font->index2_first && c <= font->index2_last) 
	{
		bitoffset = c - font->index2_first + font->index1_last - font->index1_first + 1;
		bitoffset *= font->bits_index;
	} 
	else if (font->unicode) 
	{
		return;
	} 
	else 
	{
		return;
	}
	
	data = font->data + fetchbits_unsigned(font->index, bitoffset, font->bits_index);

	uint32_t encoding = fetchbits_unsigned(data, 0, 3);
	if (encoding != 0) return;
	uint32_t width = fetchbits_unsigned(data, 3, font->bits_width);
	bitoffset = font->bits_width + 3;
	uint32_t height = fetchbits_unsigned(data, bitoffset, font->bits_height);
	bitoffset += font->bits_height;

	int32_t xoffset = fetchbits_signed(data, bitoffset, font->bits_xoffset);
	bitoffset += font->bits_xoffset;
	int32_t yoffset = fetchbits_signed(data, bitoffset, font->bits_yoffset);
	bitoffset += font->bits_yoffset;

	uint32_t delta = fetchbits_unsigned(data, bitoffset, font->bits_delta);
	bitoffset += font->bits_delta;

	// horizontally, we draw every pixel, or none at all
	if (cursor_x < 0) 
    cursor_x = 0;
	int32_t origin_x = cursor_x + xoffset;
	if (origin_x < 0) 
  {
		cursor_x -= xoffset;
		origin_x = 0;
	}
	if (origin_x + width > plane->width) 
  {
		origin_x = 0;
		if (xoffset >= 0) 
    {
			cursor_x = 0;
		} else {
			cursor_x = -xoffset;
		}
		cursor_y += font->line_space;
	}
	if (cursor_y >= (int32_t)plane->height) return;
	cursor_x += delta;

	// vertically, the top and/or bottom can be clipped
	int32_t origin_y = cursor_y + font->cap_height - height - yoffset;
	int32_t linecount = height;
	uint32_t y = origin_y;
	while (linecount) 
  {	
		uint32_t b = fetchbit(data, bitoffset++);
		if (b == 0) 
    {
			uint32_t x = 0;
			do {
				uint32_t xsize = width - x;
				if (xsize > 32) 
          xsize = 32;
				uint32_t bits = fetchbits_unsigned(data, bitoffset, xsize);
				drawFontBits(plane,bits, xsize, origin_x + x, y, 1);
				bitoffset += xsize;
				x += xsize;
			} while (x < width);
			y++;
			linecount--;
		} 
    else 
    {
			uint32_t n = fetchbits_unsigned(data, bitoffset, 3) + 2;
			bitoffset += 3;
			uint32_t x = 0;
			do 
      {
				uint32_t xsize = width - x;
				if (xsize > 32) 
          xsize = 32;
				uint32_t bits = fetchbits_unsigned(data, bitoffset, xsize);
				drawFontBits(plane,bits, xsize, origin_x + x, y, n);
				bitoffset += xsize;
				x += xsize;
			} while (x < width);
			y += n;
			linecount -= n;
		}
	}
}

void DrawBitFont(hvs_plane* plane,const BitFont* font, uint32_t x,uint32_t y,const char *str,uint32_t color)
{  
	cursor_x = x;
	cursor_y = y;
	textcolor = color;
	
	while(*str!='\0')
	{	
		drawFontChar(plane,font, (*str) );
		str++;
	}	
}