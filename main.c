/*
 * projekt Mikro.c
 *
 * Created: 25.01.2022 17:30:17
 * Author : barto
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

void t1_init(){
		TCCR1A |=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);		// kierunek na pb1 ,pb2, non inverted
		TCCR1B |=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10); //prescaler ckl/64, Fast PWM
		ICR1=4999;	// fPWM=50 hz ( fcpu/(64*50)-1
											
}


int Czas_powrotu_imp(void)
{
	int obecnie=0;
	int narast_czas=0;

	_delay_us(2);
	PORTD |= (1 << PD2);	// stan wysoki na triger
	_delay_us(10);			// sygna³ minimum 10ms wed³ug katalogu (8 impulsow)	
	PORTD &= ~(1 << PD2);	//stan niski

	
	while(obecnie==0)			//petla odczytu echo
	{
		while(PINB&(1<<PINB0)) //petla trwa dopóki stan wyoki pinu echo 
		{
			narast_czas++;
			obecnie = 1;
		}
	}

	return narast_czas;
}

int main(void)
{
	/* Replace with your application code*/
	uint16_t zmiennab =300;
	uint16_t zmiennaa = 442;
	//uint16_t zeropredk = 375; //brak napiecia
	t1_init();
	DDRB |= (1<<PB1) | (1<<PB2);
	DDRD|= (1<<PD2);
	int czas;
	int odl;
	while (1)
	{
		Czas_powrotu_imp();
		czas = Czas_powrotu_imp();
		odl = czas*0.00862;
		
		if(odl<20)
		{	
			OCR1A=zmiennab;
			OCR1B=zmiennab;
			_delay_ms(1000);
			
		}
		else
		{	
			OCR1A=zmiennaa;
			OCR1B=zmiennab;
			_delay_ms(10);
		}
		
	}
	
}

