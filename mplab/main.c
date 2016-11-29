#include <p18f2220.h>

#pragma config OSC = INTIO2
#pragma config WDT = OFF

#pragma code INTERRUPT_VECTOR = 0x8
void ISR (void)
{
	// See http://www.romanblack.com/one_sec.htm for method of making a 1s period
}

#pragma code

void main (void)
{
	// Oscillator configuration
	OSCCONbits.IDLEN = 1; // Idle mode enabled; CPU core is not clocked in power managed modes
	OSCCONbits.IRCF2 = 0; // 31 kHz (INTRC source drives clock directly, check if 31.25 kHz) 
	OSCCONbits.IRCF1 = 0;
	OSCCONbits.IRCF0 = 0;
	OSCCONbits.SCS1 = 1; // Internal oscillator block (RC modes)

	// Interrupts
	RCONbits.IPEN = 0; // Disable interrupt priority
	INTCONbits.GIE = 1; // Enable all interrupt sources
	INTCONbits.PEIE = 1; // Enable peripheral interrupt sources
	INTCONbits.TMR0IE = 1; // Enable TMR0 interrupt
	INTCONbits.INT0IE = 1; // Enable INT0 interrupt
	INTCON2bits.INTEDG0 = 1; // INT0 on rising edge
	INTCON3bits.INT1IE = 0; // Enable INT1 interrupt
	INTCON2bits.INTEDG1 = 1; // INT1 on rising edge
	PIE1bits.ADIE = 1;

	// Timer0
	T0CONbits.TMR0ON = 1; // Enable TMR0
	T0CONbits.T08BIT = 0; // 16-bit timer (65536)
	T0CONbits.T0CS = 0; // Internal instruction cycle clock
	T0CONbits.PSA = ; // 
	T0CONbits. = ; // 
	T0CONbits. = ; // 
	T0CONbits. = ; // 

	// A/D converter
	ADCON0
	ADCON1
	ADCON2

	// Ports A,B,C
	TRISA = 0b00000001;
	PORTA = 0x00;
	TRISB = 0b11110011;
	PORTB = 0x00;
	TRISC = 0b00000000;
	PORTC = 0x00;
}
