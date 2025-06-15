/*
 *  Satellite Scene
 *
 *  Draw 3 satellites at different positions in orbit around Earth
 *
 * Key bindings
 *  m          Toggle between orthogonal, perspective, and first-person
 *  +/-        Increase/decrease field of view
 *  PgDn/PgUp  Zoom in and out
 *  arrows     Change view angle
 *  1          View the whole scene
 *  2          View only satellite object
 *  3          View only Earth object
 *  0          Reset view angle
 *  ESC        Exit
 *
 * (in orthogonal or perspective)
 *  a          toggles axis
 * (in first-person)
 *  w          move camera forward
 *  a          move camera left
 *  s          move camera back
 *  d          move camera right
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#ifdef USEGLEW
#include <GL/glew.h>
#endif
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
// Tell Xcode IDE to not gripe about OpenGL deprecation
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#include <GL/glut.h>
#endif
//  Default resolution
//  For Retina displays compile with -DRES=2
#ifndef RES
#define RES 1
#endif

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int obj=1;        //  Display individual object
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
double zh=0;      // Orbit of satellites
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=10;    //  Size of world

//  First-person camera position
double camX; double camY; double camZ;
// First-person camera angle
double lookX; double lookY; double lookZ; 

const char* text[] = {"Scene", "Satellite", "Earth"};
const char* modes[] = {"Orthogonal", "Perspective", "First Person"};

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.14159265/180*(th))
#define Sin(th) sin(3.14159265/180*(th))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Check for OpenGL errors
 */
