/*
This is a simple sketch to demonstrate the use of pRNG, the
<p>retty <R>andom <N>umber <G>enerator

pRNG is a simple pseudo-random number generator. It uses a mechanism based
on an interrupt raised by the WatchDog Timer of the microcontroller to 
collect entropy and a Galois 32 bit LFSR (Linear Feedback Shift Register)
to distribuite it into a pool.

More info on the mechanism of the entropy collector and the methods 
and their usage can be found into the README file.

Written by Leonardo Miliani <leonardo AT leonardomiliani DOT com>
	
This code and the pRNG library are free software; you can redistribute 
and/or modify them under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3.0 of the License,
or (at your option) any later version.

The pRNG library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 

WARNING - IMPORTANT ADVICE FOR ALL THE USERS OF pRNG

pRNG is NOT intended to be used in critical applications where a REAL
random number generator is required because the mechanism used by
this library does NOT guarantee that the sequence won't contain repeating
patterns. If you need a more secure algorithm, try looking something else.

*/

//library inclusion
#include "pRNG.h"
pRNG prng; //new istance of the class pRNG

//setup
void setup() {
    Serial.begin(9600); //let's open the serial
    delay(2000);
}

//main loop
void loop() {
    Serial.println(prng.getRndLong()); //print an unsigned long random number
    delay(1000);
    Serial.println(prng.getRndInt()); //print an unsigned int random number
    delay(1000);
    Serial.println(prng.getRndByte()); //print a byte random number
    delay(1000);
}