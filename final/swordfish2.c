// Scott Hardman
// The Swordfish II from Cowboy Bebop
#include "util/CSCIx229.h"
#include "swordfish2.h"

/*
 *  Wings
 *  The whole ship was way too big to be just one function
 *  Code separation is the only purpose for this
 */
void wings(double nose_r, double inner, double wing_length, double stabilizer)
{
    // I thought an easier way to do roundedwings might be to just squash some cylinders
    // Right Wing
    glPushMatrix();
    glTranslated(0,0,nose_r);
    glScaled(4,1,1);
    glColor3f(0.8,0,0);
    glBegin(GL_QUAD_STRIP);
    for (int th=0;th<=360;th+=30)
    {
        glNormal3d(Cos(th),Sin(th), 0);
        glVertex3d(inner*Cos(th),inner*Sin(th), wing_length);
        glVertex3d(inner*Cos(th),inner*Sin(th), -0.1); // Slight intersect with nose cone
    } 
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(0,0,1);
    glVertex3d(0, 0, wing_length);
    for (int th=0;th<=360;th+=30)
        glVertex3d(inner*Cos(th),inner*Sin(th), wing_length);
    glEnd();
    glPopMatrix();

    // Right Wing Tip
    glPushMatrix();
    glTranslated(0,0,nose_r+wing_length);
    glScaled(0.5,inner,0.1);
    glColor3f(0.8,0,0);
    glBegin(GL_TRIANGLES);
    for (int th=180;th<360;th+=30)
    {
        glNormal3f(1,Cos(th),Sin(th));
        glVertex3f(0,Cos(th),Sin(th));
        glNormal3f(1,Cos(th+15),Sin(th+15));
        glVertex3f(1,0,0);
        glNormal3f(1,Cos(th+30),Sin(th+30));
        glVertex3f(0,Cos(th+30),Sin(th+30));
    }
    glNormal3f(0,0,1);
    glVertex3f(0,-1,0);
    glNormal3f(0,0,1);
    glVertex3f(1,0,0);
    glNormal3f(0,0,1);
    glVertex3f(0,1,0);
    glEnd();
    glPopMatrix();

    // Right wing stabilizer
    glPushMatrix();
    glTranslated(0,0,nose_r+wing_length-stabilizer);
    glScaled(1,3,1);
    glColor3f(0.8,0,0);
    glBegin(GL_QUAD_STRIP);
    for (int th=0;th<=180;th+=30)
    {
        glNormal3d(0,Cos(th),Sin(th));
        glVertex3d(stabilizer, stabilizer*Cos(th),stabilizer*Sin(th));
        glVertex3d(-stabilizer, stabilizer*Cos(th),stabilizer*Sin(th));
    }
    // Close arch
    glNormal3d(0,0,-1);
    glVertex3d(stabilizer, stabilizer, 0);
    glVertex3d(-stabilizer, stabilizer, 0);
    glNormal3d(0,0,-1);
    glVertex3d(stabilizer, -stabilizer, 0);
    glVertex3d(-stabilizer, -stabilizer, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(1,0,0);
    glVertex3d(stabilizer, 0.0, 0.0);
    for (int th=0;th<=180;th+=30)
        glVertex3d(stabilizer,stabilizer*Cos(th),stabilizer*Sin(th));
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(-1,0,0);
    glVertex3d(-stabilizer, 0.0, 0.0);
    for (int th=0;th<=180;th+=30)
        glVertex3d(-stabilizer,stabilizer*Cos(th),stabilizer*Sin(th));
    glEnd();
    glPopMatrix();
    
    // Left Wing
    glPushMatrix();
    glTranslated(0,0,-nose_r);
    glScaled(4,1,1);
    glColor3f(0.8,0,0);
    glBegin(GL_QUAD_STRIP);
    for (int th=0;th<=360;th+=30)
    {
        glNormal3d(Cos(th),Sin(th),0);
        glVertex3d(inner*Cos(th),inner*Sin(th), -wing_length);
        glVertex3d(inner*Cos(th),inner*Sin(th), 0.1); 
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(0,0,-1);
    glVertex3d(0, 0, -wing_length);
    for (int th=0;th<=360;th+=30)
        glVertex3d(inner*Cos(th),inner*Sin(th), -wing_length);
    glEnd();
    glPopMatrix();

    // Left Wing Tip
    glPushMatrix();
    glTranslated(0,0,-nose_r-wing_length);
    glScaled(0.5,inner,0.1);
    glColor3f(0.8,0,0);
    glBegin(GL_TRIANGLES);
    for (int th=0;th<180;th+=30)
    {
        glNormal3f(1,Cos(th),Sin(th));
        glVertex3f(0,Cos(th),Sin(th));
        glNormal3f(1,Cos(th+15),Sin(th+15));
        glVertex3f(1,0,0);
        glNormal3f(1,Cos(th+30),Sin(th+30));
        glVertex3f(0,Cos(th+30),Sin(th+30));
    }
    glNormal3f(0,0,-1);
    glVertex3f(0,1,0);
    glNormal3f(0,0,-1);
    glVertex3f(1,0,0);
    glNormal3f(0,0,-1);
    glVertex3f(0,-1,0);
    glEnd();
    glPopMatrix();

    // Left wing stabilizer
    glPushMatrix();
    glTranslated(0,0,-nose_r-wing_length+stabilizer);
    glScaled(1,3,1);
    glColor3f(0.8,0,0);
    glBegin(GL_QUAD_STRIP);
    for (int th=180;th<=360;th+=30)
    {
        glNormal3d(0,Cos(th),Sin(th));
        glVertex3d(stabilizer, stabilizer*Cos(th),stabilizer*Sin(th));
        glVertex3d(-stabilizer, stabilizer*Cos(th),stabilizer*Sin(th));
    }
    // Close arch
    glNormal3d(0,0,1);
    glVertex3d(stabilizer, stabilizer, 0);
    glVertex3d(-stabilizer, stabilizer, 0);
    glNormal3d(0,0,1);
    glVertex3d(stabilizer, -stabilizer, 0);
    glVertex3d(-stabilizer, -stabilizer, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(1,0,0);
    glVertex3d(stabilizer, 0.0, 0.0);
    for (int th=180;th<=360;th+=30)
        glVertex3d(stabilizer,stabilizer*Cos(th),stabilizer*Sin(th));
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(-1,0,0);
    glVertex3d(-stabilizer, 0.0, 0.0);
    for (int th=180;th<=360;th+=30)
        glVertex3d(-stabilizer,stabilizer*Cos(th),stabilizer*Sin(th));
    glEnd();
    glPopMatrix();
}

/*
 *  Cannon
 *  The Swordfish II's signature cannon is an iconic part of the design
 */
void cannon(double nose, double nose_r, double cannon_w)
{
    // Cannon connection point is a sphere
    glPushMatrix();
    glTranslated(0,-nose_r/2,0);
    glRotated(90,0,1,0);
    glScaled(0.2,0.2,0.2);
    for (int ph=-90;ph<90;ph+=30)
    {
        glBegin(GL_QUAD_STRIP);
        glColor3f(0.8,0,0);
        for (int th=30;th<=330;th+=30)
        {
            glNormal3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));
            glVertex3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));

            glNormal3d(Sin(th) * Cos(ph + 30), Sin(ph + 30), Cos(th) * Cos(ph + 30));
            glVertex3d(Sin(th) * Cos(ph + 30), Sin(ph + 30), Cos(th) * Cos(ph + 30));
        }
        glEnd();
    }
    // Grey cannon mounting point
    for (int ph=-90;ph<90;ph+=30)
    {
        glBegin(GL_QUAD_STRIP);
        glColor3f(0.3,0.3,0.3);
        for (int th=-30;th<=30;th+=30)
        {
            glNormal3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));
            glVertex3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));

            glNormal3d(Sin(th) * Cos(ph + 30), Sin(ph + 30), Cos(th) * Cos(ph + 30));
            glVertex3d(Sin(th) * Cos(ph + 30), Sin(ph + 30), Cos(th) * Cos(ph + 30));
        }
        glEnd();
    }
    glPopMatrix();

    // Cannon barrel
    glPushMatrix();
    glTranslated(0,-nose_r/2,0);
    glColor3f(0.3,0.3,0.3);
    glBegin(GL_QUAD_STRIP);
    for (int th=0;th<=360;th+=30)
    {
        glNormal3d(0,Cos(th),Sin(th));
        glVertex3d(1.5*nose,cannon_w*Cos(th),cannon_w*Sin(th));
        glVertex3d(0.1,cannon_w*Cos(th),cannon_w*Sin(th));
    } 
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(1,0,0);
    glVertex3d(1.4*nose, 0, 0);
    for (int th=0;th<=360;th+=30)
        glVertex3d(1.4*nose,cannon_w*Cos(th),cannon_w*Sin(th));
    glEnd();
    glPopMatrix();
}

