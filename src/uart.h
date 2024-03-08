#ifndef	UART_H
#define	UART_H
#pragma once

#include <stdint.h>
#include "peripherals.h"


//returns the base clock of the uart (needed for uart init)
uint32_t uart_get_clock();
void uart_compute_divider(uint32_t baud, uint32_t* ibrd, uint32_t* fbrd);

// The default config is 8N1, interrupts are off
void uart_init (uint32_t baud ); 

//top bit is set if there is a character - this does not wait
uint32_t uart_probe ( void );

void uart_write ( char c );
int32_t uart_read();

void uart_write_string(const char* str);

//needed for printf
void uart_putc( void* p, char c);
#endif 

