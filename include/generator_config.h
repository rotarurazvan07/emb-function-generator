/* Number of defined waves */
#define GENERATOR_WAVES_NUMBER ((sizeof(waves_types)/sizeof(0[waves_types])) / ((size_t)(!(sizeof(waves_types) % sizeof(0[waves_types])))))

/* Buttons used for interrupts */
#define WAVE_SELECT_BUTTON     PD2
#define WAVE_UTILITY_BUTTON    PD3
#define WAVE_SIGNAL_OUTPUT_PIN 9

/* The wave types */
#define GENERATOR_NO_WAVE            14
#define GENERATOR_WAVE_TYPE_SQUARE   10

/* The wave types names */
#define GENERATOR_WAVE_TYPE_SQUARE_NAME     "Square"
#define GENERATOR_NO_WAVE_NAME              "Continuous"

#define GENERATOR_FREQ_KNOB_PIN     A2
#define GENERATOR_USE_FREQ_KNOB_YES 0
#define GENERATOR_USE_FREQ_KNOB_NO  1
#define GENERATOR_USE_FREQ_KNOB GENERATOR_USE_FREQ_KNOB_NO

/* Input desired frequency here if not using knob */
#if GENERATOR_USE_FREQ_KNOB == GENERATOR_USE_FREQ_KNOB_YES
    #define GENERATOR_FREQUENCY 100UL
#endif

#define GENERATOR_DUTY_KNOB_PIN     A1
#define GENERATOR_USE_DUTY_KNOB_YES 0
#define GENERATOR_USE_DUTY_KNOB_NO  1
#define GENERATOR_USE_DUTY_KNOB GENERATOR_USE_DUTY_KNOB_YES

/* Input desired duty here if not using knob */
#if GENERATOR_USE_DUTY_KNOB == GENERATOR_USE_DUTY_KNOB_NO
    #define GENERATOR_DUTY 50
#endif

inline String wave_name(int wave_type)
{
    switch (wave_type)
    {
    case GENERATOR_NO_WAVE:
        return String(GENERATOR_NO_WAVE_NAME);
    case GENERATOR_WAVE_TYPE_SQUARE:
        return String(GENERATOR_WAVE_TYPE_SQUARE_NAME);
    default:
        break;
    }
    return String("");
}

/* The waves types list */
const int waves_types[] =
            {
                GENERATOR_NO_WAVE,
                GENERATOR_WAVE_TYPE_SQUARE
            };