void ErrCheck(const char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

/*
 *  Print message to stderr and exit
 */
void Fatal(const char* format , ...)
{
   va_list args;
   va_start(args,format);
   vfprintf(stderr,format,args);
   va_end(args);
   exit(1);
}

/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();

   switch (mode)
   {
      case 0:
         //  Orthogonal projection
         glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
         break;
      case 1:
         //  Perspective projection
         gluPerspective(fov,asp,dim/4,4*dim);
         break;
      case 2:
         // First-person mode
         gluPerspective(fov,asp,dim/4,4*dim);
         break;         
   }
      
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

// Draw a solar panel for a satellite
static void panel(double start, double stop, double width, double height)
{
   glColor3f(0,0.1,0.8);
   glBegin(GL_QUADS);
   //  Top
   glVertex3d(-width, height, start);
   glVertex3d(-width, height, stop);
   glVertex3d(width, height, stop);
   glVertex3d(width, height, start);   
   //  Bottom
   glVertex3d(-width, -height, start);
   glVertex3d(-width, -height, stop);
   glVertex3d(width, -height, stop);
   glVertex3d(width, -height, start);
   //  Side 1
   glVertex3d(-width, height, start);
   glVertex3d(-width, height, stop);
   glVertex3d(-width, -height, stop);
   glVertex3d(-width, -height, start);
   //  Side 2
   glVertex3d(width, height, start);
   glVertex3d(width, height, stop);
   glVertex3d(width, -height, stop);
   glVertex3d(width, -height, start);
   // Side 3
   glVertex3d(-width, height, start);
   glVertex3d(width, height, start);
   glVertex3d(width, -height, start);
   glVertex3d(-width, -height, start);
   // Side 4
   glVertex3d(-width, height, stop);
   glVertex3d(width, height, stop);
   glVertex3d(width, -height, stop);
   glVertex3d(-width, -height, stop);
   glEnd();
}

/*
 *  Draw a satellite
 *     at (x,y,z)
 *     nose towards (dx,dy,dz)
 *     up towards (ux,uy,uz)
 *     size
 *  Designed referencing SolidPlane from ex8
 */
static void satellite(double x, double y,  double z,
                      double dx, double dy, double dz,
                      double ux, double uy, double uz,
                      double s)
{
   // Dimensions used to size satellite
   const double nose=+0.50;
   const double tail=-0.30;
   const double inner=0.03;
   const double outer=0.05;
   const double pstart=0.1;
   const double pstop=0.6;
   const double pwid=0.2;
   const double pheight=0.01;
   const double dish=0.20;

   //  Normalize direction vector
   double D0 = sqrt(dx*dx+dy*dy+dz*dz);
   double X0 = dx/D0;
   double Y0 = dy/D0;
   double Z0 = dz/D0;
   //  Normalize up vector
   double D1 = sqrt(ux*ux+uy*uy+uz*uz);
   double X1 = ux/D1;
   double Y1 = uy/D1;
   double Z1 = uz/D1;
   //  Take cross product
   double X2 = Y0*Z1-Y1*Z0;
   double Y2 = Z0*X1-Z1*X0;
   double Z2 = X0*Y1-X1*Y0;
   //  Rotation matrix
   double mat[16];
   mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
   mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
   mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
   mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glMultMatrixd(mat);
   glScaled(s,s,s);

   // Inner body
   glBegin(GL_QUAD_STRIP);
   glColor3f(0.5,0.5,0.5);
   for (int th=0;th<=360;th+=30)
   {
      glVertex3d(nose,inner*Cos(th),inner*Sin(th));
      glVertex3d(tail,inner*Cos(th),inner*Sin(th));
   }
   glEnd();
   // Close back
   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(tail, 0.0, 0.0);
   for (int th=0;th<=360;th+=30)
      glVertex3d(tail,inner*Cos(th),inner*Sin(th));
   glEnd();

   //  Front outer body
   glColor3f(0.7,0.7,0.7);
   glBegin(GL_QUAD_STRIP);
   for (int th=0;th<=360;th+=30)
   {
      glVertex3d(nose-nose/5, outer*Cos(th), outer*Sin(th));
      glVertex3d(nose/5, outer*Cos(th), outer*Sin(th));
   }
   glEnd();   
   // Close body
   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(nose-nose/5, 0.0, 0.0);
   for (int th=0;th<=360;th+=30)
      glVertex3d(nose-nose/5,outer*Cos(th),outer*Sin(th));
   glEnd();
   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(nose/5, 0.0, 0.0);
   for (int th=0;th<=360;th+=30)
      glVertex3d(nose/5,outer*Cos(th),outer*Sin(th));
   glEnd();

   // Back outer body
   glBegin(GL_QUAD_STRIP);
   for (int th=0;th<=360;th+=30)
   {
      glVertex3d(tail/5, outer*Cos(th), outer*Sin(th));
      glVertex3d(tail-tail/5, outer*Cos(th), outer*Sin(th));
   }
   glEnd();   
   // Close body
   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(tail/5, 0.0, 0.0);
   for (int th=0;th<=360;th+=30)
      glVertex3d(tail/5,outer*Cos(th),outer*Sin(th));
   glEnd();
   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(tail-tail/5, 0.0, 0.0);
   for (int th=0;th<=360;th+=30)
      glVertex3d(tail-tail/5,outer*Cos(th),outer*Sin(th));
   glEnd();

   //  Panels
   panel(pstart, pstop, pwid, pheight);
   panel(-pstart, -pstop, pwid, pheight);
   glBegin(GL_QUAD_STRIP); 
   // Connect panels to body
   glColor3f(0.3,0.3,0.3);
   for (int th=0;th<=360;th+=30)
   {
      glVertex3d(inner/2*Cos(th),inner/2*Sin(th),pstart);
      glVertex3d(inner/2*Cos(th),inner/2*Sin(th),-pstart);
   }
   glEnd();

   //  Dish - inspired by the canopy from ex8
   glPushMatrix();
   glTranslated(nose+dish/2,0,0);
   glRotatef(90,0,0,1);
   glScaled(dish,dish,dish);
   glColor3f(0.4, 0.4, 0.4);
   for (int ph=0;ph<90;ph+=30)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=30)
      {
         glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
         glVertex3d(Sin(th)*Cos(ph+30) , Sin(ph+30) , Cos(th)*Cos(ph+30));
      }
      glEnd();
   }
   glPopMatrix();
   glPopMatrix();
}

// Helper for earth -> add a lil green in there
static void Vertex(double th,double ph)
{
   glColor3f(0, Sin(th)*Sin(th)/2, Sin(ph)*Sin(ph)/2);
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}
/*
 *  Just a blue sphere at origin for the Earth
 *  Sphere code adapted from ex8
 */
