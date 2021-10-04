/*
 * timer0.c
 *
 * Author: Peter Sutton
 *
 * We setup timer0 to generate an interrupt every 1ms
 * We update a global clock tick variable - whose value
 * can be retrieved using the get_clock_ticks() function.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer0.h"
#include "score.h"
#include "pixel_colour.h"
#include "game.h"



#include <avr/pgmspace.h>
#include <stdio.h>

volatile uint16_t displayScore = 0;

/* Seven segment display digit being displayed.
** 0 = right digit; 1 = left digit.
*/
volatile uint8_t seven_seg_cc = 0;

/* Seven segment display segment values for 0 to 9 */
uint8_t seven_seg_data[10] = {63,6,91,79,102,109,125,7,127,111};

/* Our internal clock tick count - incremented every 
 * millisecond. Will overflow every ~49 days. */
static volatile uint32_t clockTicks;
volatile int xk;

/* Set up timer 0 to generate an interrupt every 1ms. 
 * We will divide the clock by 64 and count up to 124.
 * We will therefore get an interrupt every 64 x 125
 * clock cycles, i.e. every 1 milliseconds with an 8MHz
 * clock. 
 * The counter will be reset to 0 when it reaches it's
 * output compare value.
 */
void init_timer0(void) {
	/* Reset clock tick count. L indicates a long (32 bit) 
	 * constant. 
	 */

	clockTicks = 0L;
	
	/* Clear the timer */
	TCNT0 = 0;

	/* Set the output compare value to be 124 */
	OCR0A = 124;
	
	/* Set the timer to clear on compare match (CTC mode)
	 * and to divide the clock by 64. This starts the timer
	 * running.
	 */
	TCCR0A = (1<<WGM01);
	TCCR0B = (1<<CS01)|(1<<CS00);

	/* Enable an interrupt on output compare match. 
	 * Note that interrupts have to be enabled globally
	 * before the interrupts will fire.
	 */
	TIMSK0 |= (1<<OCIE0A);
	
	/* Make sure the interrupt flag is cleared by writing a 
	 * 1 to it.
	 */
	TIFR0 &= (1<<OCF0A);
}

uint32_t get_current_time(void) {
	uint32_t returnValue;

	/* Disable interrupts so we can be sure that the interrupt
	 * doesn't fire when we've copied just a couple of bytes
	 * of the value. Interrupts are re-enabled if they were
	 * enabled at the start.
	 */

	

	uint8_t interruptsOn = bit_is_set(SREG, SREG_I);
	cli();
	returnValue = clockTicks;
	if(interruptsOn) {
		sei();
	}
	return returnValue;
}
ISR(TIMER0_COMPA_vect) {
	/* Increment our clock tick count */
	clockTicks++;
	 //SET OUR VARIABLES HERE
	 displayScore = get_score();
	 //seven_seg_cc = 1 ^ seven_seg_cc;
	 //int slow = 50;

	 //THE SCORE CHANGES ONCE WE HIT THE MAXIMUM OF 99
	if(displayScore > 100) {
		displayScore = 0;
	}
	
	
	/* Change which digit will be displayed. If last time was
	** left, now display right. If last time was right, now 
	** display left.
	*/
	PORTA = 0;
	//WE WANT TO SLOW DOWN TO AVOID GHOSTING 
	if( xk == 0){
		if(displayScore < 10) {
		/* Display a digit */
			/* Display rightmost digit - tenths of seconds */
			PORTA = seven_seg_data[displayScore%10];
		}
		else{
			if(displayScore >= 10){

			/* Display leftmost digit  */
				//PORTA = 0;
				seven_seg_cc = ~PORTC & 0x01;
				//printf_P(PSTR("%d\n"),seven_seg_cc);
				if(seven_seg_cc == 0){
					//WE WANT TO CHECK THE FIRST DIGIT
					PORTA = seven_seg_data[(displayScore)%10];

				}else {
					//CHECK THE SECOND DIGIT
					PORTA = seven_seg_data[(displayScore/10) % 10];

				}
				PORTC ^= (1 << 0);	
			}else{
				PORTA = 0;
				}
		}
	}

	
	
	//slow = slow +10;
	//PORTC |= (1<<1)|(1<<2)|(1<<3)|(1<<4);

}
void lifeLost(int led){
	//HERE WE GET THE COUNTER AND CHECK WHICH LED TO DELETE
	// 1 means end game 0 is keep going 
    //printf_P(PSTR("%d\n"),x);

	switch(led){
		case 1:
			PORTC &= ~(1<<1);
			xk = 0;
			break;
		case 2:
			PORTC &= ~(1<<4);
			xk = 0;
			break;
		case 3:
			PORTC &= ~(1<<2);
			xk = 0;
			break;
		case 4:
			PORTC &= ~(1<<3);
			xk = 1;
			returnGameState();
			break;
		default: xk = 0;
	}
	    //printf_P(PSTR("%d\n"),x);

}
int returnGameState(void){
	if(xk == 1){
	 resetX(0);
	 //seven_seg_cc = 0;
	 return 1 ;
	}
	return xk;
}
void resetX(int newx){
	xk = newx;
}
