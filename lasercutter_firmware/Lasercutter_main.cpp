#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #if defined(__AVR__)
    #include <avr/io.h>
  #endif
  #include "WProgram.h"
#endif

#include "config.h"
#include "Lasercutter_main.h"
#include "hardware.h"
#include "motorcontrol.h"
#include <math.h>
int x_neu, y_neu, x_aktu, y_aktu;


//int LASER = 10;//14; // A0 //2;
float vspeed = 0;
int cutspeed = 10;//10;
int pwmspeed = 255;

#define COMMAND_SIZE 128
// 0= eigneer code; 1 = orginal
//#define OwnCode

char aWord[COMMAND_SIZE];
String command;
String command_part[6];
int part;
int last_space;
long serial_count;
long no_data = 0;
float x,y,i,j;
char c;
long code;
char buf[10]; // Buffer für Umwandlung String to Float

bool laserpwm =false;



void startup()
{
  Serial.begin(9600);

  x_aktu= 0;
  y_aktu= 0;
  init_hardware();
  laser_off();
}

void main_lasercutter()
{


   if (Serial.available() > 0) {
    // nächstes Zeichen
    //====================
    c = Serial.read();
    no_data = 0;

    // newline ist Befehlsende
    //==========================
    if (c != '\n') {
      aWord[serial_count] = c;
      serial_count++;
    }
  }

    if (serial_count && (c == '\n'))  {

    no_data = 0;
    c = ' ';
    command = aWord;
    int length;

    part=0;
    last_space= 0;
   //Serial.print("alles da");
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

     String part0buff = command_part[0];
    //char part0buff[command_part[0].length()];
    //part0buff[0] = command_part[0];
     int cnt = 0;
     bool b = false;

     for (int i=0; i <= command_part[0].length() ; i++)
     {
        if ((part0buff[i] && part0buff[i] >= 65) || (122 >= part0buff[i] && part0buff[i] >= 79))
        {
          cnt++;
          //Serial.print(cnt,DEC);
        }
        if(cnt == 2 && b == false)
          {
            //Serial.print("double");
            command_part[1] = command_part[0].substring(i,command_part[0].length());
            command_part[0] = command_part[0].substring(0,i);
            b = true;
            //Serial.print(command_part[0] );
            //Serial.print(command_part[1] );
          }
     }

    //Serial.print("ende");


    for (int i=0; i < part; i++){

    }

      if ((command_part[0] == "G01") || (command_part[0] == "g01") || (command_part[0] == "G1")) {
        extract_parameter();
        MoveTo(x, y,cutspeed);
        //moveTo(x_neu, y_neu,cutspeed);
        Serial.println("OK");
       }

     if ((command_part[0] == "G02") || (command_part[0] == "g02") || (command_part[0] == "G2")) {
        extract_parameter();

        MoveArc(x, y, i, j, false, cutspeed);
        //MoveTo(x, y,cutspeed);
        //moveTo(x_neu, y_neu,cutspeed);
        Serial.println("OK");
       }

     if ((command_part[0] == "G03") || (command_part[0] == "g03") || (command_part[0] == "G3")) {
        extract_parameter();
        MoveArc(x, y, i, j, true, cutspeed);

        //MoveTo(x, y,cutspeed);
        //moveTo(x_neu, y_neu,cutspeed);
        Serial.println("OK");
       }

     if ((command_part[0] == "G21") || (command_part[0] == "g21")) {

        //Nix tun
        Serial.println("OK");
       }

     if ((command_part[0] == "G90") || (command_part[0] == "g90")) {

        //Nix tun
        Serial.println("OK");
       }

     if ((command_part[0] == "G0") || (command_part[0] == "g0")) {
        extract_parameter();
        MoveTo(x, y,5);
        //moveTo(x_neu, y_neu,5);
        Serial.println("OK");
       }

     if ((command_part[0] == "M03") || (command_part[0] == "M3") || (command_part[0] == "m3") || (command_part[0] == "m03")) {
       if (command_part[1].startsWith("S")) {
         extract_parameter();

       }



         laser_on(pwmspeed);




        Serial.println("OK");
       }

     if ((command_part[0] == "M05") || (command_part[0] == "M5") || (command_part[0] == "m5") || (command_part[0] == "m05")) {
        //Laser aus
        //laserpwm = false;
        laser_off();
        Serial.println("OK");
       }



     if ((command_part[0] == "M02") || (command_part[0] == "M2")) {
        //Laser aus
        //laserpwm = false;
        laser_off();
        Serial.println("OK");
       }
       if (command_part[0] == "M18") {
        turnmotorsoff();
        Serial.println("OK");
       }
    clear_process_string();
    }


    //#endif
}








void clear_process_string()
{
  // init

  for (byte i=0; i<COMMAND_SIZE; i++)
    aWord[i] = 0;
    serial_count = 0;
}




void extract_parameter()
{


   if (command_part[1].startsWith("X")) {
        command_part[1] = command_part[1].substring(1, command_part[1].length()+1);
        command_part[1].toCharArray(buf,command_part[1].length()+1);

        x = atof (buf);
        /*
        x = x *10;
        x_neu = int (x);
        */
      }
      if (command_part[2].startsWith("Y")) {
        command_part[2] = command_part[2].substring(1, command_part[2].length()+1);
        command_part[2].toCharArray(buf,command_part[2].length()+1);
        y= atof (buf);


       /*
        y = y *10;

        y_neu = int (y);*/

      }

      if (command_part[3].startsWith("I"))
      {
        command_part[3] = command_part[3].substring(1, command_part[3].length() + 1);
        command_part[3].toCharArray(buf,command_part[3].length()+1);
        i = atof(buf);

      }

      if (command_part[4].startsWith("J"))
      {
        command_part[4] = command_part[4].substring(1, command_part[4].length() + 1);
        command_part[4].toCharArray(buf,command_part[4].length()+1);
        j = atof(buf);

      }

      if (command_part[1].startsWith("F")) {
        command_part[1] = command_part[1].substring(1, command_part[1].length()+1);
        command_part[1].toCharArray(buf,command_part[1].length()+1);
        vspeed = atof (buf);
        cutspeed = int(900/vspeed);//int (1/(((steps_per_mm_X + steps_per_mm_X)/2) *2*pow(10,5)));

      }

      if (command_part[1].startsWith("S")) {
        command_part[1] = command_part[1].substring(1, command_part[1].length()+1);
        command_part[1].toCharArray(buf,command_part[1].length()+1);

        pwmspeed = atof (buf);
        pwmspeed = int (pwmspeed);
        //Serial.println(pwmspeed);


      }




}
