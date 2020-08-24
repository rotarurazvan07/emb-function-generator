#include "Arduino.h"

extern boolean reading_knob_freq;

void wave_generate(int current_wave_type);

static void uninit_timer1();
static void init_timer1();
static void setup_timer1();

static void uninit_timer1_pwm();
static void init_timer1_pwm();
static void setup_timer1_pwm();

static void generate_constant();
static void generate_square();
static void generate_sine();

static void init_potentiometer_freq();
static void read_potentiometer_freq();
static void uninit_potentiometer_freq();

static void init_potentiometer_duty();
static void read_potentiometer_duty();
static void uninit_potentiometer_duty();
