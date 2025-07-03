/*
 *  Scott Hardman - Final Project
 *
 *  Put everything from the semester together and then some
 *
 *  Key bindings
 *  arrows     Change view angle
 *  Space      Stop/Start light source motion
 *  h          Cycle between different shaders
 *  m          Cycles scene being viewed
 *  PgDn/PgUp  Zoom in and out
 *  x          Toggle axes
 *  []         Lower/rise light
 *  0          Reset view angle
 *  p          Toggle display parameters
 *  f          Enter first-person mode
 *  ESC        Exit
 *     (First-person mode)
 *  w          move camera forward
 *  a          move camera left
 *  s          move camera back
 *  d          move camera right
 */
#include "util/CSCIx229.h"
#include "satellite.h"
#include "swordfish2.h"
int axes=0;       //  Display axes
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=10.0;  //  Size of world=

// Textures
unsigned int background_texture;  // NEW Spherical background
unsigned int solar_panel_texture; // Just used for satellite
unsigned int earth_texture;       // Used for HW3's scene


// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
int sh        =  0;   // Sat azimuth
float ylight  =   0;  // Elevation of light
int scene = 0;        // Which scene is being displayed
int fov = 55;         // Field of view / zoom
int move_light = 0;   // Allow light stopping
int fps = 0;          // First person mode toggle
int engine_on=0;      // Toggleable engine
int shade_mode=1;     // Changing shaders
int display_parameters=0;
double asteroid[6];   // Asteroids zipping by

float ys; float ts;   // Add wobble to ship flight

//  First-person camera position
double camX; double camY; double camZ;

// Shaders
int toon_shader;
int gouraud_shader;
int thruster_shader;

// Scene text
const char* text[] = {"SEE YOU SPACE COYBOY...", "Swordfish II", "Homework 3"};
const char* shaders[] = {"None", "Cartoon", "Gouraud"};

/*
 *  Read text file
 *  From ex25
 */
char* ReadText(char *file)
{
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   int n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}

/*
 *  Print Shader Log
 *  From ex25
 */
void PrintShaderLog(int obj,char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error compiling %s\n",file);
}

/*
 *  Print Program Log
 *  From ex25
 */
void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}

/*
 *  Create Shader
 *  From ex25
 */
int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Return name
   return shader;
}

/*
 *  Create Shader Program
 *  From ex25
 */
int CreateShaderProg(char* VertFile,char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   int vert = CreateShader(GL_VERTEX_SHADER,VertFile);
   //  Create and compile fragment shader
   int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
   //  Attach vertex shader
   glAttachShader(prog,vert);
   //  Attach fragment shader
   glAttachShader(prog,frag);
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}

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
 *  Draw a sphere and put the earth map on it
 */
static void earth(double r){
   const int d=15;
   glPushMatrix();
   glScaled(r,r,r);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D, earth_texture);
   for (int ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      glColor3f(1,1,1);
      for (int th=0;th<=360;th+=d)
      {
         glTexCoord2f(th/360.0, (ph+90)/180.0);
         glNormal3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));
         glVertex3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));

         glTexCoord2f(th/360.0, (ph+d+90)/180.0);
         glNormal3d(Sin(th) * Cos(ph + d), Sin(ph + d), Cos(th) * Cos(ph + d));
         glVertex3d(Sin(th) * Cos(ph + d), Sin(ph + d), Cos(th) * Cos(ph + d));
      }
      glEnd();
   }
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

// Crease sense of motion with asteroids whizzing by
void asteroids(){
   ball(asteroid[0], 1, 1, 0.01);
   ball(asteroid[1], -1, -1, 0.01);
   ball(asteroid[2], 1, -1, 0.01);
   ball(asteroid[3], -1, 1, 0.01);
   ball(asteroid[4], 1.5, 0, 0.01);
   ball(asteroid[5], -1.5, 0, 0.01);
}

/*
 *  Giant sphere for background texture
 */
