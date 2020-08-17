#include "generator.h"

volatile int waves_types_index = 0;
String wave_type_current = "";
volatile bool wave_signaling = false;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("Starting debug!");

    pinMode(wave_select_button, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(wave_select_button), wave_select_btn_ISR, FALLING);

    pinMode(wave_utility_button, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(wave_utility_button), wave_utility_btn_ISR, FALLING);
}

void loop() {
    if (wave_signaling)
        Serial.println(String("Signaling: ") + wave_type_current);
    delay(500);
}

void wave_utility_btn_ISR()
{
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();

    // If interrupts come faster than 200ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > 300)
    {
        if (wave_signaling == false)
        {
            if (wave_type_current == "")
                Serial.println("Please select a wave type, there is no default");
            else
            {
                detachInterrupt(digitalPinToInterrupt(wave_select_button));
                Serial.print("Selected: ");
                Serial.println(wave_type_current);
                wave_signaling = true;
            }
        }
        else
        {
            wave_signaling = false;
            Serial.println("Exiting current signal run");

            // Clear interrupt flag
            EIFR = bit(INTF0);

            attachInterrupt(digitalPinToInterrupt(wave_select_button), wave_select_btn_ISR, FALLING);
        }
    }
    last_interrupt_time = interrupt_time;
}

void wave_select_btn_ISR()
{
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();

    // If interrupts come faster than 200ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > 300)
    {
        if (waves_types_index == GENERATOR_WAVES_NUMBER)
            waves_types_index = 0;
        wave_type_current = String(waves_types[waves_types_index++]);
        Serial.println(wave_type_current);
    }
    last_interrupt_time = interrupt_time;
}
