#include "Arduino.h"

extern volatile bool wave_signaling;

void generator_start();
void generator_execute();
static void wave_utility_btn_ISR();
static void wave_select_btn_ISR();
