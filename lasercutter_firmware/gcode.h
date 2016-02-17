#ifndef gcode_h
#define gcode_h

#include "Arduino.h"
// G Codes
#define G0  "G0"
#define G01 "G01"
#define G02 "G02"
#define G03 "G03"
#define G21 "G21"
#define G90 "G90"

//M Codes
#define M02 "M02"
#define M03 "M03"
#define M05 "M05"
#define M18 "M18"

typedef struct {
  String Code;
  float X = 0;
  float Y = 0;
  float Z = 0;
  float E0 = 0;
  float E1 = 0;
  float F = 0;
  float S = 0;
  float I = 0;
  float J = 0;
  bool b_x = false;
  bool b_y = false;
  bool b_z = false;
  bool b_e0 = false;
  bool b_e1 = false;
  bool b_f = false;
  bool b_s = false;
  bool b_i = false;
  bool b_j = false;
}gcode_line;

void process_g(String command);

#endif
