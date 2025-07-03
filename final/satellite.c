// Scott Hardman
#include "util/CSCIx229.h"
#include "satellite.h"

// Draw a solar panel for a satellite
void panel(double start, double stop, double width, 
    double height, int flip, unsigned int solar_panel_texture)
{
    glColor3f(0.6, 0.6, 0.6);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Top face (textured)
    glBindTexture(GL_TEXTURE_2D, solar_panel_texture);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3d(-width, height, start);
    glTexCoord2f(0, 1); glVertex3d(-width, height, stop);
    glTexCoord2f(1, 1); glVertex3d(width, height, stop);
    glTexCoord2f(1, 0); glVertex3d(width, height, start);
    glEnd();

    // Bottom face (textured)
    glBindTexture(GL_TEXTURE_2D, solar_panel_texture);
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
    glTexCoord2f(0, 0); glVertex3d(-width, -height, start);
    glTexCoord2f(0, 1); glVertex3d(-width, -height, stop);
    glTexCoord2f(1, 1); glVertex3d(width, -height, stop);
    glTexCoord2f(1, 0); glVertex3d(width, -height, start);
    glEnd();

    // Front
    glBindTexture(GL_TEXTURE_2D, solar_panel_texture);
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3d(width, height, start);
    glVertex3d(width, height, stop);
    glVertex3d(width, -height, stop);
    glVertex3d(width, -height, start);
    glEnd();

    // Back
    glBindTexture(GL_TEXTURE_2D, solar_panel_texture);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3d(-width, height, start);
    glTexCoord2f(0, 1); glVertex3d(-width, height, stop);
    glTexCoord2f(1, 1); glVertex3d(-width, -height, stop);
    glTexCoord2f(1, 0); glVertex3d(-width, -height, start);
    glEnd();

    // Inside
    glBindTexture(GL_TEXTURE_2D, solar_panel_texture);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, flip);
    glTexCoord2f(0, 0); glVertex3d(-width, height, stop);
    glTexCoord2f(0, 1); glVertex3d(width, height, stop);
    glTexCoord2f(1, 1); glVertex3d(width, -height, stop);
    glTexCoord2f(1, 0); glVertex3d(-width, -height, stop);
    glEnd();

    // Outside
    glBindTexture(GL_TEXTURE_2D, solar_panel_texture);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -flip);
    glTexCoord2f(0, 0); glVertex3d(-width, height, start);
    glTexCoord2f(0, 1); glVertex3d(width, height, start);
    glTexCoord2f(1, 1); glVertex3d(width, -height, start);
    glTexCoord2f(1, 0); glVertex3d(-width, -height, start);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}


/*
 *  Draw a satellite
 *     at (x,y,z)
 *     nose towards (dx,dy,dz)
 *     up towards (ux,uy,uz)
 *     size
 *  Designed referencing SolidPlane from ex8
 */
void satellite(double x, double y,  double z,
                      double dx, double dy, double dz,
                      double ux, double uy, double uz,
                      double s, unsigned int solar_panel_texture)
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

   // Make it shiny
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,1);

   // Inner body
   glColor3f(0.5,0.5,0.5);
   // Double-sided for receiver
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
   glBegin(GL_QUAD_STRIP);
   for (int th=0;th<=360;th+=30)
   {
      // Satellite body is just cylinders
      // Strategy for defining normals from ex13
      glNormal3d(0,Cos(th),Sin(th));
      glVertex3d(nose,inner*Cos(th),inner*Sin(th));
      glVertex3d(tail,inner*Cos(th),inner*Sin(th));
   }
   glEnd();
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);

   // Close back
   glBegin(GL_TRIANGLE_FAN);
   glNormal3d(-1,0,0);
   glVertex3d(tail, 0.0, 0.0);
   for (int th=0;th<=360;th+=30)
      glVertex3d(tail,inner*Cos(th),inner*Sin(th));
   glEnd();

   //  Front outer body
   glColor3f(0.7,0.7,0.7);
   glBegin(GL_QUAD_STRIP);
   for (int th=0;th<=360;th+=30)
   {
      glNormal3d(0,Cos(th),Sin(th));
      glVertex3d(nose-nose/5, outer*Cos(th), outer*Sin(th));
      glVertex3d(nose/5, outer*Cos(th), outer*Sin(th));
   }
   glEnd();   
   // Close body
   glBegin(GL_TRIANGLE_FAN);
   glNormal3d(1,0,0);
   glVertex3d(nose-nose/5, 0.0, 0.0);
   for (int th=0;th<=360;th+=30)
      glVertex3d(nose-nose/5,outer*Cos(th),outer*Sin(th));
   glEnd();
   glBegin(GL_TRIANGLE_FAN);
   glNormal3d(-1,0,0);
   glVertex3d(nose/5, 0.0, 0.0);
   for (int th=0;th<=360;th+=30)
      glVertex3d(nose/5,outer*Cos(th),outer*Sin(th));
   glEnd();

   // Back outer body
   glBegin(GL_QUAD_STRIP);
   for (int th=0;th<=360;th+=30)
   {
      glNormal3d(0,Cos(th),Sin(th));
      glVertex3d(tail/5, outer*Cos(th), outer*Sin(th));
      glVertex3d(tail-tail/5, outer*Cos(th), outer*Sin(th));
   }
   glEnd();   
   // Close body
   glBegin(GL_TRIANGLE_FAN);
   glNormal3d(1,0,0);
   glVertex3d(tail/5, 0.0, 0.0);
   for (int th=0;th<=360;th+=30)
      glVertex3d(tail/5,outer*Cos(th),outer*Sin(th));
   glEnd();
   glBegin(GL_TRIANGLE_FAN);
   glNormal3d(-1,0,0);
   glVertex3d(tail-tail/5, 0.0, 0.0);
   for (int th=0;th<=360;th+=30)
      glVertex3d(tail-tail/5,outer*Cos(th),outer*Sin(th));
   glEnd();

   //  Panels
   panel(pstart, pstop, pwid, pheight, 1, solar_panel_texture);
   panel(-pstart, -pstop, pwid, pheight, -1, solar_panel_texture);
   glBegin(GL_QUAD_STRIP); 
   // Connect panels to body
   glColor3f(0.3,0.3,0.3);
   for (int th=0;th<=360;th+=30)
   {
      glNormal3d(0,Cos(th),Sin(th));
      glVertex3d(inner/2*Cos(th),inner/2*Sin(th),pstart);
      glVertex3d(inner/2*Cos(th),inner/2*Sin(th),-pstart);
   }
   glEnd();

    //  Dish - just  half a sphere which means...
    //  we can use the fun trick where normals are just vectors!
   glPushMatrix();
   glTranslated(nose+dish/2,0,0);
   glRotatef(90,0,0,1);
   glScaled(dish,dish,dish);
   glColor3f(0.4, 0.4, 0.4);
   // Using two sided mode
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
   for (int ph=0;ph<90;ph+=30)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=30)
      {
        // Turns out two sided mode flipped the sides so I have to flip them first here
         glNormal3d(-Sin(th)*Cos(ph) , -Sin(ph) , -Cos(th)*Cos(ph));
         glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
         glVertex3d(Sin(th)*Cos(ph+30) , Sin(ph+30) , Cos(th)*Cos(ph+30));
      }
      glEnd();
   }
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);

   glPopMatrix();
   glPopMatrix();
}