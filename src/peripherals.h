#ifndef	PERIPHERLS_H
#define	PERIPHERLS_H

#pragma once

//in bare metal without the MMU the peripheral base is at 0x3f000000
#define PBASE 0x3F000000


//microsecond timer
//in 64bit mode you cannot read this timer as a 64bit value, it has to be read (safely) as a pair of 32bit values
#define TIMER_LO 0x3f003004
#define TIMER_HI 0x3f003008


//GPIO
#define GPFSEL0         (PBASE+0x00200000)    //GPIO0-9 FUNCTION
#define GPFSEL1         (PBASE+0x00200004)    //GPIO10-19
#define GPFSEL2         (PBASE+0x00200008)    //GPIO20-29
#define GPSET0          (PBASE+0x0020001C)
#define GPSET1          (PBASE+0x00200020)
#define GPCLR0          (PBASE+0x00200028)
#define GPCLR1          (PBASE+0x0020002C)
#define GPLEVEL0        (PBASE+0x00200034)
#define GPLEVEL1        (PBASE+0x00200038)
#define GPPUD           (PBASE+0x00200094)
#define GPPUDCLK0       (PBASE+0x00200098)
#define GPPUDCLK1       (PBASE+0x0020009C)

//UART
#define UART_BASE (PBASE + 0x201000)
#define UART_DR       (UART_BASE + 0x00)
#define UART_RDRECR   (UART_BASE + 0x04)
#define UART_FR       (UART_BASE + 0x18)
#define UART_IBRD     (UART_BASE + 0x24)
#define UART_FBRD     (UART_BASE + 0x28)
#define UART_LCRH     (UART_BASE + 0x2c)
#define UART_CR       (UART_BASE + 0x30)
#define UART_IFLS     (UART_BASE + 0x34)
#define UART_IMSC     (UART_BASE + 0x38)
#define UART_RIS      (UART_BASE + 0x3c)
#define UART_MIS      (UART_BASE + 0x40)
#define UART_ICR      (UART_BASE + 0x44)
#define UART_DMACR    (UART_BASE + 0x48)

#define TXPIN 14
#define RXPIN 15

//PV
#define PV0_BASE (PBASE + 0x206000)
#define PV1_BASE (PBASE + 0x207000)
#define PV2_BASE (PBASE + 0x807000)

//HVS
#define SCALER_LIST_MEMORY (PBASE + 0x402000)
#define SCALER_BASE (PBASE + 0x400000)
#define SCALER_DISPCTRL     (SCALER_BASE + 0x00)
#define SCALER_DISPSTAT     (SCALER_BASE + 0x04)
#define SCALER_DISPEOLN     (SCALER_BASE + 0x18)
#define SCALER_DISPLIST0    (SCALER_BASE + 0x20)
#define SCALER_DISPLIST1    (SCALER_BASE + 0x24)
#define SCALER_DISPLIST2    (SCALER_BASE + 0x28)
#define SCALER_DISPCTRL0    (SCALER_BASE + 0x40)

//VEC (stdv encoder)
#define VEC_BASE (PBASE + 0x806000)

#define VEC_WSE_RESET   (VEC_BASE + 0x0c0)
#define VEC_WSE_CONTROL (VEC_BASE + 0x0c4)
#define VEC_CONFIG0     (VEC_BASE + 0x104)
#define VEC_SCHPH       (VEC_BASE + 0x108)
#define VEC_SOFT_RESET  (VEC_BASE + 0x10c)
#define VEC_CLMP0_START (VEC_BASE + 0x144)
#define VEC_CLMP0_END   (VEC_BASE + 0x148)
#define VEC_FREQ3_2     (VEC_BASE + 0x180)
#define VEC_FREQ1_0     (VEC_BASE + 0x184)
#define VEC_CONFIG1     (VEC_BASE + 0x188)
#define VEC_CONFIG2     (VEC_BASE + 0x18c)
#define VEC_CONFIG3     (VEC_BASE + 0x1a0)
#define VEC_MASK0       (VEC_BASE + 0x204)
#define VEC_CFG         (VEC_BASE + 0x208)
#define VEC_DAC_MISC    (VEC_BASE + 0x214)


#endif
