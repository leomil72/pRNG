/* 
    This file is part of pRNG library.
    Please check the README file and the notes inside the pRNG.h file
*/

//include required files
#include "pRNG.h"
#include <avr/wdt.h>


//global settings - modify them to change the pRNG characteristics
const uint8_t _dimPrngPool = 10; //size of the pool in bytes
volatile byte _prngPool[_dimPrngPool]; //random pool
volatile byte _prngPointer = 0; //points to the current cell of the pool
volatile byte _prngBitPointer = 0; //points to the current bit of the cell being used
uint8_t _initialized;

//class constructor
pRNG::pRNG(void) {
	_initialized = 0;
}


//class initialization
void pRNG::begin() {
    //we ensure that the memory cells that will contain the pool are empty
    for (byte _tempI = 0; _tempI < _dimPrngPool; _tempI++) {
        _prngPool[_tempI] = 0;
    }
    _setWDT(); //initialize the WDT
	_initialized = 1;
}


//return a random byte from the pool
uint8_t pRNG::getRndByte() {
    if (!_initialized) {
        return 0;
    }
    uint8_t _result;
    if (_prngPointer < 1) { //value not ready
        while (_prngPointer < 1) {} //we'll wait...
    }
    SREG &= ~(1<<SREG_I); //disable interrupts so we can access the pool securely
    _result = _prngPool[0]; //get the first byte
    //shift the bytes of the pool to the left to full the empty cell
    for (uint8_t _tempPointer = 0; _tempPointer < _dimPrngPool; _tempPointer++) {
        _prngPool[_tempPointer] = _prngPool[_tempPointer + 1];
    }
    //clear the last cell - we don't want to insert recursion into the pool
    _prngPool[_prngPointer] = 0; 
    _prngPointer--;
    SREG |= (1<<SREG_I); //reactivate the interrupts
    return _result;
}


//return an unsinged
unsigned int pRNG::getRndInt() {
    return ((getRndByte()<<8) | getRndByte());
}


/* 
************************************************************
WARNING!! DO NOT MODIFY THE FOLLOWING CODE IF YOU DON'T KNOW
WHAT YOU'RE DOING! YOU COULD PUT YOUR MICROCONTROLLER IN A
NEVERENDING RESET!!
************************************************************
*/

//set the WatchDog Timer
void pRNG::_setWDT() {
    MCUSR = 0; //ensure that the reset vectors are off
    wdt_disable(); //disable WD 
    SREG &= ~(1<<SREG_I); //disable all the interrupts    
    
    //set WDT to raise an interrupt every 16 ms
    _WD_CONTROL_REG = ((1<<_WD_CHANGE_BIT) | (1<<WDE));
    _WD_CONTROL_REG = (1<<WDIE);
    
    SREG |= (1<<SREG_I); //re-enable interrupts
}


//main core of the algorithm - this function collects entropy
//from timer 1 and redistribuite it using a Galois 32-bits LFSR
//into a 10-bytes pool
ISR(WDT_vect) {
    volatile static unsigned long _lfsr = 1; //LFSR starting values
    _lfsr = (_lfsr >> 1) ^ (-(_lfsr & 1ul) & 0xD0000001ul); //rotate the LFSR
    //XOR between the 1st bit of the LFSR and the 1st bit of the TCNT1L register
    //the, put the result into a bit of the pool

#ifdef TCNT1L
    if ((_lfsr & 1) ^ (TCNT1L & 1)) {
#else
    if ((_lfsr & 1) ^ (TCNT0 & 1)) {
#endif
        _prngPool[_prngPointer] |= (1 << _prngBitPointer);
    } else {
        _prngPool[_prngPointer] &= ~(1 << _prngBitPointer);
    }
    //increment the bit's pointer so we distribuite the entropy inside the whole pool
    if(++_prngBitPointer > 7) {
        _prngBitPointer = 0;
        if (++_prngPointer == _dimPrngPool) {
           _prngPointer = 0;
        }
    }
}
