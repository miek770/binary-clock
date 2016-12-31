#include <p18f2220.h>

// Configuration
#pragma config OSC = INTIO2
#pragma config WDT = OFF
#pragma config BOR = OFF
#pragma config PWRT = OFF
#pragma config LVP = OFF
#pragma config MCLRE = OFF

// Prototypes - Functions
void update_time(void);
void refresh_clock(void);
void blink(void);
void sync_leds(void);
void isr(void);

// Prototypes - Configurations
void conf_tmr0(void);
void conf_adc(void);
void conf_int(void);
void conf_osc(void);
void conf_tmr1(void);
void conf_ports(void);
unsigned char get_bit(unsigned char byte, unsigned char pos);

/* Initialized variables declarations

Using "access" and "near" ensures that the data be saved in the accessram
region of the data memory (quicker access).

Using "volatile" is recommended by the C18 User's Guide when the variable
is to be used by the ISR as well as other functions. Basically it ensures
that the variable isn't cached during execution so changes in either the
normal program or interrupt routines are reflected in each other. */

#pragma idata access accessram
volatile near unsigned int bres = 0;
volatile near unsigned char hour = 0b11;
volatile near unsigned char min = 0b11;
volatile near unsigned char sec = 0b11;

// High-priority interrupt vector
#pragma code high_vector = 0x08
void interrupt_at_high_vector(void) {
	_asm GOTO isr _endasm
}

#pragma code

// Interrupt subroutine (priorities are disabled)

#pragma interrupt isr
void isr (void) {
	
	// See www.romanblack.com/one_sec.htm for details
	// Values below work for 4MHz clock (Fosc/4 = 1MHz)
	if (INTCONbits.TMR0IF) {
		bres += 16; // add (256/16) ticks to bresenham total
		//LATCbits.LATC7 ^= 1; // For testing

		if (bres >= 62500) { // if reached (1000000/16) 1 second!
			bres -= 62500; // subtract 1 second, retain error
			sec += 1;
			update_time();
			sync_leds();
			ADCON0bits.GO = 1; // Check battery level
		}

		INTCONbits.TMR0IF = 0; // Reset flag
	}

	// A/D conversion is complete
	if (PIR1bits.ADIF) {
		// Check if battery voltage under 1V/cell
		if (ADRESH >= 0b10 && ADRESL >= 0b10011010) {
			blink(); // Initiates RA7 blink
		}
		PIR1bits.ADIF = 0; // Reset flag
	}

	// Time to blink!
	if (PIR1bits.TMR1IF) {
		LATAbits.LATA7 ^= 1; // Toggle RA7
		T1CONbits.TMR1ON = 0; // Turn off TMR1
		PIR1bits.TMR1IF = 0; // Reset flag
	}

	// Hour adjustment!
	if (INTCONbits.INT0IF) {
		hour += 1;
		update_time();
		sync_leds();
		INTCONbits.INT0IF = 0; // Reset flag
	}

	// Min adjustment!
	if (INTCON3bits.INT1IF) {
		min += 1;
		update_time();
		sync_leds();
		INTCON3bits.INT1IF = 0; // Reset flag
	}
}

// Main code
#pragma code
void main (void) {
	conf_osc(); // Oscillator configuration
	conf_int(); // Interrupts configuration
	conf_tmr0(); // Timer0 - To count seconds
	conf_tmr1(); // Timer1 - To blink RA7 (low battery)
	conf_adc(); // A/D converter
	conf_ports(); // Ports A,B,C

	sync_leds();

	while (1); // Loop indefinitely
}

// Other functions
unsigned char get_bit(unsigned char byte, unsigned char pos) {
	return (byte >> pos) & 1;
}

void blink(void) {
	LATAbits.LATA7 ^= 1; // Toggle RA7
	T1CONbits.TMR1ON = 1; // Turn on TMR1
}

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

void sync_leds(void) {
	LATAbits.LATA3 = get_bit(hour, 0);
	LATAbits.LATA4 = get_bit(hour, 1);
	LATAbits.LATA5 = get_bit(hour, 2);
	LATAbits.LATA6 = get_bit(hour, 3);
	LATAbits.LATA7 = get_bit(hour, 4);

	LATAbits.LATA1 = get_bit(min, 0);
	LATAbits.LATA2 = get_bit(min, 1);
	LATBbits.LATB2 = get_bit(min, 2);
	LATBbits.LATB3 = get_bit(min, 3);
	LATCbits.LATC0 = get_bit(min, 4);
	LATCbits.LATC1 = get_bit(min, 5);

	LATCbits.LATC2 = get_bit(sec, 0);
	LATCbits.LATC3 = get_bit(sec, 1);
	LATCbits.LATC4 = get_bit(sec, 2);
	LATCbits.LATC5 = get_bit(sec, 3);
	LATCbits.LATC6 = get_bit(sec, 4);
	LATCbits.LATC7 = get_bit(sec, 5);
}

// Configurations
void conf_ports(void) {
	TRISA = 0b00000001;
	LATA = 0x00;
	TRISB = 0b11110011;
	LATB = 0x00;
	TRISC = 0b00000000;
	LATC = 0x00;
}

void conf_osc(void) {
	OSCCONbits.IDLEN = 1; // Idle mode enabled; CPU core is not clocked in power managed modes
	OSCCONbits.IRCF2 = 1; // 4 MHz
	OSCCONbits.IRCF1 = 1;
	OSCCONbits.IRCF0 = 0;
	OSCCONbits.SCS1 = 1; // Internal oscillator block (RC modes)
}

void conf_int(void) {
	RCONbits.IPEN = 0; // Disable interrupt priority
	INTCONbits.GIE = 1; // Enable all interrupt sources
	INTCONbits.PEIE = 1; // Enable peripheral interrupt sources
	INTCONbits.TMR0IE = 1; // Enable TMR0 interrupt
	INTCONbits.INT0IE = 1; // Enable INT0 interrupt
	INTCON2bits.INTEDG0 = 1; // INT0 on rising edge
	INTCON3bits.INT1IE = 0; // Enable INT1 interrupt
	INTCON2bits.INTEDG1 = 1; // INT1 on rising edge
	PIE1bits.ADIE = 1; // Enable A/D conversion interrupt
}

void conf_tmr0(void) {
	T0CONbits.TMR0ON = 1; // Enable TMR0
	T0CONbits.T08BIT = 1; // 8-bit timer (256 ticks)
	T0CONbits.T0CS = 0; // Internal instruction cycle clock
	T0CONbits.PSA = 1; // Bypass prescaler
}

void conf_tmr1(void) {
	T1CONbits.T1CKPS1 = 0; // 1:2 prescale value
	T1CONbits.T1CKPS0 = 1;
	T1CONbits.TMR1CS = 0; // Internal clock (Fosc/4)
	T1CONbits.TMR1ON = 0; // Disable TMR1
}

void conf_adc(void) {
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
}
