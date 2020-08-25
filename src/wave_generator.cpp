#include "wave_manager.h"
#include "wave_generator.h"

#include "generator_config.h"

#ifdef GENERATOR_FREQUENCY
    volatile float frequency = GENERATOR_FREQUENCY;
#else
    volatile float frequency = map(analogRead(GENERATOR_FREQ_KNOB_PIN), 0, 1023, OSCILOSCOPE_LOWER_LIMIT, OSCILOSCOPE_UPPER_LIMIT);;

    const int numReadings_freq = 100;
    float readings_freq[numReadings_freq];
    int readIndex_freq = 0;
    float total_freq = 0;
    float average_freq = 0;
    float prev_value_freq = 0;
#endif

#ifdef GENERATOR_DUTY
    volatile float duty_cycle = GENERATOR_DUTY;
#else
    volatile float duty_cycle = map(analogRead(GENERATOR_DUTY_KNOB_PIN), 0, 1023, 0, 100);

    const int numReadings_duty = 100;
    float readings_duty[numReadings_duty];
    int readIndex_duty = 0;
    float total_duty = 0;
    float average_duty = 0;
    float prev_value_duty = 0;
#endif

void wave_generate(int current_wave_type)
{
    if (current_wave_type == GENERATOR_NO_WAVE)
    {
        generate_constant();
    }

    else
    {
        generate_square();
    }
}

static void generate_constant()
{
    digitalWrite(WAVE_SIGNAL_OUTPUT_PIN, HIGH);
    while(wave_signaling);
    digitalWrite(WAVE_SIGNAL_OUTPUT_PIN, LOW);
}

static void generate_square()
{
#ifndef GENERATOR_DUTY
    init_potentiometer_duty();
#endif

#ifndef GENERATOR_FREQUENCY
    init_potentiometer_freq();
#endif

    init_timer1_pwm();

        while(wave_signaling)
        {
#ifndef GENERATOR_FREQUENCY
            read_potentiometer_freq();

            if ((int)prev_value_freq != (int)average_freq)
            {
                frequency = map(average_freq, 0, 1023, OSCILOSCOPE_LOWER_LIMIT, OSCILOSCOPE_UPPER_LIMIT);
                prev_value_freq = average_freq;

                setup_timer1_pwm();
            }

            delay(1);
#endif

#ifndef GENERATOR_DUTY
            read_potentiometer_duty();

            if (map(prev_value_duty, 0, 1023, 0, 100) != map(average_duty, 0, 1023, 0, 100))
            {
                duty_cycle = map(average_duty, 0, 1023, 0, 100);
                prev_value_duty = average_duty;

                setup_timer1_pwm();
            }

            delay(1);
#endif
        }

#ifndef GENERATOR_FREQUENCY
    uninit_potentiometer_freq();
#endif

#ifndef GENERATOR_DUTY
    uninit_potentiometer_duty();
#endif

    uninit_timer1_pwm();
}

#ifndef GENERATOR_FREQUENCY
    static void init_potentiometer_freq()
    {
        for (int thisReading = 0; thisReading < numReadings_freq; thisReading++) {
            readings_freq[thisReading] = 0;
        }
    }

    static void read_potentiometer_freq()
    {
        total_freq = total_freq - readings_freq[readIndex_freq];
        readings_freq[readIndex_freq] = analogRead(GENERATOR_FREQ_KNOB_PIN);
        total_freq = total_freq + readings_freq[readIndex_freq];
        readIndex_freq = readIndex_freq + 1;

        if (readIndex_freq >= numReadings_freq) {
            readIndex_freq = 0;
        }

        average_freq = total_freq / numReadings_freq;
    }

    static void uninit_potentiometer_freq()
    {
        init_potentiometer_freq();
        readIndex_freq = 0;
        total_freq = 0;
        average_freq = 0;
    }
#endif

#ifndef GENERATOR_DUTY
    static void init_potentiometer_duty()
    {
        for (int thisReading = 0; thisReading < numReadings_duty; thisReading++) {
            readings_duty[thisReading] = 0;
        }
    }

    static void read_potentiometer_duty()
    {
        total_duty = total_duty - readings_duty[readIndex_duty];
        readings_duty[readIndex_duty] = analogRead(GENERATOR_DUTY_KNOB_PIN);
        total_duty = total_duty + readings_duty[readIndex_duty];
        readIndex_duty = readIndex_duty + 1;

        if (readIndex_duty >= numReadings_duty) {
            readIndex_duty = 0;
        }

        average_duty = total_duty / numReadings_duty;
    }

    static void uninit_potentiometer_duty()
    {
        init_potentiometer_duty();
        readIndex_duty = 0;
        total_duty = 0;
        average_duty = 0;
    }
#endif

static void init_timer1_pwm()
{
    uninit_timer1_pwm();

    TCCR1A |= (1 << COM1A1);

    TCCR1A |= (1 << WGM11);

    setup_timer1_pwm();
}

static void setup_timer1_pwm()
{
    TCCR1B = (1 << WGM12) | (1 << WGM13);
    float prescaler = 0;
    if (frequency < 200){
        TCCR1B |= (1 << CS12);
        prescaler = 256UL;
    }
    else
        TCCR1B |= (1 << CS10);

    float pwmFrequency;
    if (prescaler != 0)
        pwmFrequency = F_CPU / (prescaler * frequency) - 1;
    else
        pwmFrequency = F_CPU / frequency - 1;

    float dutyCycleDivisor = 100 / duty_cycle;
    float pwmValueWithDutyCycle = pwmFrequency  / dutyCycleDivisor;

    ICR1 = (uint16_t)pwmFrequency;
    OCR1A = (uint16_t)pwmValueWithDutyCycle;
}

static void uninit_timer1_pwm()
{
    TCCR1A = 0;
    TCCR1B = 0;
    ICR1   = 0;
    OCR1A  = 0;
}

/*
static void init_timer1()
{
        cli();

        uninit_timer1();

        TCCR1B |= (1 << WGM12);
        setup_timer1();
        TIMSK1 |= (1 << OCIE1A);

        sei();
}

static void setup_timer1()
{
        TCCR1B = (1 << WGM12);
        unsigned long prescaler = 0;
        if (frequency < 200){
            TCCR1B |= (1 << CS12);
            prescaler = 256UL;
        }
        else
            TCCR1B |= (1 << CS10);

        if (prescaler != 0)
            OCR1A = F_CPU / (prescaler * (frequency * 2UL)) - 1;
        else
            OCR1A = F_CPU / (frequency * 2UL) - 1;
}

static void uninit_timer1()
{
        TCCR1A = 0;
        TCCR1B = 0;
        TCNT1  = 0;
        OCR1A  = 0;
        TIMSK1 = 0;
}
ISR(TIMER1_COMPA_vect)
{

}
*/
