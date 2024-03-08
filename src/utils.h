#ifndef	UTILS_H
#define	UTILS_H

#pragma once

#include <stdint.h>

//make a 32bit pointer int from a 64bit C pointer
#define PTR32(ptr) (uint32_t)((uint64_t)(ptr))

extern void delay_small();
extern void delay_usec ( uint32_t us);
extern void memzero(void* src, uint64_t n);  //n is the number of 64bit words to store

//try optimizing these out Mr Compiler!
//easy to use functions that use an integer address
extern void write32 ( uint64_t adr, uint32_t data );
extern uint32_t read32 ( uint64_t adr );
extern void write64 ( uint64_t adr, uint64_t data );
extern uint64_t read64 ( uint64_t adr);


//send a ch8/property request to the mailbox
//has to be a 32bit pointer hence the uint32_t
extern void mailbox_ch8_send_sync(uint32_t adr32);

#endif
