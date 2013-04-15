#include "pRNG.h"
pRNG prng;

void setup() {
    prng.begin();
    Serial.begin(19200);
    delay(2000);
}

void loop() {
    Serial.println(prng.getRndInt());
    delay(1000);
    Serial.println(prng.getRndByte());
    delay(1000);
}
