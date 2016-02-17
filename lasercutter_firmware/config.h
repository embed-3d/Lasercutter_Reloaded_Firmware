#ifndef _config_h
#define _config_h

// 0 - no pwm contorlled laser
// 1 - pwm controlled laser
#define LASER_PWM 1

#define steps_per_mm_X 74.44
#define steps_per_mm_Y 74.51



// Hardware config
#if (LASER_PWM == 1)
// SERVO 1 connector Adafruit motor shield V 1.2
#define LASER_PIN 10
#else
// A0 input Adaruit shiel
#define LASER_PIN 14
#endif






#endif