/*
 *  Thrusters
 */
void thrusters(double cockpit, int engine_on, int thruster_shader)
{
    // Spherical connection to main body
    glPushMatrix();
    glTranslated(-cockpit,0,0);
    glScaled(0.3,0.22,0.21);
    for (int ph=-90;ph<90;ph+=30)
    {
        glBegin(GL_QUAD_STRIP);
        glColor3f(0.3,0.3,0.3);
        for (int th=0;th<=360;th+=30)
        {
            glNormal3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));
            glVertex3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));

            glNormal3d(Sin(th) * Cos(ph + 30), Sin(ph + 30), Cos(th) * Cos(ph + 30));
            glVertex3d(Sin(th) * Cos(ph + 30), Sin(ph + 30), Cos(th) * Cos(ph + 30));
        }
        glEnd();
    }
    glPopMatrix();

    // Rear jet is also spherical but with an eyelid-like opening
    glPushMatrix();
    glTranslated(-1,0,0);
    glRotated(90,1,0,0);
    glScaled(0.5,0.3,0.3);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    for (int ph=-90;ph<90;ph+=30)
    {
        glBegin(GL_QUAD_STRIP);
        glColor3f(0.3,0.3,0.3);
        for (int th=-30;th<=210;th+=30)
        {
            glNormal3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));
            glVertex3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));

            glNormal3d(Sin(th) * Cos(ph + 30), Sin(ph + 30), Cos(th) * Cos(ph + 30));
            glVertex3d(Sin(th) * Cos(ph + 30), Sin(ph + 30), Cos(th) * Cos(ph + 30));
        }
        glEnd();
    }
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
    glPopMatrix();

    // Thrusters have a red ring aroundt the rear
    // Cylinder to main engine
    glPushMatrix();
    glTranslated(-1,0,0);
    glScaled(0.2,0.3,0.3);
    glColor3f(0.8,0,0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    glBegin(GL_QUAD_STRIP);
    for (int th=0;th<=360;th+=30)
    {
        glNormal3d(0,Cos(th),Sin(th));
        glVertex3d(1,Cos(th),Sin(th));
        glVertex3d(0,Cos(th),Sin(th));
    } 
    glEnd();
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
    glColor3f(0.8,0,0);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(1,0,0);
    glVertex3d(1,0,0);
    for (int th=0;th<=360;th+=30)
        glVertex3d(1,Cos(th),Sin(th));
    glEnd();
    glColor3f(0.3,0.3,0.3);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(-1,0,0);
    glVertex3d(0,0,0);
    for (int th=0;th<=360;th+=30)
        glVertex3d(0,Cos(th),Sin(th));
    glEnd();
    glPopMatrix();

    // Cylinder to main engine
    glPushMatrix();
    glTranslated(-1.1,0,0);
    glScaled(0.7,0.15,0.15);
    glColor3f(0.3,0.3,0.3);
    glBegin(GL_QUAD_STRIP);
    for (int th=0;th<=360;th+=30)
    {
        glNormal3d(0,Cos(th),Sin(th));
        glVertex3d(1,Cos(th),Sin(th));
        glVertex3d(0,Cos(th),Sin(th));
    } 
    glEnd();

    // 3... 2... 1...
    // LET'S JAM
    if (engine_on)
    {
        float ambient[]  = {0.2, 0.2, 0.2, 1.0};
        float diffuse[]  = {1.0, 1.0, 1.0, 1.0};
        float specular[] = {1.0, 1.0, 1.0, 1.0};
        float thruster_vertex[] = {0, 0, 0, 1};

        glEnable(GL_LIGHT1);
        glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
        glLightfv(GL_LIGHT1,GL_POSITION,thruster_vertex);
        glUseProgram(thruster_shader); 
        glColor4f(1,1,1,0.5);
        glBegin(GL_TRIANGLES);
        for (int th=0;th<360;th+=30)
        {
            glNormal3f(1,Cos(th),Sin(th));
            glVertex3f(1,Cos(th),Sin(th));
            glNormal3f(1,Cos(th+15),Sin(th+15));
            glVertex3f(-1,0,0);
            glNormal3f(1,Cos(th+30),Sin(th+30));
            glVertex3f(1,Cos(th+30),Sin(th+30));
        }
        glEnd();
    }
    else
        glDisable(GL_LIGHT1);

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.8,1,1);
    glNormal3d(-1,0,0);
    glVertex3d(0.1,0,0);
    for (int th=0;th<=360;th+=30)
        glVertex3d(0.1,Cos(th),Sin(th));
    glEnd();
    glPopMatrix();
}

