/*
 *  Scott Hardman - Homework 3
 *
 *  Demonstrates using lighting and textures
 *  Expands upon the scene created for homework 2.
 *
 *  Key bindings
 *  Space      Stop/Start light source motion
 *  m/M        Cycles scene being viewed
 * 
 *     (First-person mode)
 *  w          move camera forward
 *  a          move camera left
 *  s          move camera back
 *  d          move camera right
 * 
 *     (Projection Mode)
 *  a/A        decrease/increase ambient light
 *  d/D        decrease/increase diffuse light
 *  s/S        decrease/increase specular light
 *  e/E        decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  []         Lower/rise light
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
#include "satellite.h"
int mode=0;       //  Texture mode
int ntex=0;       //  Cube faces
int axes=1;       //  Display axes
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int light=1;      //  Lighting
int rep=1;        //  Repitition
double asp=1;     //  Aspect ratio
double dim=10.0;   //  Size of world
// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;    // Shininess (value)
int zh        =  90;  // Light azimuth
int sh        =  0;    // Sat azimuth
float ylight  =   0;  // Elevation of light
int scene = 0;       // Which scene is being displayed
int fov = 55;        // Field of view / zoom
int move_light = 1; // Allow light stopping
int fps = 0;         // First person mode?
unsigned int solar_panel_texture; // Loaded solar panel bmp
unsigned int earth_texture;       // Loaded earth bmp

//  First-person camera position
double camX; double camY; double camZ;

const char* text[] = {"Scene", "Satellite", "Earth"};

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r
 */
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
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
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   //  Latitude bands
   glBindTexture(GL_TEXTURE_2D, earth_texture);
   for (int ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      glColor3f(0.5, 0.5, 0.5);
      for (int th=0;th<=360;th+=d)
      {
         glTexCoord2f(th/360.0, (ph+90)/180.0); // Used ChatGPT to help get math
         glNormal3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));
         glVertex3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));

         glTexCoord2f(th/360.0, (ph+d+90)/180.0); // For TexCoords
         glNormal3d(Sin(th) * Cos(ph + d), Sin(ph + d), Cos(th) * Cos(ph + d));
         glVertex3d(Sin(th) * Cos(ph + d), Sin(ph + d), Cos(th) * Cos(ph + d));
      }
      glEnd();
   }
   glDisable(GL_TEXTURE_2D);

   //  Undo transformations
   glPopMatrix();
}

/*
 * Helper function for getting the direction the camera is facing
 * in vector form
 */
