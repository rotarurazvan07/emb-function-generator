#include "generator.h"

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
    // put your main code here, to run repeatedly:
    Serial.println(GENERATOR_DUTY_CYCLE);
}
