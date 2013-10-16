/*
 *REVO Sensing - WatchDog
 *written by: Chris Lee (@sihrc)
 *9/27/2013

 *ATmega168
 *Watchdog

*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define F_CPU (1000000 UL) //Setting the clock speed in delay.h
#define maxDataLength 8	

int main(void){
	cli();     //Disable Global Interrupts

	//Set up the WatchDog Death Timer
	TCCR1B |= 1<<CS11 | 1<<CS10; //Divide by 64
	OCR1A = 15624 * 4; 			 //Watchdog death timer every 2 seconds
	TCCR1B |= 1<<WGM12; 		 //Sets death timer in CTC mode
	TIMSK1 |= 1<<OCIE1A;		 //Enables timer compare interrupt

	//Set up Pins

	//Kill Signal Pin
	DDRD |= (1<<PD0);				 //Setting PortD Pin0 as output
	PORTD &= ~(1<<PD0);			     //Setting Pin0 to low

	sei();   //Enables Global Interrupts

	while (1){} //Loop Forever
}

void handleCANmsg(int destID, int msgID, char msg[], int msgLen){
	//BMS Node
	msg[]
	//Motor Controller Node
}

//Interrupt Service Routines
ISR(TIMER1_COMPA_vect)  //Die Die Die Timer ISR
{    
	//Send kill-signal
	PORTD |= (1<<PD0);  //Setting Pin0 to high
} 

ISR(){
	//Reset Watchdog death timer - Everything is okay.
}