#include "LPC8xx.h"
#include "timer.h"

// Variabili globali per ISR
volatile uint32_t elapsed_time = 0;
extern volatile uint8_t cc, ss, mm, hh;

void InitMRT(void)
{
  // *** Configurazione MRT in modalità one-shot per generazione periodi di pausa ***

  // Abilita il clock per MRT
  LPC_SYSCON->SYSAHBCLKCTRL0 |= (1<<10);

  // Reset del modulo
  LPC_SYSCON->PRESETCTRL0 &= ~(1<<10);
  LPC_SYSCON->PRESETCTRL0 |=  (1<<10);

  // MODE: one-shot; interrupt = enable
  LPC_MRT->Channel[0].CTRL = (1<<0) | (1<<1);

  // MODE: ripetitivo; interrupt = enable
   LPC_MRT->Channel[1].CTRL = (1<<0) | (0<<1);

  // Connette e abilita l'interrupt dello MRT al NVIC
  NVIC_EnableIRQ(MRT_IRQn);

  // VAL = 0 => MRT0 in idle
  LPC_MRT->Channel[0].INTVAL = 0; //già presente al reset



  // cancella ogni richiesta (spuria?) di interrupt
  LPC_MRT->Channel[0].STAT = 0x1;
  LPC_MRT->Channel[1].STAT = 0x1;
}

void delayMs(uint32_t delay_in_ms)
{
	LPC_MRT->Channel[0].INTVAL = (30000000/1000)*delay_in_ms;

	// Aspetta l'handshake dalla ISR
	while(elapsed_time == 0);

	elapsed_time = 0; // resetta flag di handshake
}

void delayUs(uint32_t delay_in_us)
{
	LPC_MRT->Channel[0].INTVAL = (30000000/1000000)*delay_in_us;

	// Aspetta l'handshake dalla ISR
	while(elapsed_time == 0);

	elapsed_time = 0; // resetta flag di handshake
}

void MRT_IRQHandler(void) {

  // Se è '1' il bit 0 delle interrupt flag CH0 ha dato un interrupt
  if (LPC_MRT->IRQ_FLAG & (0x1))
  {
	  elapsed_time = 1;

    // Clear the interrupt flag
    LPC_MRT->Channel[0].STAT = 0x1;
  }

  // Se è '1' il bit 1 delle interrupt flag CH1 ha dato un interrupt
  if (LPC_MRT->IRQ_FLAG & (0x2))
  {	cc++;
  	  if(cc == 100)
  	  {
  	  cc=0;
	  ss++;
	   if(ss == 60)
	   {
		   ss = 0;
		   mm++;
		   if( mm == 60)
		   {
			   mm == 0;
			   hh++;
			   if(hh == 24)
				   hh = 0;
		   }
	   }
  	  }

	  // Clear the interrupt flag
      LPC_MRT->Channel[1].STAT = 0x1;
  }

  return;
}
