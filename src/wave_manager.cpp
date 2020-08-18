#include "wave_manager.h"
#include "wave_generator.h"

#include "generator_config.h"

volatile int waves_types_index = 0;
volatile int wave_type_current = -1;
volatile bool wave_signaling = false;

void generator_start()
{
    Serial.begin(9600);
    Serial.println("Starting debug!");


    pinMode(WAVE_SIGNAL_OUTPUT_PIN, OUTPUT);
    digitalWrite(WAVE_SIGNAL_OUTPUT_PIN, LOW);
    Serial.println(GENERATOR_WAVES_NUMBER);

    pinMode(WAVE_SELECT_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(WAVE_SELECT_BUTTON), wave_select_btn_ISR, FALLING);

    pinMode(WAVE_UTILITY_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(WAVE_UTILITY_BUTTON), wave_utility_btn_ISR, FALLING);
}

void generator_execute()
{
    if (wave_signaling)
    {
        wave_generate(wave_type_current);
    }
}

static void wave_utility_btn_ISR()
{
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();

    // If interrupts come faster than 200ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > 300)
    {
        if (wave_signaling == false)
        {
            if (wave_type_current == -1)
                Serial.println("Please select a wave type, there is no default");
            else
            {
                detachInterrupt(digitalPinToInterrupt(WAVE_SELECT_BUTTON));
                Serial.print("Selected: ");
                Serial.println(wave_name(wave_type_current));
                wave_signaling = true;
            }
        }
        else
        {
            wave_signaling = false;

            Serial.println("Exiting current signal run");

            // Clear interrupt flag
            EIFR = bit(INTF0);

            attachInterrupt(digitalPinToInterrupt(WAVE_SELECT_BUTTON), wave_select_btn_ISR, FALLING);
        }
    }
    last_interrupt_time = interrupt_time;
}

static void wave_select_btn_ISR()
{
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();

    // If interrupts come faster than 200ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > 300)
    {
        if (waves_types_index == GENERATOR_WAVES_NUMBER)
            waves_types_index = 0;
        wave_type_current = waves_types[waves_types_index++];
        Serial.println(wave_name(wave_type_current));
    }
    last_interrupt_time = interrupt_time;
}
