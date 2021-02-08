/*	Author: Nicole Navarro
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #6  Exercise #2
 *	Demo Link: https://youtu.be/gZMcOekCVO0
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

enum light_states{start, lights, pause, cont} light_state;

unsigned char tracker = 0x00;
unsigned char count = 0x00;
unsigned char A0;

void Tick(){
    switch(light_state){
	case start:
		light_state = lights;
		break;
	case lights:
		if(A0){
			light_state = pause;
		}
		else{
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
		}
		break;
	case pause:
		if(A0 && (count == 1)){
			count = 0;
			light_state = cont;
		}
		else if(A0){
			light_state = pause;
		}
		else if(!A0){ 
			count = 1;
			light_state = pause;
		}
   		break;
	case cont:
		if(A0 && (count == 1)){
			count = 0;
			light_state = pause;
		}
		else{
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
		if(!A0){
			count = 1;
		}
		light_state = cont;
		}
		break; 
	default:
		light_state = lights;
		break;
    }
}

int main(void) {
    DDRB = 0XFF;   PORTB = 0x00;
    DDRA = 0X00;   PINA = 0XFF; 
    TimerSet(300);
    TimerOn();
	
    	light_state = lights;	

    while (1) {
	A0 = ~PINA & 0X01;
	Tick();
	while(!TimerFlag); //Wait 1 sec
	TimerFlag = 0;
	//Better style would use synchSM with TickSM, this exmaple just illustrates the use of the ISR and flag
    }
    return 1;
}