/*
 *  Draw a the Swordfish II
 *     at (x,y,z)
 *     nose towards (dx,dy,dz)
 *     up towards (ux,uy,uz)
 *     size
 *  Designed referencing SolidPlane from ex8
 */
void swordfish2(double x, double y,  double z, double dx, double dy, double dz, double ux, double uy, double uz, double s, int engine_on, int thruster_shader, int shader)
{
    // Dimensions used to size satellite
    const double nose=1.2;
    const double nose_r=0.3;

    const double cannon_w=0.025;
    const double wing_length=1;
    const double inner=0.03; // Wing thickness in y
    const double stabilizer=0.1;
    const double cockpit=0.2;

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

    // For loading Spike
    unsigned int spike_texture;


    glPushMatrix();
    glTranslated(x,y,z);
    glMultMatrixd(mat);
    glScaled(s,s,s);

    // Make it shiny
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,1);

    /*
    *  Nose Cone
    *  An example of how to do this instead of GL_TRIANGLE_FAN in ex13
    */
    // Top of nose
    glPushMatrix();
    glTranslated(-0.1,0,0);
    glScaled(nose,0.2,nose_r);
    glColor3f(0.8,0,0);
    glBegin(GL_TRIANGLES); // Front
    for (int th=-90;th<90;th+=30)
    {
        glNormal3f(1,Cos(th),Sin(th));
        glVertex3f(0,Cos(th),Sin(th));
        glNormal3f(1,Cos(th+15),Sin(th+15));
        glVertex3f(1,0,0);
        glNormal3f(1,Cos(th+30),Sin(th+30));
        glVertex3f(0,Cos(th+30),Sin(th+30));
    }
    glEnd();
    glBegin(GL_TRIANGLES); // Back
    for (int th=-90;th<90;th+=30)
    {
        glNormal3f(-1,Cos(th),Sin(th));
        glVertex3f(0,Cos(th),Sin(th));
        glNormal3f(-1,Cos(th+15),Sin(th+15));
        glVertex3f(-0.1,0,0);
        glNormal3f(-1,Cos(th+30),Sin(th+30));
        glVertex3f(0,Cos(th+30),Sin(th+30));
    }
    glEnd();
    glPopMatrix();

    // Bottom of nose
    glPushMatrix();
    glTranslated(-0.1,0,0);
    glScaled(nose,0.1,nose_r);
    glColor3f(0.8,0,0);
    glBegin(GL_TRIANGLES); // Front
    for (int th=90;th<270;th+=30)
    {
        glNormal3f(1,Cos(th),Sin(th));
        glVertex3f(0,Cos(th),Sin(th));
        glNormal3f(1,Cos(th+15),Sin(th+15));
        glVertex3f(1,0,0);
        glNormal3f(1,Cos(th+30),Sin(th+30));
        glVertex3f(0,Cos(th+30),Sin(th+30));
    }
    glEnd();
    glBegin(GL_TRIANGLES); // Back
    for (int th=90;th<270;th+=30)
    {
        glNormal3f(-1,Cos(th),Sin(th));
        glVertex3f(0,Cos(th),Sin(th));
        glNormal3f(-1,Cos(th+15),Sin(th+15));
        glVertex3f(-0.1,0,0);
        glNormal3f(-1,Cos(th+30),Sin(th+30));
        glVertex3f(0,Cos(th+30),Sin(th+30));
    }
    glEnd();
    glPopMatrix();

    // Wings and things
    wings(nose_r, inner, wing_length, stabilizer);
    cannon(nose, nose_r, cannon_w);

    /*
     * Cockpit
     */
    glPushMatrix();
    glTranslated(-cockpit,cockpit/2,0);
    glScaled(1.5,1,1);
    glColor3f(0.8,0,0);
    glBegin(GL_QUAD_STRIP);
    for (int th=0;th<=360;th+=30)
    {
        glNormal3d(Cos(th),0,Sin(th));
        glVertex3d(cockpit*Cos(th), cockpit, cockpit*Sin(th));
        glVertex3d(cockpit*Cos(th), 0, cockpit*Sin(th));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(0,1,0);
    glVertex3d(0, cockpit, 0);
    for (int th=0;th<=360;th+=30)
        glVertex3d(cockpit*Cos(th),cockpit, cockpit*Sin(th));
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(0,-1,0);
    glVertex3d(0,0,0);
    for (int th=0;th<=360;th+=30)
        glVertex3d(cockpit*Cos(th),0,cockpit*Sin(th));
    glEnd();
    glPopMatrix();

    // Put Spike in the cockpit
    spike_texture = LoadTexBMP("img/cockpit.bmp");
    glPushMatrix();
    glTranslated(-cockpit,1.5*cockpit,0);
    glRotated(90,0,0,1);
    glRotated(-90,1,0,0);
    glScaled(0.1,0.18,0.25);
    glColor3f(0.8,1,1);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, spike_texture);
    if (shader)
    {
        glUniform1i(glGetUniformLocation(shader, "tex"), 0);
        glUniform1f(glGetUniformLocation(shader, "useTexture"), 1);
    }
    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(1,0,0);
    glTexCoord2f(0.5, 0.5);
    glVertex3d(0.001,0,0); // Slight offset janky avoid buffer fighting
    for (int th=0;th<=360;th+=30)
    {
        // Both Claude and ChatGPT tried to sabotage this
        // Eventually I figured it out myself
        glTexCoord2f(0.5*Cos(th)+0.5, 0.5*Sin(th)+0.5);
        glVertex3d(0.001, Cos(th), Sin(th));
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    if (shader)
        glUniform1f(glGetUniformLocation(shader, "useTexture"), 0);
    glPopMatrix();

    // Canopy
    // Draw transparent objects last
    glPushMatrix();
    glTranslated(-cockpit,1.5*cockpit,0);
    glRotated(90,0,0,1);
    glScaled(0.1,0.25,0.18);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // GL_ONE works best for me
    for (int ph=-90;ph<90;ph+=30)
    {
        glBegin(GL_QUAD_STRIP);
        glColor4f(0.4,0.4,1,0.5);
        for (int th=0;th<=180;th+=30)
        {
            glNormal3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));
            glVertex3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));

            glNormal3d(Sin(th) * Cos(ph + 30), Sin(ph + 30), Cos(th) * Cos(ph + 30));
            glVertex3d(Sin(th) * Cos(ph + 30), Sin(ph + 30), Cos(th) * Cos(ph + 30));
        }
        glEnd();
    }
    glDisable(GL_BLEND);
    glPopMatrix();

    thrusters(cockpit, engine_on, thruster_shader);

    glPopMatrix();
    }