#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lib/fatfs/ff.h"
#include "lib/fatfs/diskio.h"

#include "lib/serial.h"

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
  PORTCFG.VPCTRLB = PORTCFG_VP02MAP_PORTE_gc | PORTCFG_VP13MAP_PORTC_gc;

  // SET PE0 (LED) to output
  VPORT2.DIR = PIN0_bm;

  /* Start 100Hz system timer with TC0 */
  TCC0.CTRLA = TC_CLKSEL_DIV256_gc;
  TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
  TCC0.PER = 1250; // With a 32 MHz clock and a /256 prescaler, counting to 1250 gives up ~100Hz
  PMIC.CTRL |= PMIC_LOLVLEN_bm; // Enable low-level interrupts (though sei() still needs to be called before interrupts will start)
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

// Timer interrupt for driving SD communication
ISR(TCC0_OVF_vect) {
  TCC0.INTFLAGS |= TC0_OVFIF_bm; // Clear interrupt flag
  disk_timerproc();
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

  FATFS *fs;

  fs = malloc(sizeof(FATFS));
  FRESULT res = f_mount(fs, "", 1);
  switch(res) {
    case FR_OK:
      sendString("FR_OK\n");
    break;
    case FR_INVALID_DRIVE:
      sendString("FR_INVALID_DRIVE\n");
    break;
    case FR_DISK_ERR:
      sendString("FR_DISK_ERR\n");
    break;
    case FR_NOT_READY:
      sendString("FR_NOT_READY\n");
    break;
    case FR_NOT_ENABLED:
      sendString("FR_NOT_ENABLED\n");
    break;
    case FR_NO_FILESYSTEM:
      sendString("FR_NO_FILESYSTEM\n");
    break;
  }

  while (1) {
    ;
    // ADCA.CH0.CTRL |= 0x80;
    // while(!(ADCA.INTFLAGS & ADC_CH0IF_bm));
    // ADCA.INTFLAGS = ADC_CH0IF_bm;
    // output = ADCA.CH0.RES;
    // itoa(output, outstring, 10);
    // sendString(outstring);
    // sendChar('\r');
    // sendChar('\n');
    // _delay_ms(100);
  }
}