static void background(){
   // No lighting
   glDisable(GL_LIGHTING);
   // High res, looked at a lot
   const int d=5;
   glPushMatrix();
   // Very large
   glScaled(50,50, 50);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D, background_texture);
   for (int ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      glColor3f(1,1,1);
      for (int th=0;th<=360;th+=d)
      {
         glTexCoord2f(th/360.0, (ph+90)/180.0); // Used ChatGPT to helg with TexCoords
         glNormal3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));
         glVertex3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));

         glTexCoord2f(th/360.0, (ph+d+90)/180.0);
         glNormal3d(Sin(th) * Cos(ph + d), Sin(ph + d), Cos(th) * Cos(ph + d));
         glVertex3d(Sin(th) * Cos(ph + d), Sin(ph + d), Cos(th) * Cos(ph + d));
      }
      glEnd();
   }
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
   glEnable(GL_LIGHTING);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   // For printing
   int height = glutGet(GLUT_WINDOW_HEIGHT);
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
      gluLookAt(camX,camY,camZ, Ex,Ey,Ez, 0,Cos(ph),0);
   else
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);

   //  Translate intensity to color vectors
   float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
   float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
   float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
   // Light source location
   float Position[4];

   if (move_light)
   {
      Position[0] = (dim+4)*Cos(zh);
      Position[1] = ylight;
      Position[2] = (dim+4)*Sin(zh);
      Position[3] = 1;
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);

   }
   else
   {
      // Default is to have light coming from apparent source in background
      // (no ball)
      Position[0] = 10;
      Position[1] = 0;
      Position[2] = 0;
      Position[3] = 1;
   }
   
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);

   //  Enable light 0
   glEnable(GL_LIGHT0);
   // Set ambient, diffuse, specular components and position of light 0
   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,Position);

   //  Draw scene
    if (!scene)
   {
      dim = 4;

      // Multiple shader options
      if (shade_mode == 1)
      {
         glUseProgram(toon_shader);
         glUniform1f(glGetUniformLocation(toon_shader, "useTexture"), 0);
         swordfish2(0,ys,0, 1,ts,0, 0,1-ts,ts, 1, engine_on, thruster_shader, toon_shader);
      }
      else if (shade_mode == 2)
      {
         glUseProgram(gouraud_shader);
         glUniform1f(glGetUniformLocation(toon_shader, "useTexture"), 0);
         swordfish2(0,ys,0, 1,ts,0, 0,1-ts,ts, 1, engine_on, thruster_shader, gouraud_shader);
      }
      else
      {
         glUseProgram(0);
         swordfish2(0,ys,0, 1,ts,0, 0,1-ts,ts, 1, engine_on, thruster_shader, 0);
      }

      glUseProgram(0);
      asteroids();
      background();
   } else if (scene == 1) {
      dim = 4;
      swordfish2(0,0,0, 1,0,0, 0,1,0, 1, 0, thruster_shader, 0);
   } else{
      // Draw the satellite scene from HW3
      dim = 12;
      earth(5);
      satellite(8*Cos(sh),8*Sin(sh),0, -Cos(sh),-Sin(sh),0, 
               -Sin(sh),Cos(sh),0, 1, solar_panel_texture);
      satellite(-8*Cos(sh),0,8*Sin(sh), Cos(sh),0,-Sin(sh), 
               Sin(sh),0,Cos(sh), 1, solar_panel_texture);
      swordfish2(0,-8*Sin(sh),8*Cos(sh), 0,-Cos(sh),-Sin(sh), 
               0,-Sin(sh),Cos(sh), 1, 0, thruster_shader, 0);
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

   if (display_parameters)
   {
      //  Display parameters
      glWindowPos2i(5,5);
      Print("Angle=%d,%d  Dim=%.1f  FOV=%d",th,ph,dim, fov);
      glWindowPos2i(5,25);
      Print("Engine=%s  Light=%s",engine_on?"on":"off",move_light?"moving":"still");
   }

   // Different mode text
   glWindowPos2i(5, height-20);
   Print("%s", text[scene]);
   // Print vision mode
   glWindowPos2i(5, height-40);
   Print("%s", shaders[shade_mode]);
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
   // Move ship
   ys = 0.2 * sin(t * 0.5);
   ts = 0.1 * sin(t);
   // Move asteroids
   asteroid[0] = 10-fmod(20*t,20);
   asteroid[1] = 10-fmod(25*t,20);
   asteroid[2] = 10-fmod(30*t,20);
   asteroid[3] = 10-fmod(35*t,20);
   asteroid[4] = 10-fmod(40*t,20);
   asteroid[5] = 10-fmod(45*t,20);
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
   // Start/Stop light on Space
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
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   // Shader mode
   else if (ch=='h')
      shade_mode = (shade_mode+1)%3;
   //Engine
   else if (ch=='e')
      engine_on = 1-engine_on;
   // Display parameters
   else if (ch=='p')
      display_parameters = 1-display_parameters;

   // Turns on first-person
   else if (ch=='f' || ch=='F'){
      camX = -2*dim*Sin(th)*Cos(ph);
      camY = 2*dim*Sin(ph);
      camZ = 2*dim*Cos(th)*Cos(ph);
      fps = 1-fps;
      }
   
   // Enable first-person movement
   if (fps) {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = 2*dim*Sin(ph);
      double Ez = 2*dim*Cos(th)*Cos(ph);
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
   glutCreateWindow("Scott Hardman - Final Project");
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
   background_texture = LoadTexBMP("img/background.bmp");

   toon_shader = CreateShaderProg("base.vert","toon.frag");
   gouraud_shader = CreateShaderProg("base.vert","gouraud.frag");
   thruster_shader = CreateShaderProg("thruster.vert","thruster.frag");

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
