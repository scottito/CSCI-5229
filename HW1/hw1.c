/*
 *  Homework 1: Lorenz Attractor
 *  Scott Hardman 
 * 
 *  Display the Lorenz Attractor in 3D.
 *
 *  This file makes use of code from ex6, ex8, and lorenz.c
 * 
 *  Key bindings:
 *  1      Modify s
 *  2      Modify b
 *  3      Modify r
 *  +/-    Increase/decrease s, b, or r
 *  arrows Change view angle
 *  0      Reset view angle
 *  ESC    Exit
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

//  Globals
int th=0;       // Azimuth of view angle
int ph=0;       // Elevation of view angle

/*  Lorenz Parameters  */
double s  = 8;
double b  = 1;
double r  = 15;
/*  Time step  */
double dt = 0.001;
int steps = 100000;
double** coords; // Lorenz Attractor points

const char param[] = {'s', 'b', 'r'};
double* val[] = {&s, &b, &r};

int mode=0;     // Parameter to modify
double dim=2;   // Dimension of orthogonal box

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  // Maximum length of text string
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
 *  Check for OpenGL errors
 */
void ErrCheck(const char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

double** lorenz(double s, double b, double r, int steps)
{
    // Allocate memory for array of pointers (rows)
    double** coords = (double**)malloc(steps * sizeof(double*));
    if (!coords) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    
    // Allocate memory for each row (3 columns: x, y, z)
    for (int i = 0; i < steps; i++) {
        coords[i] = (double*)malloc(3 * sizeof(double));
        if (!coords[i]) {
            // Free previously allocated memory on failure
            for (int j = 0; j < i; j++) {
                free(coords[j]);
            }
            free(coords);
            printf("Memory allocation failed!\n");
            return NULL;
        }
    }

    /* Initial Coordinates  */
    double x = -0.5;
    double y = 0.1;
    double z = 0.1;

    // Store initial condition
    coords[0][0] = x;
    coords[0][1] = y;
    coords[0][2] = z;

   /*
    *  Integrate steps
    *  Explicit Euler integration
    */
   int i;
   for (i=1;i<steps;i++)
   {
        double dx = s*(y-x);
        double dy = x*(r-z)-y;
        double dz = x*y - b*z;
        x += dt*dx;
        y += dt*dy;
        z += dt*dz;

        // Store coordinates
        coords[i][0] = x;
        coords[i][1] = y;
        coords[i][2] = z;
    }
    return coords;
}

// Helper function to free the allocated memory
void free_lorenz_array(double** coords, int steps)
{
    if (coords) {
        for (int i = 0; i < steps; i++) {
            free(coords[i]);
        }
        free(coords);
    }
}

void display()
{
    //  Clear the image
    glClear( GL_COLOR_BUFFER_BIT );
    //  Reset previous transforms
    glLoadIdentity();
    //  Set view angle
    glRotated(ph,1,0,0);
    glRotated(th,0,1,0);

    // Make tail cyan-ish
    glColor3f(0,0.3,0.7);
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<steps-100; i++){
        glVertex4d(coords[i][0], coords[i][1], coords[i][2], 10);
    }
    // Draw leading points in red
    glColor3f(0.9,0,0.1);
    for(int i=steps-100; i<steps; i++){
        glVertex4d(coords[i][0], coords[i][1], coords[i][2], 10);
    }
    glEnd();

    //  Draw axes in white
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    glVertex3d(0,0,0);
    glVertex3d(1,0,0);
    glVertex3d(0,0,0);
    glVertex3d(0,1,0);
    glVertex3d(0,0,0);
    glVertex3d(0,0,1);
    glEnd();
    //  Label axes
    glRasterPos3d(1,0,0);
    Print("X");
    glRasterPos3d(0,1,0);
    Print("Y");
    glRasterPos3d(0,0,1);
    Print("Z");
    //  Display parameters
    glWindowPos2i(5,5);
    Print("View Angle=%d,%d %c=%.1f",th,ph,param[mode],*val[mode]);
    //  Flush and swap
    ErrCheck("display");
    glFlush();
    glutSwapBuffers();
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
    else if (ch == '1')
        mode = 0;
    else if (ch == '2')
        mode = 1;
    else if (ch == '3')
        mode = 2;
    else if (ch == '-' || ch == '_') {
        switch (mode)
        {
            case 0:
                s -= 0.1;
            case 1:
                b -= 0.1;
            case 2:
                r -= 0.1;
        }
        coords = lorenz(s, b, r, steps);
    }
    else if (ch == '+' || ch == '=') {
        switch (mode)
        {
            case 0:
                s += 0.1;
            case 1:
                b += 0.1;
            case 2:
                r += 0.1;
        }
        // Recalculate attractor
        coords = lorenz(s, b, r, steps);
    }
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   // Always use aspect ratio of 1.0 (square)
   //  Orthogonal projection box adjusted for the
   //  aspect ratio of the window
   double asp = (height>0) ? (double)width/height : 1;
   glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  GLUT calls this routine when there is nothing else to do
 *  We will use this to show the attractor appearing over time
 */
void idle()
{
   // Calculate next lorenz step
   double x = coords[steps-1][0];
   double y = coords[steps-1][1];
   double z = coords[steps-1][2];
   double dx = s*(y-x);
    double dy = x*(r-z)-y;
    double dz = x*y - b*z;
    x += dt*dx;
    y += dt*dy;
    z += dt*dz;
    // Here and everywhere else I had Claude help me use malloc because I forgot the syntax
   double* new_coord = (double*)malloc(3 * sizeof(double));
    if (!new_coord) {
        printf("Memory allocation failed!\n");
    }
   new_coord[0] = x;
   new_coord[1] = y;
   new_coord[2] = z;

   // Allocate memory for array of pointers (rows)
    double** new_coords = (double**)malloc(steps * sizeof(double*));
    if (!new_coords) {
        printf("Memory allocation failed!\n");
    }
    
    // Allocate memory for each row (3 columns: x, y, z)
    for (int i = 0; i < steps; i++) {
        new_coords[i] = (double*)malloc(3 * sizeof(double));
        if (!new_coords[i]) {
            // Free previously allocated memory on failure
            for (int j = 0; j < i; j++) {
                free(new_coords[j]);
            }
            free(new_coords);
            printf("Memory allocation failed!\n");
        }
    }

    // Copy all elements except the first
    for (int i = 1; i < steps; i++) {
        new_coords[i-1] = coords[i];
    }
    // Add new coordinate to end
    new_coords[steps-1] = new_coord;
    coords = new_coords;

   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
  //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   // Calculate Lorenz Attractor points
    coords = lorenz(s,b,r, steps);

   //  Request double buffered, true color window 
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   //  Request 1200 x 900 pixel window
   glutInitWindowSize(1200,900);

   //  Create the window
   // Title should be your name
   glutCreateWindow("Scott Hardman");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   free_lorenz_array(coords, steps);
   return 0;
}
