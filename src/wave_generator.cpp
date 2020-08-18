#include "wave_manager.h"
#include "wave_generator.h"

#include "generator_config.h"

void wave_generate(int current_wave_type)
{
    Serial.println(wave_name(current_wave_type));
    if (current_wave_type == GENERATOR_NO_WAVE)
    {
        digitalWrite(WAVE_SIGNAL_OUTPUT_PIN, HIGH);
        while(wave_signaling);
        digitalWrite(WAVE_SIGNAL_OUTPUT_PIN, LOW);
    }
    if (current_wave_type == GENERATOR_WAVE_TYPE_SQUARE)
    {
        pinMode(WAVE_SIGNAL_OUTPUT_PIN, OUTPUT);

        unsigned long halfPeriod = 1000000UL * 1UL/32000; // 5mS = 1/2 100 Hz
        Serial.println(halfPeriod);
        // don't use a floating # for delay, delayMicroseconds

        while(wave_signaling)
        {
            digitalWrite(WAVE_SIGNAL_OUTPUT_PIN, HIGH);
            delayMicroseconds(halfPeriod/2);

            digitalWrite(WAVE_SIGNAL_OUTPUT_PIN, LOW);
            delayMicroseconds(halfPeriod/2);
        }
        digitalWrite(WAVE_SIGNAL_OUTPUT_PIN, LOW);
    }
}