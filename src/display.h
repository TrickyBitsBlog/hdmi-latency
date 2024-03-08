#ifndef DISPLAY_H
#define DISPLAY_H
#pragma once

#include <stdint.h>
#include "peripherals.h"

#define SCALER_CTL0_END                         1U << 31
#define SCALER_CTL0_VALID                       1U << 30
#define SCALER_CTL0_UNITY                       1U << 4

#define SCALER_CTL0_RGBA_EXPAND_ZERO            0
#define SCALER_CTL0_RGBA_EXPAND_LSB             1
#define SCALER_CTL0_RGBA_EXPAND_MSB             2
#define SCALER_CTL0_RGBA_EXPAND_ROUND           3

#define SCALER_CTL0_HFLIP                       1U << 16
#define SCALER_CTL0_VFLIP                       1U << 15

struct hvs_channel {
  volatile uint32_t dispctrl;
  volatile uint32_t dispbkgnd;   // xxRRGGBB, bit 24 (SCALER_DISPBKGND_FILL) is the enable

  volatile uint32_t dispstat;
  // 31:30  mode
  // 29     full
  // 28     empty
  // 17:12  frame count
  // 11:0   line
  volatile uint32_t dispbase;
};

#define SCALER_DISPCTRLX_ENABLE (1<<31)
#define SCALER_DISPCTRLX_RESET  (1<<30)
#define SCALER_DISPCTRL_W(n)    ((n & 0xfff) << 12)
#define SCALER_DISPCTRL_H(n)    (n & 0xfff)
#define SCALER_DISPBKGND_AUTOHS    (1<<31)
#define SCALER_DISPBKGND_INTERLACE (1<<30)
#define SCALER_DISPBKGND_GAMMA     (1<<29)
#define SCALER_DISPBKGND_FILL      (1<<24)


typedef enum {
	/* 8bpp */
	HVS_PIXEL_FORMAT_RGB332 = 0,

	/* 16bpp */
	HVS_PIXEL_FORMAT_RGBA4444 = 1,
	HVS_PIXEL_FORMAT_RGB555 = 2,
	HVS_PIXEL_FORMAT_RGBA5551 = 3,
	HVS_PIXEL_FORMAT_RGB565 = 4,

	/* 24bpp */
	HVS_PIXEL_FORMAT_RGB888 = 5,
	HVS_PIXEL_FORMAT_RGBA6666 = 6,

	/* 32bpp */
	HVS_PIXEL_FORMAT_RGBA8888 = 7,
} hvs_pixel_format;

typedef enum {
    HVS_PIXEL_ORDER_RGBA = 0,
    HVS_PIXEL_ORDER_BGRA = 1,
    HVS_PIXEL_ORDER_ARGB = 2,
    HVS_PIXEL_ORDER_ABGR = 3
} hvs_pixel_order;

//a high level plane description for convenience 
typedef struct {
    hvs_pixel_format format;            // format of the pixels in the plane
    hvs_pixel_order pixel_order;        // order of the components in each pixel
    uint16_t start_x;                   // x position of the left of the plane
    uint16_t start_y;                   // y position of the top of the plane
    uint16_t height;                    // height of the plane, in pixels
    uint16_t width;                     // width of the plane, in pixels
    uint16_t pitch;                     // number of bytes between the start of each scanline
    void* framebuffer;                  // pointer to the pixels in memory
} hvs_plane;



//hardware layout of PV
struct pixel_value {
  volatile uint32_t c;
  volatile uint32_t vc;
  volatile uint32_t vsyncd_even;
  volatile uint32_t horza;
  volatile uint32_t horzb;
  volatile uint32_t verta;
  volatile uint32_t vertb;
  volatile uint32_t verta_even;
  volatile uint32_t vertb_even;
  volatile uint32_t int_enable;
  volatile uint32_t int_status;
  volatile uint32_t h_active;
  volatile uint32_t dsi;
};

//pv interrupts (int_ words above)
#define PV_INTEN_HSYNC_START (1<<0)
#define PV_INTEN_HBP_START   (1<<1)
#define PV_INTEN_HACT_START  (1<<2)
#define PV_INTEN_HFP_START   (1<<3)
#define PV_INTEN_VSYNC_START (1<<4)
#define PV_INTEN_VBP_START   (1<<5)
#define PV_INTEN_VACT_START  (1<<6)
#define PV_INTEN_VFP_START   (1<<7)
#define PV_INTEN_VFP_END     (1<<8)
#define PV_INTEN_IDLE        (1<<9)


#endif
