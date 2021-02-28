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
unsigned char d = 0x00;
unsigned char u = 0x00;
unsigned char left = 0x00;
unsigned char right = 0x00;
unsigned char flag = 1;
unsigned char flag1 = 1;
unsigned char flag2 = 2;
unsigned char flag3 = 2;
unsigned char arr[4] = {0x20,0x10,0x08,0x04};
unsigned char arr1[4] = {0xF1, 0xF5, 0xF5, 0xF1};

enum Demo_States{start,l,r,m1,m2}state;
enum move_states{init,checking,but,but1,but2,but3,wait1,wait2,wait3,wait4}move;
void Demo_Tick(){
switch(state){
case start:
		state = l;	
break;
		case l:
		pattern = arr[0];
		row = arr1[0];
		state = m1;
		break;
	
		case m1:
		pattern = arr[1];
		row = arr1[1];
		state = m2;
		break;
case m2:	
		pattern = arr[2];
		row = arr1[2];
		state = r;
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

void buttons(){
	d = ~PINA & 0x01;
	left = ~PINA & 0x02;
	right = ~PINA & 0x04;
	u = ~PINA & 0x08;
switch(move){
	case init:
		move = checking;
		break;
	case checking:
		if (d){
		move = wait1;
		}
		else if (left){
		move = wait2;
		}
		else if (right){
		move = wait3;
		}
		else if (u){
		move = wait4;
		}
		else{
		move = checking;
		}
		break;
	case wait1:
		if (flag != 0){
		for (unsigned int i = 0; i < 4; i++){
		arr1[i] = arr1[i] << 1 | 0x01;
		}
		flag--;
		flag1++;
		}
		move = but;
		break;
	case wait2:
		if (flag2 != 0){
		for (unsigned int i = 0; i < 4; i++){
		
		arr[i] = arr[i] << 1;
		
		}
		flag2--;
		flag3++;
		}
		move = but1;
		break;
	case wait3:
		if(flag3 !=0 ){
		for (unsigned int i = 0; i < 4; i++){
		arr[i] = arr[i] >> 1;
		}
		flag3--;
		flag2++;
		}
		move = but2;
		break;
	case wait4:
		if (flag1 != 0){
		for (unsigned int i = 0; i < 4; i++){
		arr1[i] = arr1[i] >> 1;
		}
		flag1--;
		flag++;
		}
		move = but3;
		break;
	case but:
		if(d){
		move = but;
		}
		else if (!d){
		move = checking;
		}
		break;
	case but1:
		if (left){
		move = but1;
		}
		else if (!left){
		move  = checking;
		}
		break;
	case but2:
		if (right){
		move = but2;
		}
		else if (!right){
		move = checking;
		}
		break;
	case but3:
		if (u){
		move = but3;
		}
		else if (!u){
		move = checking;
		}
		break;


	default:
		move = init;
}

}


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
	buttons();
//	while(!TimerFlag);
//	TimerFlag = 0;

    }
    return 1;
}
