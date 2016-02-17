#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #if defined(__AVR__)
    #include <avr/io.h>
  #endif
  #include "WProgram.h"
#endif


#include "motorcontrol.h"
#include "Lasercutter_main.h"

#include <math.h>
#include "AFMotor.h"

AF_Stepper motor1(48, 2); //Y-Achse
AF_Stepper motor2(48, 1); //X-Achse 

double lastx_g;
double lasty_g;


void line(int x0, int y0, int x1, int y1, int velo)
{
  int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;  // 1 Forward -1 Backward
  int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;  //
  int err = dx+dy, e2; /* error value e_xy */

 
  for(;;){  /* loop */
 
    
    if (x0==x1 && y0==y1) break;
    e2 = 2*err;
    if (e2 > dy) { 
      err += dy; x0 += sx; 
      if (sx == 1) motor1.onestep(FORWARD, MICROSTEP);//INTERLEAVE); 
      else motor1.onestep(BACKWARD, MICROSTEP);//INTERLEAVE); 
  
     } 
    
    if (e2 < dx) {
       err += dx; y0 += sy; 
       if (sy == 1) motor2.onestep(FORWARD, MICROSTEP);//INTERLEAVE); 
       else motor2.onestep(BACKWARD, MICROSTEP);//INTERLEAVE); 
       } 
    delay (velo);
  }
}

// This method assumes the limits have already been checked.
// This method assumes the start and end radius match (both points are same distance from center)
// This method assumes arcs are not >180 degrees (PI radians)
// This method assumes all movement is at a constant Z height
// cx/cy - center of circle
// x1/y1 - end position  (vorher x/y)
// dir - ARC_CW or ARC_CCW to control direction of arc
// (posx,posy,posz) is the starting position|| x0 y0 startingposition
// line() is our existing command to draw a straight line using Bresenham's algorithm.
// dir - Arc_CW = true , arc_CCW = false;

void arc(double posx, double posy, double posi,double posj,double x1,double y1, bool dir,int velo)
{
    double cx,cy;
    // berechne mittlepunkt
    cx = posx + posi;
    cy = posy + posj;


    // get radius
    double dx = posx -cx;
    double dy = posy - cy;
    double radius = sqrt(pow(dx, 2) + pow(dy, 2));


    
    // find the sweep of the arc

      double angle1 = atan3(dy, dx);
      double angle2 = atan3(y1 - cy, x1 - cx);
      double sweep = angle2 - angle1;

    // ARC_CW
      if(dir ==  true && sweep < 0) 
    {
        angle2 += 2 * PI;
    }
          // ARC_CCW
      else if (dir == false && sweep > 0)
      {
          angle1 += 2  *PI;
      }

      sweep = angle2 - angle1;

      // get length of arc
      // float circumference=PI*2.0*radius;
      // float len=sweep*circumference/(PI*2.0);
      // simplifies to
      double len = abs(sweep) * radius;
      

      double CM_PER_SEGMENT = 0.01;

      int i, num_segments = abs(int(floor( len / CM_PER_SEGMENT )));
      
      // declare variables outside of loops because compilers can be really dumb and inefficient some times.
      double nx, ny, angle3, fraction;
     
      for(i=0;i<num_segments;++i) {
        // interpolate around the arc
                 
        fraction = ((float)i)/((float)num_segments);
        angle3 =  sweep * fraction  + angle1;

        // find the intermediate position
        nx = cx + cos(angle3) * radius ;
        ny = cy + sin(angle3) * radius;
        // make a line to that intermediate position
        //Serial.println(nx,DEC) ;
        //Serial.println(ny,DEC) ;   
        Line(nx, ny , velo);    
     
      }

    // one last line hit the end
    Line(x1, y1,velo);
    //Serial.println("ende loop"); 
}



void Line( double xin, double yin, int velo)
{
    //schrittumrechung
    int x1 = int(steps_per_mm_X * xin);
    int y1 = int(steps_per_mm_Y * yin);



    int x0 = x_aktu; //int(round((lastx_g / 2.54) * int(dpi_X)));
    int y0 = y_aktu; //int(round((lasty_g / 2.54) * int(dpi_X)));

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;  // 1 Forward -1 Backward
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;  //
    int err = dx + dy, e2; /* error value e_xy */


    for (; ; )
    {  /* loop */


        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 > dy)
        {
            err += dy;
            x0 += sx;
            if (sx == 1) motor1.onestep(FORWARD, MICROSTEP);//INTERLEAVE); 
            else  motor1.onestep(BACKWARD, MICROSTEP);//INTERLEAVE); 

        }

        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
            if (sy == 1) motor2.onestep(FORWARD, MICROSTEP);//INTERLEAVE); 
            else motor2.onestep(BACKWARD, MICROSTEP);//INTERLEAVE); 
        }
delay (velo);
            x_aktu = x1;
            y_aktu = y1;
       

    }

    //x_aktu = x1;
    //y_aktu = y1;

    


}

double atan3(double dy, double dx)
{
  double a = atan2(dy, dx);
  if (a < 0) a = (PI * 2) + a;
  return a;
}

void MoveArc(double xin, double yin, double cx, double cy, bool dir ,int velo)
{
  
  arc(lastx_g, lasty_g, cx, cy, xin, yin, dir, velo);
  lastx_g = xin;
  lasty_g = yin;
}

//neue methode
void MoveTo(double gxin, double gyin, int velo)
{
Line(gxin, gyin, velo);
lastx_g = gxin;
lasty_g = gyin; 
}


void moveTo(int x1, int y1, int velo)
{
  line (x_aktu, y_aktu, x1, y1, velo);
  x_aktu=x1;
  y_aktu=y1;

}

void turnmotorsoff()
{
  motor1.release();
  motor2.release();
}