// double* getCamDir(){
//    return [, 2*dim*Sin(ph), -2*dim*Cos(ph)*Cos(th)];
// }

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   int height = glutGet(GLUT_WINDOW_HEIGHT);
   // Start: perspective and lighting code ripped from ex15
   //  Eye position
   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Set perspective
   glLoadIdentity();
   // Allow first-person mode
   if (fps)
      gluLookAt(camX,camY,camZ, -Ex,Ey,-Ez, 0,Cos(ph),0);
   else
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);

   //  Translate intensity to color vectors
   float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
   float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
   float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
   //  Light direction
   float Position[]  = {(dim+4)*Cos(zh),ylight,(dim+4)*Sin(zh),1};
   //  Draw light position as ball (still no lighting here)
   glColor3f(1,1,1);
   ball(Position[0],Position[1],Position[2] , 0.1);
   //  OpenGL should normalize normal vectors
   glEnable(GL_NORMALIZE);
   //  Enable lighting
   glEnable(GL_LIGHTING);
   //  glColor sets ambient and diffuse color materials
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
   //  Enable light 0
   glEnable(GL_LIGHT0);
   //  Set ambient, diffuse, specular components and position of light 0
   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,Position);
   // End: perspective and lighting code ripped from ex15


   //  Draw scene
    if (!scene)
   {
      dim = 12;
      earth(5);
      satellite(8*Cos(sh),8*Sin(sh),0, -Cos(sh),-Sin(sh),0, 
               -Sin(sh),Cos(sh),0, 1, solar_panel_texture);
      satellite(-8*Cos(sh),0,8*Sin(sh), Cos(sh),0,-Sin(sh), 
               Sin(sh),0,Cos(sh), 1, solar_panel_texture);
      satellite(0,-8*Sin(sh),8*Cos(sh), 0,Sin(sh),-Cos(sh), 
               0,-Cos(sh),-Sin(sh), 1, solar_panel_texture);
      //  Draw satellite
   } else if (scene == 1) {
      dim = 1;
      satellite(0,0,0, 1,0,0, 0,1,0, 1, solar_panel_texture);
   } else {
      dim = 2;
      earth(1);
   }  
   
   // Axes code is the same as always
   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      const double len=2.0; //  Length of axes
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f  FOV=%d",th,ph,dim, fov);
   glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   //  Print what's being shown
   glWindowPos2i(5, height-20);
   Print("%s", text[scene]);
   // Print vision mode
   glWindowPos2i(5, height-40);
   Print("%s", fps?"First-Person":"Projection");
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   if (move_light) zh = fmod(90*t,360.0);
   sh = fmod(15*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
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
   //  PageUp key - increase fov
   else if (key == GLUT_KEY_PAGE_DOWN)
      fov += 1;
   //  PageDown key - decrease fov
   else if (key == GLUT_KEY_PAGE_UP)
      fov -= 1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(fov,asp,dim);
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
   // Stop light on Space
   else if (ch == 32)
      move_light = 1-move_light;
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   else if (ch == 'm' || ch == 'M')
      scene = (scene+1)%3;
   //  Toggle textures mode
   else if (ch == 't')
      ntex = 1-ntex;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Repitition
   else if (ch=='+')
      rep++;
   else if (ch=='-' && rep>1)
      rep--;
   else if (ch=='f' || ch=='F'){
      camX = -2*dim*Sin(th)*Cos(ph);
      camY = 2*dim*Sin(ph);
      camZ = 2*dim*Cos(th)*Cos(ph);
      fps = 1-fps;
      }
   
   // Enable first-person movement
   if (fps) {
      double Ex = 2*dim*Sin(th)*Cos(ph);
      double Ey = 2*dim*Sin(ph);
      double Ez = -2*dim*Cos(th)*Cos(ph);
      if (ch == 'w' || ch == 'W'){
         camX += 0.01*Ex;
         camY += 0.01*Ey;
         camZ += 0.01*Ez;
      // Move along calculated left vector (help from ChatGPT)
      } else if (ch == 'a' || ch == 'A') {
         camX += 0.01*Ez;
         camZ -= 0.01*Ex;
      } else if (ch == 's' || ch == 'S') {
         camX -= 0.01*Ex;
         camY -= 0.01*Ey;
         camZ -= 0.01*Ez;
      // Move along calculated right vector (help from ChatGPT)
      } else if (ch == 'd' || ch == 'D') {
         camX -= 0.01*Ez;
         camZ += 0.01*Ex;
      }
   } else {
      //  Ambient level
      if (ch=='a' && ambient>0)
         ambient -= 5;
      else if (ch=='A' && ambient<100)
         ambient += 5;
      //  Diffuse level
      else if (ch=='d' && diffuse>0)
         diffuse -= 5;
      else if (ch=='D' && diffuse<100)
         diffuse += 5;
      //  Specular level
      else if (ch=='s' && specular>0)
         specular -= 5;
      else if (ch=='S' && specular<100)
         specular += 5;
   }
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(fov,asp,dim);
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
   Project(fov,asp,dim);
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
   glutCreateWindow("Scott Hardman - Homework 3");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load textures
   solar_panel_texture = LoadTexBMP("img/solar_panel.bmp");
   earth_texture = LoadTexBMP("img/earth.bmp");

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
