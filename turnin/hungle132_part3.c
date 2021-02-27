/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){

TCCR1B = 0x0B;

OCR1A = 125;

TIMSK1 = 0x02;

TCNT1 = 0;

_avr_timer_cntcurr = _avr_timer_M;

SREG |= 0x80;

}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

//--------------------------------------
// LED Matrix Demo SynchSM
// Period: 100 ms
//--------------------------------------
//
static unsigned char pattern = 0x80;
static unsigned char row = 0x00;
unsigned char but = 0x00;
unsigned char but1 = 0x00;
unsigned char flag = 0;

unsigned char arr[4] = {0x20,0x10,0x08,0x04};
unsigned char arr1[4] = {0xF1, 0xF5, 0xF5, 0xF1};

//enum Demo_States {start,button,wait,check,wait1,check1,check2,check3,check4,check5}state;
enum Demo_States{start,l,r,m1,m2}state;
void Demo_Tick() {


	for (unsigned int i = 0; i < 4; i++){
	pattern = arr[i];
	row = arr1[i];
	PORTC = pattern;
	PORTD = row;
	}
}
//	but = ~PINA & 0x01;
//	but1 = ~PINA & 0x02;

/*
	switch (state) {
		case start:
		state = m1;	
break;
		case l:
		pattern = arr[0];
		row = arr1[0];
		state = r;
		break;
	
		case m1:
		pattern = arr[1];
		row = arr1[1];
		state = m2;
		break;
case m2:	
		pattern = arr[2];
		row = arr1[2];
		state = l;
		break;
case r:
		pattern = arr[3];
		row = arr1[3];
		state = l;
	break;	

	default:
	state = start;
	}
	PORTC = pattern;	// Pattern to display
	PORTD = row;		// Row(s) displaying pattern
}
*/


	/*
	case button:
	if (but){
	state = wait;
	}
	else if (but1){
	state = wait1;
	}
	else{
	
	state = button;
	}
break;
	case wait:
	if (flag == 7){
	state = check;
	}
	else if (flag !=7){
	flag++;
	pattern = pattern >> 1;
	

	state = check;
	}
break;
	case wait1:
	if (flag == 0){
	state = check1;
	}
	else if (flag != 0){
	flag--;
	pattern = pattern << 1;
state = check1;
	}
break;
	case check:
	if (but){
	state = check;
	}
	else if (!but){
	state = button;
	}
break;

	case check1:
	if(but1){
	state = check1;
	}
	else if (!but1){
	state = button;
	}
	break;
	case check2:
break;
	case check3:
break;
	case check4:
break;
	case check5:
break;



	default:
state = start;
break;
	}
	// Actions
	switch (state) {
		default:
	break;
		case button:
	break;
		case wait:
	break;
		case wait1:
	break;
		case check:
//	row = 0xFD;
	break;
		case check1:
//	row = 0xFE;
	break;

	}
	PORTC = pattern;	// Pattern to display
	PORTD = row;		// Row(s) displaying pattern	
}
*/

int main(void) {
    /* Insert DDR and PORT initializations */
DDRA = 0x00; PORTA = 0xFF;
DDRB = 0x00; PORTB = 0xFF;
DDRC = 0xFF; PORTC = 0x00;
DDRD = 0xFF; PORTD = 0x00;

//TimerSet(25);
//TimerOn();
    /* Insert your solution below */
    while (1) {
	Demo_Tick();
//	while(!TimerFlag);
//	TimerFlag = 0;

    }
    return 1;
}
