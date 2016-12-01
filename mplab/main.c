#include <p18f2220.h>

#pragma config OSC = INTIO2
#pragma config WDT = OFF
#pragma config BOR = OFF
#pragma config PWRT = OFF
#pragma config LVP = OFF

unsigned int bres = 0;
unsigned char hour = 0;
unsigned char min = 0;
unsigned char sec = 0;

void update_time(void);
void refresh_clock(void);
void blink(void);
void check_battery(void);

#pragma code INTERRUPT_VECTOR = 0x8
void ISR (void) {

	// See www.romanblack.com/one_sec.htm for details
	if (INTCONbits.TMR0IF) {
		bres += 16; // add (256/16) ticks to bresenham total
		
		if (bres >= 62500) { // if reached (1000000/16) 1 second!
			bres -= 62500; // subtract 1 second, retain error
			sec += 1;
			update_time();
			check_battery();
		}

		INTCONbits.TMR0IF = 0; // Reset flag
		INTCONbits.GIE = 1; // Re-enable all interrupt sources (not sure if required)
		INTCONbits.PEIE = 1; // Enable peripheral interrupt sources (not sure if required)
	}

	if (PIR1bits.ADIF) {
		// Check if battery voltage under 1V/cell
		// If so, blink RA7
	}
}

#pragma code

void update_time(void) {
	if (sec == 60) {
		sec = 0;
		min += 1;
		if (min == 60) {
			min = 0;
			hour += 1;
			if (hour == 24) {
				hour = 0;
			}
		}
	}
}

void check_battery(void) {
	ADCON0bits.GO = 1; // Start A/D conversion
}

void main (void) {
	// Oscillator configuration
	OSCCONbits.IDLEN = 1; // Idle mode enabled; CPU core is not clocked in power managed modes
	OSCCONbits.IRCF2 = 1; // 4 MHz
	OSCCONbits.IRCF1 = 1;
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
	PIE1bits.ADIE = 1; // Enable A/D conversion interrupt

	// Timer0
	T0CONbits.TMR0ON = 1; // Enable TMR0
	T0CONbits.T08BIT = 1; // 8-bit timer (256 ticks)
	T0CONbits.T0CS = 0; // Internal instruction cycle clock
	T0CONbits.PSA = 1; // Bypass prescaler

	// A/D converter
	ADCON0bits.CHS3 = 0; // AN0 (battery voltage)
	ADCON0bits.CHS2 = 0;
	ADCON0bits.CHS1 = 0;
	ADCON0bits.CHS0 = 0;
	ADCON0bits.ADON = 1; // A/D converter module is enabled
	ADCON1bits.VCFG1 = 0; // VREFL = AVss
	ADCON1bits.VCFG0 = 0; // VREFH = AVdd
	ADCON1bits.PCFG3 = 1; // AN0 = analog, others = digital
	ADCON1bits.PCFG2 = 1;
	ADCON1bits.PCFG1 = 1;
	ADCON1bits.PCFG0 = 0;
	ADCON2bits.ADFM = 1; // A/D result right justified (don't know the point..)
	ADCON2bits.ACQT2 = 1; // 20 Tad (acquisition time)
	ADCON2bits.ACQT1 = 1;
	ADCON2bits.ACQT0 = 1;
	ADCON2bits.ADCS2 = 0; // Fosc/8
	ADCON2bits.ADCS1 = 0;
	ADCON2bits.ADCS0 = 1;

	// Ports A,B,C
	TRISA = 0b00000001;
	PORTA = 0x00;
	TRISB = 0b11110011;
	PORTB = 0x00;
	TRISC = 0b00000000;
	PORTC = 0x00;
}
