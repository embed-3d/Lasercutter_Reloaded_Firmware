#include "gcode.h"

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #if defined(__AVR__)
    #include <avr/io.h>
  #endif
  #include "WProgram.h"
#endif

#include <math.h>
#include "Configuration.h"
#include "commands.h"
gcode_line extract_gcode_paramters(String command_part[], int number_of_parts);



void process_g(String command)
{
  Serial.println(command.length());
  //gcode_line a;

  int serial_count = command.length();
  int last_space= 0;
  int part = 0;
  String command_part[6];
  // Seperate string into substrings
  for (int i=0; i < serial_count; i++)
  {
    if (command.charAt(i) == ' ') {
      command_part[part] = command.substring(last_space,i);
      last_space=i+1;
      if (command_part[part].length() != 0) {
         part++;
      }
    }
  }
  command_part[part] = command.substring(last_space,serial_count); //letztes Kommando extrahieren
  part++;
  Serial.print("Number of parts: ");
  Serial.println(part);
  String part0buff = command_part[0];

  int cnt = 0;
  bool b = false;

   for (int i=0; i <= command_part[0].length() ; i++)
   {
      if ((part0buff[i] && part0buff[i] >= 65) || (122 >= part0buff[i] && part0buff[i] >= 79))
      {
        cnt++;
        #ifdef _debug_
        Serial.print(cnt,DEC);
        #endif
      }
      if(cnt == 2 && b == false)
        {
          //Serial.print("double");
          command_part[1] = command_part[0].substring(i,command_part[0].length());
          command_part[0] = command_part[0].substring(0,i);
          b = true;
          #ifdef _debug_
          Serial.print(command_part[0] );
          Serial.print(command_part[1] );
          #endif
        }





 }
 gcode_line line = extract_gcode_paramters(command_part, part);


  Serial.println("\n-Code: " + line.Code);
  Serial.print("\n-x: ");
  Serial.println( line.X);
  Serial.print("\n-y: ");
  Serial.println( line.Y);
  Serial.print("\n-z: ");
  Serial.println( line.Z);
  Serial.print("\n-e0: ");
  Serial.println(line.E0);
  Serial.print("\n-e1: ");
  Serial.println(line.E1);
  Serial.print("\n-f: ");
  Serial.println(line.F);
  Serial.print("\n-s: ");
  Serial.println(line.S);
  Serial.print("\n-i: ");
  Serial.println(line.I);
  Serial.print("\n-j: ");
  Serial.println(line.J);

  process_command(line);


}

gcode_line extract_gcode_paramters(String command_part[], int number_of_parts)
{
  char buf[10]; // Buffer  Umwandlung String to Float

  gcode_line out;

  out.Code = command_part[0];


  for(int i = 1; i < number_of_parts; i++)
  {

    if (command_part[i].startsWith("X"))
    {
        command_part[i] = command_part[i].substring(1, command_part[i].length()+1);
        command_part[i].toCharArray(buf,command_part[i].length()+1);
        out.X = atof (buf);
        out.b_x = true;
    }
    else if (command_part[i].startsWith("Y"))
    {
       command_part[i] = command_part[i].substring(1, command_part[i].length()+1);
       command_part[i].toCharArray(buf,command_part[i].length()+1);
       out.Y= atof (buf);
       out.b_x = true;
     }
     else if (command_part[i].startsWith("Z"))
     {
        command_part[i] = command_part[i].substring(1, command_part[i].length()+1);
        command_part[i].toCharArray(buf,command_part[i].length()+1);
        out.Z= atof (buf);
        out.b_z = true;
      }
     else if (command_part[i].startsWith("E0"))
     {
        command_part[i] = command_part[i].substring(2, command_part[i].length()+1);
        command_part[i].toCharArray(buf,command_part[i].length()+1);
        out.E0= atof (buf);
        out.b_e0 = true;
      }
      else if (command_part[i].startsWith("E1"))
      {
         command_part[i] = command_part[i].substring(2, command_part[i].length()+1);
         command_part[i].toCharArray(buf,command_part[i].length()+1);
         out.E1= atof (buf);
         out.b_e1 = true;
       }
    else if (command_part[i].startsWith("I"))
    {
      command_part[i] = command_part[i].substring(1, command_part[i].length() + 1);
      command_part[i].toCharArray(buf,command_part[i].length()+1);
      out.I = atof(buf);
    }
    else if (command_part[i].startsWith("J"))
    {
      command_part[i] = command_part[i].substring(1, command_part[i].length() + 1);
      command_part[i].toCharArray(buf,command_part[i].length()+1);
      out.J = atof(buf);

    }
    else if (command_part[i].startsWith("F"))
    {
      command_part[i] = command_part[i].substring(1, command_part[i].length()+1);
      command_part[i].toCharArray(buf,command_part[i].length()+1);
      out.F = atof (buf);  //vspeed
      //cutspeed = int (1/(((steps_per_mm_X + steps_per_mm_X)/2) *2*pow(10,5)));
    }

    else if (command_part[i].startsWith("S"))
    {
      command_part[i] = command_part[i].substring(1, command_part[i].length()+1);
      command_part[i].toCharArray(buf,command_part[i].length()+1);
      float pwmspeed = atof (buf);
      //pwmspeed = int (pwmspeed);
    }



  }


   // Vereinfachung des weitern prozesses
   if ((out.Code == "G0") || (out.Code == "g0")) {
      out.Code = G0;
   }
   else if ((out.Code == "G01") || (out.Code == "g01") || (out.Code == "G1")) {
      out.Code = G01;
   }
   else if ((out.Code == "G02") || (out.Code == "g02") || (out.Code == "G2")) {
      out.Code = G02;
   }
   else if ((out.Code == "G03") || (out.Code == "g03") || (out.Code == "G3")) {
      out.Code = G03;
   }
   else if ((out.Code == "G21") || (out.Code == "g21")) {
      out.Code = G21;
   }

   else if ((out.Code == "G90") || (out.Code == "g90")) {
      out.Code = G90;
   }

   else if ((out.Code == "M02") || (out.Code == "M2")){
      out.Code = M02;
   }

   else if ((out.Code == "M03") || (out.Code == "M3")) {
      out.Code = M03;
   }

   else if ((out.Code == "M05") || (out.Code == "M5")) {
      out.Code = M05;
   }

   return out;
}
