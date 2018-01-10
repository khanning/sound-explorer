#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DELAY 100 // blink delay

void init_clock(void)
{
  OSC.CTRL = OSC_RC32MEN_bm; // enable 32MHz clock
  while (!(OSC.STATUS & OSC_RC32MRDY_bm)); // wait for clock to be ready
  CCP = CCP_IOREG_gc; // enable protected register change
  CLK.CTRL = CLK_SCLKSEL_RC32M_gc; // switch to 32MHz clock
}

void init_io(void)
{
  // Map VPORT0 and VPORT1 to PORTA and PORTD
  PORTCFG.VPCTRLA = PORTCFG_VP02MAP_PORTA_gc | PORTCFG_VP13MAP_PORTD_gc;
  // Map VPORT2 to PORTE
  PORTCFG.VPCTRLB = PORTCFG_VP02MAP_PORTE_gc;

  // SET PE0 (LED) to output
  VPORT2.DIR = PIN0_bm;
}

void init_adc(void)
{
  CLK.PSCTRL = CLK_PSADIV0_bm | CLK_PSADIV1_bm;
  ADCA.PRESCALER = ADC_PRESCALER2_bm;
  ADCA.CH0.INTCTRL = ADC_CH_INTLVL1_bm | ADC_CH_INTLVL0_bm;
  ADCA.INTFLAGS = ADC_CH0IF_bm;
  ADCA.CTRLB = ADC_CONMODE_bm; // signed mode
  //ADCA.REFCTRL |= ADC_REFSEL0_bm;
  ADCA.REFCTRL = 0x0;
  VPORT0.DIR |= PIN0_bm;
  ADCA.CH0.CTRL = ADC_CH_INPUTMODE0_bm;
  ADCA.CH0.MUXCTRL = 0;
  ADCA.EVCTRL = 0;
  ADCA.CTRLA = ADC_ENABLE_bm;
}

void init_usart(void)
{
  // Set TX (PC2) to output, high
  PORTC.DIR = PIN3_bm;
  PORTC.OUT = PIN3_bm;

  // Set USARTC0 baud rate
  USARTC0.BAUDCTRLB = 0;
  USARTC0.BAUDCTRLA = 0xCF; // BSEL 207.333

  USARTC0.CTRLA = 0;
  USARTC0.CTRLC = USART_CHSIZE_8BIT_gc;
  USARTC0.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
}

void init_button_interrupts(void)
{
  PORTE.PIN1CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
  PORTE.PIN2CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;

  PORTE.INT0MASK = PIN1_bm;
  PORTE.INT1MASK = PIN2_bm;
  PORTE.INTCTRL = PORT_INT0LVL0_bm | PORT_INT1LVL0_bm;
  PMIC.CTRL = PMIC_LOLVLEN_bm;
  sei();
}

ISR(PORTE_INT0_vect)
{
  VPORT2.OUT ^= PIN0_bm;
}

ISR(PORTE_INT1_vect)
{
  VPORT2.OUT = 0;
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

int main(void)
{
  int16_t output;
  char outstring[7];
  init_clock();
  init_io();
  init_adc();
  init_usart();
  init_button_interrupts();

  while (1) {
    ADCA.CH0.CTRL |= 0x80;
    while(!(ADCA.INTFLAGS & ADC_CH0IF_bm));
    ADCA.INTFLAGS = ADC_CH0IF_bm;
    output = ADCA.CH0.RES;
    itoa(output, outstring, 10);
    sendString(outstring);
    sendChar('\r');
    sendChar('\n');
    _delay_ms(100);
  }
}