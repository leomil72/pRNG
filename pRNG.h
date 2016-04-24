/*
	pRNG.h - <p>retty <R>andom <N>umber <G>enerator
	
    pRNG is a simple pseudo-random number generator. It uses
    a mechanism based on an interrupt raised by the WatchDog
    Timer of the microcontroller to collect entropy and a
    Galois 32 bit LFSR (Linear Feedback Shift Register) to
    distribuite it into a 10-bytes pool.
    
    ** IMPORTANT: pRNG is NOT intended for use in critical applications!! **
    
    For more infos, please read the README.txt file.

	Written by Leonardo Miliani <www DOT leonardomiliani DOT com>
    
    The latest version of this library can be found at:
    http://www.leonardomiliani.com/
    or at:
    https://github.com/leomil72
    	
  	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public
	License as published by the Free Software Foundation; either
	version 3.0 of the License, or (at your option) any later version.

    You should have received a copy of the GNU General Public License
    along with this library.  If not, see <http://www.gnu.org/licenses/>
    
	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

*/


#ifndef pRNG_H
#define pRNG_H


//library version
#define pRNG_VERSION 122


//this library is compatible both with Arduino <=0023 and Arduino >=100
#if defined(ARDUINO) && (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


//check MCU compatibility - pRNG won't work on Atmega8 
#if defined (__AVR_ATmega8__) || defined (__AVR_ATmega8A__)
#error Sorry, this MCU is not supported (lack of interrupt vector for WDT)!
#endif

//set the size of the pool depending on the amount of SRAM
#ifdef RAMEND
    #if (RAMEND < 512)
    #define PRNG_POOL_SIZE 8
    #elif (RAMEND < 1024)
    #define PRNG_POOL_SIZE 12
    #else
    #define PRNG_POOL_SIZE 16
    #endif
#else
    #define PRNG_POOL_SIZE 8
#endif

//pRNG class
class pRNG {
	public: 
		//public methods
		pRNG();
		uint8_t getRndByte(void);
        uint16_t getRndInt(void);
        uint32_t getRndLong(void);
	private:
        //private methods
        void _setWDT();
};


#endif
