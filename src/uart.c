#include <stdint.h>
#include "utils.h"
#include "uart.h"

#define TAG_CLOCK_GET_RATE 0x00030002
#define CLOCK_UART 2
//Get the UART core clock (from property mailbox)
//https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
uint32_t uart_get_clock()
{
  //this should naturally always be 16 byte aligned due to being on the aarch64 stack
  //this satisfies the 28bit MSB alignment of the mailbox data (it has to be 32bits too)
  uint32_t buffer[8];               
  buffer[0] = sizeof(buffer);       //size of the request
  buffer[1] = 0;                    //0x00000000=request (will be the response code)
  
  //series of concatenated tags
  buffer[2] = TAG_CLOCK_GET_RATE;
  buffer[3] = 8;                    //input/output buffer size for the tag
  buffer[4] = 0;                    //request/response
  buffer[5] = CLOCK_UART;           //data0: input: clock id  output: clock id
  buffer[6] = 0;                    //data1: result: rate in hz

  buffer[7] = 0;                    //end tag

  //synchronous mailbox (data is on the stack below 0x80000)
  //the buffer address is a physical address and must fit in 32bits)
  mailbox_ch8_send_sync(PTR32(buffer));

  //check if we succeeded 

  return buffer[6];
}

void uart_compute_divider(uint32_t baud, uint32_t* ibrd, uint32_t* fbrd)
{
  // the UART baud rate is set from the the UART base frequency which is dependent on core-freq. 
  // We can get the UART base clock from the property mailbox (see uart_get_clock). Computing
  // the UART divider from the base clock is much more reliable than hard coding a divder.  
  // The baud divisor is stored as 16.6 fixed point, with the integer part in IBRD and the
  // fraction in FBRD.
  // The manual says the divisor is calculated from `uart_clock / (16 * bps)`.
  // to get 6 fraction bits: (uart_clock*64)/(16*bps) = (uart_clock*4)/bps
  //
  // On the raspi3 using force_turbo=1 changes the core-freq from 250 to 400Mhz, this code will
  // correctly compute the diver.  If you change the core clock on the fly then the UART divider
  // needs to be recalculated.
  uint64_t clock = uart_get_clock();
  uint64_t res = (clock*4)/baud;
  *fbrd = (uint32_t)(res & 0x3f);
  *ibrd = (uint32_t)(res>>6);
}

/*
  uint32_t fs2;
  fs2 = *((volatile uint32_t*)GPFSEL2);
  fs2 &= ~(0x7<<9); 
  fs2 |= (1<<9);      //001 = output

  //GPIO24 as input from the detector
  //14-12 function select for GPIO24 (000=input)
  fs2 &= ~(0x7<<12);
  *((volatile uint32_t*)GPFSEL2) = fs2;

*/

void uart_init(uint32_t brd)
{
  uint32_t ibrd;
  uint32_t fbrd;

  uart_compute_divider(brd,&ibrd,&fbrd);

  // shut off the uart
  write32(UART_CR,0);

  // configure the RX/TX so they're not attached to the default pulldown
  write32(GPPUD,0);
  delay_small();
  write32(GPPUDCLK0,(1<<RXPIN) | (1<<TXPIN));
  write32(GPPUDCLK1,0);
  delay_small();
  write32(GPPUDCLK0,0);
  write32(GPPUDCLK1,0);

  // clear all pending interrupts
  write32(UART_ICR,0x7ff);

  // set bps
  write32(UART_IBRD,ibrd);
  write32(UART_FBRD,fbrd);

  // use fifo, set 8N1, and re-enable the uart
  write32(UART_LCRH,0x70);
  write32(UART_IMSC,0x7ff);
  write32(UART_CR,0x301);  //enable RX/TX
}


void uart_write(char c)
{
  //spin while the Transmit FIFO is full
  while((read32(UART_FR) & (1<<5)))
  {
  }
  //write the character once there is room
  write32(UART_DR,c);
}

//this is none blocking, it returns -1 if there is nothing to read
int32_t uart_read()
{
  //if the RX FIFO is empty return -1
  if (read32(UART_FR) & (1<<4))
    return -1;
  return (int32_t)(read32(UART_DR) & 0xff);
}

void uart_write_string(const char* str)
{
	for (int i = 0; str[i] != '\0'; i ++) {
		uart_write((char)str[i]);
	}
}

// This function is required by printf
void uart_putc ( void* p, char c)
{
	uart_write(c);
}
