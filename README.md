# Binary Clock

This is a simple binary clock project using a PIC18F252, pretty much for:

1. The fun of it;
2. Getting more experience with Eagle PCB design;
3. Doing a first PIC C project (I've only programmed PICs in assembly before).

## Display

The clock uses 5 LEDs (bits) for the hours (24h format), 6 for the minutes and 6 for the seconds. Showing fractions of seconds seemed excessive.

## Supply

It can be fed either or by both:

1. 4x AA batteries;
2. A micro-USB port.

Both supplies are paralleled with diodes, and the PIC monitors the batteries' voltage so that an alarm (blinking LED) is raised when the batteries are low. For 1.5V AA batteries, this would be pretty much when they get under 1V.

## To do

These corrections would make it version 0.1.1. They are all based on version 0.1.0:

- Do a quality-check of the schematic and all images generated by OSH Park;
- The hour header's and its associated resistors' silkscreens are all at the bottom of the PCB, not the top. They should be switched;
- I am not entirely confortable with the spacing between 2 vias near the programming header. They can and should be spaced more;
- The PCB's title is partially hidden by the mounting hole. It's also quite tiny, maybe I should split it and increase the font size;
- The programming header's arrow is too big;
- The battery header's label is partially hidden by the VBAT test point;
- Perhaps the test points are a bit too big for no reason. It's not really a problem, unless I want to solder something in it;
- On the bottom copper layer pin 3 of the adjustment header is not tied to ground. I think it should be. Update: No, it's tied to Vdd which is fine (top layer);
- Add other connections between the USB port's ground pin and the ground plane, on both the top and bottom layers.
- Use interrupt enabled pins for adjustments
