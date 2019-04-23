.TL
OpenMP: Numeric Integration with OpenMP
.AU
David LaMartina
lamartid@oregonstate.edu
.AI
Oregon Statue University Ecampus CS475 Spr2019
.NH
Introduction
.LP
Bezier surfaces are a way of sculpting shapes. Because they are analytically
defined, they can be computed to any precision desired. Using some number of
subdivisions in X and Y, NUMNODES x NUMNODES, we're going to find NUMNODES^2
height samples.
.LP
We will think of each height sample as sitting on a 2D tile. That really makes
that height sample act as a volumen, where the tile is extruded vertically from
the bottom to the top. The tiles in the middle of the floor are full-sized, the
tiles along the edges are half-sized, and the tiles in the corners are quarter-
sized. The volume contribution of each extruded height tile needs to be weighted
accordingly.
.NH
Machine and Run Parameters
.LP
Machine: OSU Rabbit (32 cores)
.LP
Run parameters:
.IP \[bu]
Threads: 1, [2..32:2]
.IP \[bu]
NUMNODES: 100, 500, 1000, 2000, 3000, 4000, 5000
.NH
Caclculated Volume
.LP
28.699 cubic units
.NH
Performances
.PSPIC threads.ps
.PSPIC nodes.ps
.NH
Patterns Observered
.NH
Reasoning for Observed Behaviors
.NH
Parallel Fraction
.NH
Maximum Possible Speed-up

