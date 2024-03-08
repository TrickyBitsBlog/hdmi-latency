.set TIMER_LO, 0x3f003004

//https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
.set MAILBOX_BASE, 0x3f00b880
.set MAILBOX_STATUS_1, 0x38
.set MAILBOX_RW_0, 0x00
.set MAILBOX_STATUS_0, 0x18
.set MAILBOX_RW_1, 0x20

.set PROPERTY, 8
.set BIT_FULL, 31
.set BIT_EMPTY, 30


// arbitrary "150ish cycles" (destroys ip1/x17), then add a memory barrier
// useful when setting GPIO pull up/down clocks
.global delay_small
delay_small:
  mov ip1, #100
1:
  subs ip1, ip1, #1
  bpl 1b
  dmb sy
  ret

// use the system timer to count wall time microseconds.
// NOTE: The counter is a 64 bit counter but its not aligned as has to be be read
//       as a pair of 32bit values (as it would have been in Arm32). This code
//       only uses the 32bit low value which wraps every 4294.967296 seconds (1hr 12ish mins)
// W0 - microseconds to wait
.global delay_usec
delay_usec: 
  ldr w2, =TIMER_LO
  ldr w1, [x2]
  add w1, w0, w1
  dmb sy
1:
  ldr w0, [x2]
  cmp w0, w1
  b.ls 1b
  ret


.globl write32
write32:
	str w1,[x0]
	ret

.globl read32
read32:
	ldr w0,[x0]
	ret

.globl write64
write64:
	ldr x0,[x0]
	ret

.globl read64
read64:
	str x1,[x0]
	ret

//writes 8 bytes per loop
.globl memzero
memzero:
	str xzr, [x0], #8
	subs x1, x1, #8
	b.gt memzero
	ret


// [w0: 32-bit addr]
.global mailbox_ch8_send_sync
mailbox_ch8_send_sync:
  mov x3, lr
  ldr w1, =MAILBOX_BASE
1:
  bl delay_small
  ldr w2, [x1, #MAILBOX_STATUS_1]
  tbnz w2, #BIT_FULL, 1b
  add w0, w0, #PROPERTY
  str w0, [x1, #MAILBOX_RW_1]

  // now wait for the reply..This is not totally safe because it assumes the reply
  // is from what we just sent. In a bare metal world this would be true but it would
  // be better to double check the reply is the one we think it is.
  // We'll fix it when we write it in C
2:
  bl delay_small
  ldr w2, [x1, #MAILBOX_STATUS_0]
  tbnz w2, #BIT_EMPTY, 2b
  ldr w0, [x1, #MAILBOX_RW_0]
  ret x3