static void earth(double r){
   const int d=15;
   //  Save transformation
   glPushMatrix();
   //  Scale radius to r
   glScaled(r,r,r);
   //  Latitude bands
   for (int ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      glColor3d(0,0,1);
      for (int th=0;th<=360;th+=d)
      {
         // glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
         // glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
         Vertex(th, ph);
         Vertex(th, ph+d);
      }
      glEnd();
   }
   //  Undo transformations
   glPopMatrix();
}

/*
 * Helper function for getting the direction the camera is facing
 * in vector form
 */
void getCamDir(){
   lookX = 2*dim*Cos(ph)*Sin(th);
   lookY = 2*dim        *Sin(ph);
   lookZ = -2*dim*Cos(ph)*Cos(th);
}
/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   // Get window height for printing (taken from ChatGPT)
   int height = glutGet(GLUT_WINDOW_HEIGHT);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal - set world orientation
   if (!mode)
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
   //  Perspective - set eye position
   else if (mode == 1)
   {
      camX = -2*dim*Sin(th)*Cos(ph);
      camY = +2*dim        *Sin(ph);
      camZ = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(camX,camY,camZ , 0,0,0 , 0,Cos(ph),0);
   }
   // First-person
   else {
      getCamDir();
      // double lookX = 2*dim*Sin(th)*Cos(ph);
      // double lookY = 2*dim        *Sin(ph);
      // double lookZ = -2*dim*Cos(th)*Cos(ph);
      gluLookAt(camX,camY,camZ , lookX,lookY,lookZ , 0,Cos(ph),0);
   }
   

   if (obj == 2)
   {
      //  Draw satellite
      satellite(0,0,0, 1,0,0, 0,1,0, 1);
   } else if (obj == 3) {
      //  Draw Earth
      earth(1);
   } else {
      // Draw whole scene
      // dim = 10;
      earth(5);
      satellite(8*Cos(zh),8*Sin(zh),0, -Cos(zh),-Sin(zh),0, -Sin(zh),Cos(zh),0, 1);
      satellite(-8*Cos(zh),0,8*Sin(zh), Cos(zh),0,-Sin(zh), Sin(zh),0,Cos(zh), 1);
      satellite(0,-8*Sin(zh),8*Cos(zh), 0,Sin(zh),-Cos(zh), 0,-Cos(zh),-Sin(zh), 1);
   }

   //  Draw axes
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(dim,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,dim,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,dim);
      glEnd();
      //  Label axes
      glRasterPos3d(dim,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,dim,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,dim);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,modes[mode]);
   //  Print what's being shown
   glWindowPos2i(5, height-20);
   Print("%s", text[obj-1]);
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if ((ch == 'a' || ch == 'A') && mode != 2)
      axes = 1-axes;
   //  Toggle object display
   else if (ch == '1') {
      obj = 1;
      dim = 10; 
      /* 
       *  Want to find a better way to make sure dimension is updated
       *  before it gets used to set eye position in display
       */
   } else if (ch == '2') {
      obj = 2;
      dim = 1;
   } else if (ch == '3') {
      
      dim = 2;
   //  Switch display mode
   } else if (ch == 'm' || ch == 'M')
      mode = (mode+1)%3;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if ((ch == '+' || ch == '=') && ch<179)
      fov++;
   // Allow first-person camera movement
   if (mode == 2){
      if (ch == 'w' || ch == 'W'){
         getCamDir();
         camX += 0.01*lookX;
         camY += 0.01*lookY;
         camZ += 0.01*lookZ;
      // Move along calculated left vector (help from ChatGPT)
      } else if (ch == 'a' || ch == 'A') {
         getCamDir();
         camX += 0.01*lookZ;
         camZ -= 0.01*lookX;
      } else if (ch == 's' || ch == 'S') {
         getCamDir();
         camX -= 0.01*lookX;
         camY -= 0.01*lookY;
         camZ -= 0.01*lookZ;
      // Move along calculated right vector (help from ChatGPT)
      } else if (ch == 'd' || ch == 'D') {
         getCamDir();
         camX -= 0.01*lookZ;
         camZ += 0.01*lookX;
      }
   }
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Set projection
   Project();
}

/*
 *  GLUT calls this routine when there is nothing else to do
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(10*t,360);
   Project();
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Scott Hardman: Homework 2");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Set callbacks
   glutIdleFunc(idle);
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
