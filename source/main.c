/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #6  Exercise #1
 *	Demo Link: https://youtu.be/NqRPfkAsIRQ
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum light_states{start, lights} light_state;

unsigned char tracker = 0x00;

void Tick(){
    switch(light_state){
	    case start:
		light_state = lights;
		break;
	case lights:
		if(tracker == 0){
			PORTB = 0x01;
		}
		if(tracker == 1){
			PORTB = 0x02;
		}
		if(tracker == 2){
			PORTB = 0X04;
		}
		++tracker;
		if(tracker > 2){
			tracker = 0;
		}
		light_state = lights;
		break;
	default:
		break;
    }
}

int main(void) {
    DDRB = 0XFF;   PORTB = 0x00; 
    TimerSet(1000);
    TimerOn();

    while (1) {
	Tick();
	while(!TimerFlag); //Wait 1 sec
	TimerFlag = 0;
	//Better style would use synchSM with TickSM, this exmaple just illustrates the use of the ISR and flag
    }
    return 1;
}
