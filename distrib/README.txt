How do you compile and run your code? Provide instructions for Athena Linux. If your executable
requires certain parameters to work well, make sure that these are specified.

To compile the code, run "make" inside the distrib/ folder. You can run the
code as ./a3 [integrator] [timestep]. The choices of [integrator] are e, t, and
r, for Euler, Trapezoidal, and RK4. For the time step, .01 works well. When the
code is already running, pressing 't' toggles between the various simulations -
the first is the simple system with the ball around the center; next is the
two-ball pendulum; next is the four-ball particle chain; last is the cloth system.
They wrap around, so pressing 't' while in the Cloth system, it will return
back to the simple system. Pressing 's' causes the cloth system (and only the
cloth system) to swing back and forth. Pressing 'r' resets the current simulation. Pressing 'w' on the cloth demo switches between the wire-frame view and the smooth shading view.
----
Did you collaborate with anyone in the class? If so, let us know who you talked to and what sort of
help you gave or received.

I talked to Chase Lambert and Leah Alpert about high-level design and possible
problems with each approach.
----
Were there any references (books, papers, websites, etc.) that you found particularly helpful for
completing your assignment? Please provide a list.

No, I didn't use anything other than the lecture notes.
----
Are there any known problems with your code? If so, please provide a list and, if possible, describe
what you think the cause is and how you might fix them if you had more time or motivation. This
is very important, as we’re much more likely to assign partial credit if you help us understand what’s
going on.

The one thing that happens when I swing the cloth back and forth (that I tried
to fix in the evalF code for the clothsystem) was that the fixed corner that is
not at the origin veers slightly upwards, even though I zero the forces in the
directions that the external force is not acting in.
---- 
Did you do any of
the extra credit? If so, let us know how to use the additional features. If
there was a substantial amount of work involved, describe what how you did it.

An 'r' keypress resets the particular simulation that you are looking at. This required making a "reset" method for each type of particle class that the constructor (or the reset function) calls. I also implemented smooth shading on the cloth by rendering the triangles (the front and the back). The normals for each vertex are calculated by the average of the cross products calculated between the vectors from the point to it's 4 nearest points (cardinal directions). To switch between the wire frame and smooth frame (for the cloth only), type 'W'. 
----
Got any comments about this assignment that you’d like to share?

Nope.
