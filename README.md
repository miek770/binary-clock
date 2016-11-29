# Binary Clock

This is a simple binary clock project using a PIC18F2220, pretty much for:

1. The fun of it;
2. Getting more experience with Eagle PCB design;
3. Doing a first PIC C project (I've only programmed PICs in assembly before).

## To Do List

The PCB order was placed on OSH Park on 2016-11-27. The next steps are:

- Choose the physical form and appearance;
- Test the PCB;
- Solder the components;
- Develop the program;
- Assemble the clock.

I'll wait until I have the assembled circuit before starting the code, otherwise I won't have anything to test on.

For the clock appearance, I hesitate between a wooden look or a more modern, metallic appearance. Clear plexiglass is also an option to consider. I like the idea of being able to see what's underneath, but it may make the clock itself harder to read.

## Display

The clock uses 5 LEDs (bits) for the hours (24h format), 6 for the minutes and 6 for the seconds. Showing fractions of seconds seemed excessive.

## Supply

It can be fed either or by both:

1. 4x AA batteries;
2. A micro-USB port.

Both supplies are paralleled with diodes, and the PIC monitors the batteries' voltage so that an alarm (blinking LED) is raised when the batteries are low. For 1.5V AA batteries, this would be pretty much when they get under 1V.

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
