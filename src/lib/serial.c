#include <avr/io.h>

#include "serial.h"

void init_usart(void)
{
  // Set TX (PC2) to output, high
  PORTC.DIR |= PIN3_bm;
  PORTC.OUT |= PIN3_bm;

  // Set USARTC0 baud rate
  USARTC0.BAUDCTRLB = 0;
  USARTC0.BAUDCTRLA = 0xCF; // BSEL 207.333

  USARTC0.CTRLA = 0;
  USARTC0.CTRLC = USART_CHSIZE_8BIT_gc;
  USARTC0.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
}

void sendChar(char c)
{
 while (!(USARTC0.STATUS & USART_DREIF_bm));
 USARTC0.DATA = c;
}

void sendString(char *text)
{
  while (*text)
  {
    sendChar(*text++);
  }
}
