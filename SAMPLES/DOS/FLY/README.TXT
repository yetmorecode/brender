Flying saucer demo
------------------

A simple flight over rugged terrain. Illustrates one approach to rendering
shadows by rendering a silhouette into a texture.

To run the demo type 

fly.

Controls:

Pitch:		Mouse forward/back
Roll:		Mouse left/right
Accelerate:	Left mouse button
Reverse:	Right mouse button

The mouse is effectively a virtual joystick. In the event of entering
an uncontrollable spin use:

Recentre:	Left and right mouse buttons simultaneously.


For fine tuning use the numeric keypad:

Yaw left/right:	1 & 3
Left/right:	4 & 6
Up/down:	2 & 8

Other controls:

Turn on/off autolevelling:	L
Switch viewpoint:		V

The autolevelling works by applying corrective thrust to drive the 
saucer towards the horizontal (or horizontal but upside down). If
autolevelling is enabled and both mouse buttons are depressed the
saucer will quickly right itself.

It is possible to record and play back a flight.
To record a flight invoke the program as 

fly -r <filename>

Ro play it back use

fly -p <filename>
