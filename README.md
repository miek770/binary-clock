# Binary Clock

This is a simple binary clock project using a PIC18F2220, pretty much for:

1. The fun of it;
2. Getting more experience with Eagle PCB design;
3. Doing a first PIC C project (I've only programmed PICs in assembly before).

## To Do List

The sync_led() function didn't function properly because I set bits to a byte value without shifting. I've fixed it in the code but haven't tested it yet.

- Consider using delays.h instead of timer1 for the low-battery blink. The use of the timer seems unecessary and it would be a good excuse to explore the available functions;
- Consider importing timers.h to setup timer0, instead of manually setting each register;
- Consider using the idle power management mode when nothing is happening to reduce battery consumption. First check if it would interfere with timer0;
- Test the PCB;
- Finish soldering the components;
- Assemble the clock;
- Replace the PIC18F252 with a PIC18F2220 in the schematic.

## Appearance

### Front

I think I'll use an old PC card for the front. I have a nice looking one with few components on it and I could easily drill holes for the LEDs. It would definitely give the clock its most deserved geeky look.

### Display

The clock uses 5 LEDs (bits) for the hours (24h format), 6 for the minutes and 6 for the seconds. Showing fractions of seconds seemed excessive.

## Supply

It can be fed either or by both:

1. 3x AA batteries;
2. A micro-USB port.

### Low Charge Detection

Both supplies are paralleled with diodes, and the PIC monitors the batteries' voltage so that an alarm (blinking LED) is raised when the batteries are low. For 1.5V AA batteries, this would be pretty much when they get under 1V. AN0 is used on the PIC to measure the battery voltage.

Hum thinking about it makes me realize that the voltage reference is Vdd. So if we ignore the voltage drop through the diode, which is gonna be constant, the voltage drop at the battery (with no USB supply) is going to drop proportionally with Vdd which means the AN0 measurement won't pick it up. Unless I use the voltage drop across the diode as a reference. At 4.5V at the batteries, with a 0.7V drop through the diode I'll get 3.8V at Vdd. When the batteries get to 3V (1V per cell), Vdd will equal 2.3V.

So Vbatt/Vdd = 4.5V/3.8V (1.184) at full charge, and 3V/2.3V (1.304) at low charge. Using a 50/50 voltage divider between Vbatt and AN0, the ratios would be 0.592 (full) and 0.652 (low). With a 10-bit A/D converter I have a 1024 resolution, which would be able to measure this. Basically the reading is going to go from 0/1023 to 1023/1023. A 0.592 ratio (full) would be read as 606 and a 0.652 ratio (low) as 667. So I could then consider any reading over 666 (0b1010011010) as a low battery charge.

This will need to be adjusted to account for the real diode voltage drop as well as the voltage divider ratio, but it should do the trick. For testing purposes I could use the time diodes to show the A/D conversion output. I have up to 17 bits (5+6+6 diodes).

## Timing

I switched from the PIC18F252 to the PIC18F2220 because the former doesn't have an internal oscillator. I assumed it did.

I will be using TMR0 to count seconds, using a bresenham algorithm as explained and implemented here: [http://www.romanblack.com/one_sec.htm](http://www.romanblack.com/one_sec.htm)

Basically (this is all from his website above):

    This next one is similar to the 1 second generators above. However it is optimised for speed and code space, because it uses a 16bit unsigned int variable for bres instead of a 32bit unsigned long. We do this by using values for the int period and the 1second period that are in the same ratio to each other, but smaller! We divide both values by 16;

    Optimised C code for a 1 second period with a 1MHz timer (4MHz xtal);

        // uses 1 variable; unsigned int bres
        // gets here every TMR0 int (every 256 ticks)

        bres += 16; // add (256/16) ticks to bresenham total

        if(bres >= 62500)   // if reached (1000000/16) 1 second!
        {
            bres -= 62500;  // subtract 1 second, retain error
            do_1sec_event();    // update clock, etc
        }

This is really quite neat!
