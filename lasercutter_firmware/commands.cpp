

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #if defined(__AVR__)
    #include <avr/io.h>
  #endif
  #include "WProgram.h"
#endif

#include "gcode.h"
#include "commands.h"
#include "Configuration.h"
#include "motion.h"
#include "hw_control.h"



void process_command(gcode_line command)
{

  //G0:
  if (command.Code == G0) {
    set_speed(20);
    move_to_coordinates(command.X,command.Y,command.Z,command.E0, command.E1);





    //MoveTo(x, y,5);
    //moveTo(x_neu, y_neu,5);
    Serial.println("OK");
  }
  //G01
  else if (command.Code == G01) {
    if(command.b_f)
    {
      set_speed(command.F);
    }
    move_to_coordinates(command.X,command.Y,command.Z,command.E0, command.E1);


    //move_to(command.X,command.Y,command.Z,command.E0, command.E1);

    //MoveTo(x, y,cutspeed);
    //moveTo(x_neu, y_neu,cutspeed);
    Serial.println("OK");
  }
  //G02
  else if (command.Code == G02 ) {
      //MoveArc(x, y, i, j, false, cutspeed);
      //MoveTo(x, y,cutspeed);
      //moveTo(x_neu, y_neu,cutspeed);
      Serial.println("OK");
  }
  //G03
  else if (command.Code == G03) {

    //MoveArc(x, y, i, j, true, cutspeed);

    //MoveTo(x, y,cutspeed);
    //moveTo(x_neu, y_neu,cutspeed);
    Serial.println("OK");
  }
  //G21:
  else if (command.Code == G21) {

    //Nix tun
    Serial.println("OK");
  }
  // G90:
  else if (command.Code == G90) {

    //Nix tun
    Serial.println("OK");
  }
  //M02:
  else if (command.Code == M02)
  {
    // Laser aus
    laser_off();
    Serial.println("OK");
  }
  //M03:
  else if (command.Code == M03)
  {
    if(command.b_s == true)
    {
      // Laser mit Paramter S an
      laser_on_pwm(command.S);
    }
    else
    {
      // Laser an
      laser_on();
    }
    Serial.println("OK");
  }
  //M05:
  else if (command.Code == M05)
  {
    //Laser aus
    laser_off();
    Serial.println("OK");
  }
  //M18:
  else if(command.Code == M18)
  {
    //Motoren aus
    disable_steppers();
    Serial.println("OK");
  }

}
