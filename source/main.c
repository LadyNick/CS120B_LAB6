/*	Author: Nicole Navarro
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #6  Exercise #3
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

enum count_states{init, inc_p, inc_r, dec_p, dec_r, reset_p, reset_r, release} count_state;

//unsigned char not_pina;
unsigned char A0;
unsigned char A1;

void Tick_Count(){
	switch (count_state){
		case init:
			PORTB = 0X07;
			if( A0 && !A1 ){
				count_state = inc_p;
			}
			else if( !A0 && A1){
				count_state = dec_p;
			}
			else if( A0 && A1 ){
				count_state = reset_p;
			}
			else{
				count_state = init;
			}
			break;
		case inc_p:
			if((PORTB < 0X09)){
				PORTB = PORTB + 1;
			}
			if( A0 && A1 ){
				count_state = reset_p;
			}
			else{
				count_state = inc_r;
			}
			break;
		case inc_r: 
			if( A0 && A1 ){
				count_state = reset_p;
			}
			else if( A0 && !A1 ){
				count_state = inc_r;
			}
			else if( !A0 && !A1 ){
				count_state = release;
			}
			break;
		case dec_p:
			if((PORTB > 0)){
				PORTB = PORTB - 1;
			}
			if( A0 && A1 ) {
				count_state = reset_p;
			}
			else{
				count_state = dec_r;
			}
			break;
		case dec_r:
			if(A0 && A1 ){
				count_state = reset_p;
			}
			else if( A1 && !A0){
				count_state = dec_r;
			}
			else if( !A1 && !A0){
				count_state = release;
			}
		case reset_p:
			PORTB = 0x00;
			count_state = reset_r;
			break;
		case reset_r:
			if( !A0 && !A1 ){
				count_state = release;
			}
			else{
				count_state = reset_r;
			}
			break;
		case release:
			if( A0 && !A1 ){
                                count_state = inc_p;
                        }
                        else if( !A0  && A1){
                                count_state = dec_p;
                        }
                        else if( A0 && A1 ){
                                count_state = reset_p;
                        }
			else{
				count_state = release;	
			}
                        break;
		default:
			count_state = init;
			break;
	}
}

int main(void) {
    DDRB = 0XFF;   PORTB = 0x00;
    DDRA = 0X00;   PINA = 0XFF; 
    TimerSet(100);
    TimerOn();
	
    	count_state = init;
	PORTC = 0x07;	

    while (1) {
	A0 = ~PINA & 0X01;
	A1 = ~PINA & 0X02
	Tick_Count();
	while(!TimerFlag); 
	TimerFlag = 0;
    }
    return 1;
}
