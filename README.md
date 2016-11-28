# Binary Clock

This is a simple binary clock project using a PIC18F252, pretty much for:

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
