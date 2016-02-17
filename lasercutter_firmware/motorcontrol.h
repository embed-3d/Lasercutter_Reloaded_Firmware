
// alt
void line(int x0, int y0, int x1, int y1, int velo);
//alt
void moveTo(int x1, int y1, int velo);


void turnmotorsoff();

#define steps_per_mm_X 74.44
#define steps_per_mm_Y 74.51

// neu
void Line( double xin, double yin, int velo);

void MoveArc(double xin, double yin, double cx, double cy, bool dir ,int velo);
void MoveTo(double x1, double y1, int velo);
double atan3(double dy, double dx);
void arc(double posx, double posy, double cx,double cy,double x1,double y1, bool dir,int velo);
