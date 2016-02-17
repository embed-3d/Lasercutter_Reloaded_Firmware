#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #if defined(__AVR__)
    #include <avr/io.h>
  #endif
  #include "WProgram.h"
#endif
#include "config.h"
#include "hardware.h"

void init_hardware()
{
  
  pinMode(LASER_PIN, OUTPUT);
  laser_off();

}
void laser_on(int pwm)
{
  // no pwm
  //#if(LASER_PWM == 0)
  //digitalWrite(LASER_PIN, HIGH);
//pwm
  //#else
  analogWrite(LASER_PIN, pwm);

  //#endif



}
void laser_off()
{
    //#if(LASER_PWM == 0)
    //digitalWrite(LASER_PIN, LOW);
    //pwm
    //#else
    analogWrite(LASER_PIN, 0);

    //#endif


}
