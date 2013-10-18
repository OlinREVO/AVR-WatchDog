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

#define F_CPU (1000000 UL) 			 //Setting the clock speed in delay.h
#define maxDataLength 8

bool BMSDeath = false;
bool MCDeath = false;
int BMSShunt = 0;

int main(void){
	cli();     						 //Disable Global Interrupts

	//Set up the WatchDog Death Timer
	TCCR1B |= 1<<CS11 | 1<<CS10;	 //Divide by 64
	OCR1A = 15624 * 4; 			 	 //Watchdog death timer every 2 seconds
	TCCR1B |= 1<<WGM12; 		 	 //Sets death timer in CTC mode
	TIMSK1 |= 1<<OCIE1A;		 	 //Enables timer compare interrupt


	//Set up Pins
	//Kill Signal Pin
	DDRD |= (1<<PD0);				 //Setting PortD Pin0 as output
	PORTD &= ~(1<<PD0);			     //Setting Pin0 to low

	sei();   						 //Enables Global Interrupts

	while (1){} //Loop Forever
}

//Interrupt Service Routines
ISR(TIMER1_COMPA_vect)  //Die Die Die Timer ISR
{
	//Check BMS and MC status
	if (BMSDeath or MCDeath){
		//Send kill-signal
		PORTD |= (1<<PD0);  		 //Setting Pin0 to high	
		BMSDeath = false;
		MCDeath = false;
	}
	
	if (BMSShunt > 10){
		PORTD |= (1<<PD0);
		BMSShunt = 0;
	}
}

void handleCANmsg(uint8_t destID, uint8_t msgID, char msg[], uint8_t msgLen) {
    if (msgID == MSG_voltagelow)
    	BMSDeath = true;
    
    if (msgID == MSG_shunting)
    	BMSShunt++;
}

//From the CAN header

/*
#define NODE_watchdog       0;
#define NODE_bms            1;
#define NODE_speedometer    2;
#define NODE_halleffect     3;
#define NODE_sdlogger       4;

#define MSG_critical        0;
#define MSG_warning         1;
#define MSG_speed           2;
#define MSG_voltagelow		3;
#define MSG_shunting		4;
#define MSG_shutdown        5;
*/