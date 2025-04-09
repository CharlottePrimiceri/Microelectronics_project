#include "LPC8xx.h"
#include "HD44780.h"
#include "timer.h"


volatile uint8_t ss = 0;
volatile uint8_t mm = 0;
volatile uint8_t hh = 0;
volatile uint8_t cc = 0;

#define SET_OO 1




void Write_ndigitsval0(unsigned int dummyVal, unsigned char ndigits)
{
	unsigned int ten_base=1, digit;

	while(--ndigits)
		ten_base*=10;

	while(ten_base)
	{
		digit = dummyVal/ten_base;
		WriteChar(NUM_TO_CODE(digit));
		dummyVal %= ten_base;
		ten_base/=10;
	}
}

int main(void) {
	uint8_t pos = 0;  //variabile che indica se stiamo modificando le ore o i minuti
	int x;
	ss = 0;           //settaggio secondi pari a 0 quando si rientra nello stato 0
	uint16_t C;       //variabili che indicano i gradi e i kelvin

	uint8_t stato = 0; //variabile che indica lo stato attuale (si parte da VIS)
	uint8_t USER_pressed = 0; //flag pulsante USER premuto (P0.4)
	uint8_t ISP_pressed = 0; //flag pulsante ISP premuto (P0.12)


	    //inizio  power-up, il reset della periferica e l’abilitazione del clock
		// Power-up dell'ADC
		 LPC_SYSCON->PDRUNCFG &= ~(1<<4);
		 // Setta il reset per l'ADC
		  LPC_SYSCON->PRESETCTRL0 &= ~(1<<24);
		 // Togli reset all'ADC
		  LPC_SYSCON->PRESETCTRL0 |= (1<<24);
		 // Abilita il clock delle periferiche utilizzate
		  LPC_SYSCON->SYSAHBCLKCTRL[0] |= (1<<24) // ADC
		                               | (1<<7) // SWM
		                               | (1<<18); // IOCON
		 // Divisore di clock per l'ADC pari a 1 (abilita clock perché diverso da zero)
		  LPC_SYSCON->ADCCLKDIV = 1;
		 // Usa FRO-clk come sorgente di clock dell'ADC
		  LPC_SYSCON->ADCCLKSEL = 0;
	     //fine

		  // questa funzione permette di conoscere i valori di frequenza di clock
		  // del sistema
		   SystemCoreClockUpdate();
		  // Impostare CLKDIV a 500 kHz
		   int clk_div = (fro_clk / 500000) - 1;    //free running oscillator

		   // Inizia auto-calibrazione
		   // CALMODE = 1;
		   // Low Power Mode = 0;
		   // CLKDIV = per avere 500 kHz e il valore clk_div viene caricato nel registro specifico
		    LPC_ADC->CTRL = ((1<<30) | (0<<10) | (clk_div<<0));
		   // Il bit 30 viene automaticamente azzerato non appena
		   // finisce la auto-calibrazione
		    while (LPC_ADC->CTRL & (1<<30));//Aspetta fine cal.

		    // Il trimmer è collegato a ADC0 = PIO0.7
		     LPC_SWM->PINENABLE0 &= ~(1<<14);
		    // Disattiva il pull-up e pull-down per PIO0.7
		     LPC_IOCON->PIO0_7 &= ~(3<<3);

		     LPC_ADC->TRM &= ~(1<<5); // '0' for high voltage.  Con il registro TRM si configura l’ADC in base proprio alla tensione di alimentazione che, nel nostro caso, risulta pari al massimo di 3.3 V

		     // Definiamo una frequenza di 12 MHz per l’ADC
		     #define ADC_CLK_FREQ 1200000
		     clk_div = (fro_clk / (25 * ADC_CLK_FREQ)) - 1;  //servono 25 periodi di clock per una conversione, il tempo di conversione sarà pari a 2 µs circa
		     // CALMODE = 0;
		     //Low Power Mode = 0;
		     //CLKDIV = per avere il valore ADC_CLK_FREQ
		      LPC_ADC->CTRL = ((0<<30) | (0<<10) | (clk_div<<0));

	          //selezione del trigger di tipo software per l'avvio di una conversione
		      // Scegli ADC0 in CHANNELS
		       LPC_ADC->SEQA_CTRL |= 0x1<<0;
		      // TRIGPOL = 1 e SEQ_ENA = 1
		       LPC_ADC->SEQA_CTRL |= (1<<18) | (1<<31);
		      // disabilita IRQ da ADC, non sono previsti interrupt
		      LPC_ADC->INTEN = 0;

	LPC_SYSCON->SYSAHBCLKCTRL[0]  |= 1<<20;

	InitMRT();    //inizializza MRT: ch0 per funzioni delay, ch1 per IRQ ogni secondo
	InitLCD();     //inizializza LCD

	while(1) {

		//INIZIO STATO 0 PER VISUALIZZARE IL TEMPO E LA TEMPERATURA IN GRADI
		if(stato == 0 ){
			            LPC_MRT->Channel[1].INTVAL = 30000000/100;  //avvio CH1 di MRT con interrupt ogni cs
			            WriteString((uint8_t*)"", 0);
						Write_2digitsval(hh); WriteChar(':');
						Write_2digitsval(mm); WriteChar(':');
						Write_2digitsval(ss);
						GoToLine(2);       //passa alla seconda riga dell LCD

						    //si avvia una conversione, settiamo il bit 16 della sequenza A al canale 0
							LPC_ADC->SEQA_CTRL |= 1<<26;

							// Attesa di fine conversione che è segnata con il set del bit 31 del registro DAT del canale specifico
							while( (LPC_ADC->DAT[0] & (1<<31) ) == 0);

							//il valore di uscita si trova tra i bit 4 e 15, quindi shiftiamo
							//di 4 verso destra nel registro e filtriamo solo i 12 bit
							int dummyVal = 0;
							dummyVal = (LPC_ADC->DAT[0] >> 4) & 0xFFF;
							//per eliminare il rumore prendiamo la media di 500 campioni

							for(uint32_t i=0;i<500;i++){                              //dava un errore cioè invece di segnare 4095 diceva 5015 quindi meglio usare 500 campioni
							LPC_ADC->SEQA_CTRL |= 1<<26; // AVVIA CONVERSIONE
							while( (LPC_ADC->DAT[0] & (1<<31) ) == 0);
							// legge il valore dell'ADC e lo accumula
							dummyVal += (LPC_ADC->DAT[0] >> 4) & 0xFFF;
						    }

				dummyVal /= 500;
				x = 1000*dummyVal/4095;   //si fa variare x tra 0 e 1000 quando la temperatura varia tra 0 e 100 gradi


				if (x >= 400 ){
					C = x-400;           //dato che x è maggiore di 400 i gradi sono positivi, per avere valore tra 0 e 60

					Write_ndigitsval(C/10,  3);WriteChar('.');  // scrive le cifre prima della virgola
					Write_ndigitsval0(C%10,  1);                //scrive le cifre dopo la virgola
					WriteWord((uint8_t*)" C");


				} else {
			        C = 400 - x;        //per tutti i valori minori di 400, per avere valori tra -40 e 0

			        WriteWord((uint8_t*)"-");
			        Write_ndigitsval(C/10,  3);WriteChar('.');
			        Write_ndigitsval0(C%10,  1);
			        WriteWord((uint8_t*)" C");
			    		}


						if( ( (LPC_GPIO_PORT->PIN[0] & (1<<4) ) == 0 ) && (USER_pressed == 0) ){ 	//tasto USER premuto
						;

						}

						if( ( (LPC_GPIO_PORT->PIN[0] & (1<<4) ) ) && (USER_pressed) ){             //tasto USER rilasciato
						USER_pressed = 0;
						}


						// Press ISP
						if( ( (LPC_GPIO_PORT->PIN[0] & (1<<12) ) == 0 ) && (ISP_pressed == 0) ){ //tasto ISP premuto
							ISP_pressed = 1;
							//pulizia LCD
							PutCommand(DISPLAY_CLEAR); delayMs(5);
							PutCommand(RETURN_HOME);   delayMs(5);
						    stato = SET_OO;
						}

						if( ( (LPC_GPIO_PORT->PIN[0] & (1<<12) ) ) && (ISP_pressed) ){ //tasto ISP rilasciato
							ISP_pressed = 0;
						}
		} // FINE STATO 0

        //INIZIO STATO SET_00 PER SETTARE IL TIMER
		if (stato == SET_OO ){
			 LPC_MRT->Channel[1].INTVAL = 0 |(1<<31);  //ferma il timer

			//STATO SET_00 RIGA 1
			WriteString((uint8_t*)"", 0);
			Write_2digitsval(hh); WriteChar(':');
			Write_2digitsval(mm);


			  if( ( (LPC_GPIO_PORT->PIN[0] & (1<<4) ) == 0 ) && (USER_pressed == 0) ){ 		//tasto USER premuto
			     USER_pressed = 1;
			                   switch(pos)
			   					{
			   					case 0:
			   					hh++;
			   					if(hh==24){
			   						hh=0;

			   					}
			   				    break;

			   					case 1:
			   					if(mm==59){
			   					mm=0;
			   					hh++;
			   					}else{
			   					mm++;
			   				}
			   					break;
			   					}

				}

				if( ( (LPC_GPIO_PORT->PIN[0] & (1<<4) ) ) && (USER_pressed) ){ //tasto USER rilasciato
					USER_pressed = 0;
					}


					// Press ISP
				if( ( (LPC_GPIO_PORT->PIN[0] & (1<<12) ) == 0 ) && (ISP_pressed == 0) ){ //tasto ISP premuto
					ISP_pressed = 1;
					pos++;
								      if (pos==2){
					                  pos=0;
								      //pulizia LCD
									  PutCommand(DISPLAY_CLEAR); delayMs(5);
									  PutCommand(RETURN_HOME);   delayMs(5);
									  stato = 0;
										}

				}

			   	if( ( (LPC_GPIO_PORT->PIN[0] & (1<<12) ) ) && (ISP_pressed) ){ //tasto ISP rilasciato
					ISP_pressed = 0;
				}

		}//FINE STATO SET_OO

	}// fine while
}  // fine main


