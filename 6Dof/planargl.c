/////////////////////////////////////////////////////////////
/* Template OpengGL sengaja dibuat untuk kuliah robotik 
*  di Departemen Teknik Elektro
*  Bagi yang ingin memodifikasi untuk keperluan yang lain,
*  dipersilahkan dengan menuliskan acknowledgement pada
*    Dr. Abdul Muis, MEng.
*    Autonomous Control Electronics (ACONICS) Research Group
*    http://www.ee.ui.ac.id/aconics
*/////////////////////////////////////////////////////////////

/*
 * Dimodif lagi oleh Albert H. Mendrofa untuk keperluan kelas robotika
 * Hehe 
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <GL/glut.h> // Header File For The GLUT Library
#include <GL/gl.h> // Header File For The OpenGL32 Library
#include <GL/glu.h> // Header File For The GLu32 Library
#include <unistd.h> // Header file for sleeping.
#include <math.h> 
#include <fcntl.h>      /* File control definitions */
#include <errno.h>      /* Error number definitions */
#include <termios.h>    /* POSIX terminal control definitions */
#include <sys/time.h>
#include "planar.c"

/* ascii code for the escape key */
#define ESCkey  27

/* The number/handle of our GLUT window */
int window, wcam;  

/* To draw a quadric model */
GLUquadricObj *obj;

#define Link1 L1
#define Link2 L2
#define Link3 L3
#define Base L0

/* Joint Angle */
float *tetha1=&q1;
float *tetha2=&q2;
float *tetha3=&q3;
float *tetha4=&q4;
float *tetha5=&q5;
float *tetha6=&q6;

/* Camera Position */
float *posx = &px;
float *posy = &py;
float *posz = &pz;

/* Control Parameter */
#define Naik 1
#define Turun 2

float *x=&objx;
float *y=&objy;

char debug=0;

void controlRobot();
void Sim_main(void); // Deklarasi lebih awal agar bisa diakses oleh fungsi sebelumnya
void display(void); // fungsi untuk menampilkan gambar robot / tampilan camera awal

/* define color */  
GLfloat green1[4] ={0.8, 1.0, 0.8, 1.0};
GLfloat blue1[4]  ={0.1, 0.1, 1.0, 1.0};
GLfloat blue2[4]  ={0.2, 0.2, 1.0, 1.0};
GLfloat blue3[4]  ={0.3, 0.3, 1.0, 1.0};
GLfloat yellow1[4]={0.1, 0.1, 0.0, 1.0};
GLfloat yellow2[4]={0.2, 0.2, 0.0, 1.0};
GLfloat pink6[4]  ={0.8, 0.55, 0.6, 1.0};
GLfloat yellow5[4]={0.8, 0.8, 0.0, 1.0};
GLfloat abu2[4]   ={0.5,0.5,0.5,1.0};
GLfloat gray1[4]  ={0.1, 0.1, 0.1, 1.0};
GLfloat gray2[4]  ={0.2, 0.2, 0.2, 1.0};
GLfloat gray3[4]  ={0.3, 0.3, 0.3, 1.0};
GLfloat gray4[4]  ={0.4, 0.4, 0.4, 1.0};
GLfloat gray5[4]  ={0.5, 0.5, 0.5, 1.0};
GLfloat gray6[4]  ={0.6, 0.6, 0.6, 1.0};
GLfloat gray7[4]  ={0.7, 0.7, 0.7, 1.0};
GLfloat gray8[4]  ={0.8, 0.8, 0.7, 1.0};
GLfloat gray9[4]  ={0.9, 0.9, 0.7, 1.0};

/* Create Line Model Function */
void drawOneLine(double x1, double y1, double x2, double y2) 
{
  glBegin(GL_LINES); 
  glVertex3f((x1),(y1),0.0); 
  glVertex3f((x2),(y2),0.0); 
  glEnd();
}
   
/* Create Cylinder Model Function */
void model_cylinder(GLUquadricObj * object, GLdouble lowerRadius, GLdouble upperRadius, GLdouble length, GLint res, GLfloat *color1, GLfloat *color2)
{
  glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color1);
    glTranslatef(0,0,-length/2);
    gluCylinder(object, lowerRadius, upperRadius, length, 20, res);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
    gluDisk(object, 0.01, lowerRadius, 20, res); 
    glTranslatef(0, 0, length);
    gluDisk(object, 0.01, upperRadius, 20, res); 
  glPopMatrix();
}

/* Create box model function */
void model_box(GLfloat width, GLfloat depth, GLfloat height, GLfloat *color1, GLfloat *color2, GLfloat *color3, int color)
{
   width=width/2.0;depth=depth/2.0;height=height/2.0;
   glBegin(GL_QUADS);
// top
    if (color==1) 
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color1);
    glVertex3f(-width,-depth, height);
    glVertex3f( width,-depth, height);
    glVertex3f( width, depth, height);
    glVertex3f(-width, depth, height);
   glEnd();
   glBegin(GL_QUADS);
// bottom
    if (color==1) 
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color1);
    glVertex3f(-width,-depth,-height);
    glVertex3f( width,-depth,-height);
    glVertex3f( width, depth,-height);
    glVertex3f(-width, depth,-height);
   glEnd();
   glBegin(GL_QUAD_STRIP);
// sides
    if (color==1) 
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
    glVertex3f(-width,-depth,height);
    glVertex3f(-width,-depth,-height);
    glVertex3f(width,-depth,height);
    glVertex3f(width,-depth,-height);
    glVertex3f(width,depth,height);
    glVertex3f(width,depth,-height);
    glVertex3f(-width,depth,height);
    glVertex3f(-width,depth,-height);
    glVertex3f(-width,-depth,height);
   glEnd();
}

/* Create floor function */
void disp_floor(void)
{
  int i,j,flagc=1;

  glPushMatrix();
  
  GLfloat dx=4.5,dy=4.5;
  GLint amount=15;
  GLfloat x_min=-dx/2.0, x_max=dx/2.0, x_sp=(GLfloat) dx/amount, y_min=-dy/2.0, y_max=dy/2.0, y_sp=(GLfloat) dy/amount;

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green1);
  for(i = 0; i<=48; i++)
  {
     drawOneLine(-2.4+0.1*i, -2.4,       -2.4+0.1*i,  2.4);
     drawOneLine(-2.4,       -2.4+0.1*i,  2.4,       -2.4+0.1*i);
  }

  glPopMatrix();
}

/* Penunjuk Titik */
void penunjuk()
{
  glPushMatrix();
    double x=0;
    double y=0;
    double radius=0.1;
    double y1=0;
  double x1=0;
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green1);
  glBegin(GL_LINES);
  for(double angle=0.0f;angle<=(2.0f*3.14159);angle+=0.01f)
  {
    double x2=x-(radius*(float)sin((double)angle));
    double y2=y-(radius*(float)cos((double)angle));
    glVertex3f(x1,y1,0);
    y1=y2;
    x1=x2;
  }
    glEnd();
  glPopMatrix();
}

void lighting(void)
{
  GLfloat light_ambient[] =  {0.2, 0.2, 0.2, 1.0};
  GLfloat light_diffuse[] =  {0.4, 0.4, 0.4, 1.0};
  GLfloat light_specular[] = {0.3, 0.3, 0.3, 1.0};
  GLfloat light_position[] = {2, 0.1, 7,1.0};
  GLfloat spot_direction[] = {0.0, -0.1, -1.0, 1.0};

  glClearColor(0.0, 0.0, 0.0, 0.0);     
  
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 40.0);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 4);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
}

void disp_robot(void)
{
    glPushMatrix(); /*Begin robot building */
    
    /* Draw base */
    model_box(0.3, 0.5, Base, gray8, gray7, gray6,1); //width,depth,height,*color1,*color2,*color3, int color
    
    // Menuju joint-1
    glTranslatef(0, 0, Base/2); //(hasil ketebalan box/2 + 0.5 * panjang silinder)
    glRotatef(*tetha1*RTD,0,0,1);
    glRotatef(*tetha2*RTD,1,0,0); //Untuk memutar link selanjutnya
    
    /* Gambar link1 */
    glTranslatef(0,0, Link1/2);
    glPushMatrix();
    model_cylinder(obj, 0.05, 0.05, Link1, 2, blue1, yellow2);//GLUquadricObj * object,lowerRadius,upperRadius,length,res,*color1,*color2
    penunjuk();
    glPopMatrix();
    
    /* Menuju joint-2 */
    glTranslatef(0, 0, Link1/2); //move x,y,z
    glRotatef(*tetha3*RTD,0,0,1); //sudut,x,y,z
    glRotatef(*tetha4*RTD,1,0,0);
    
    /* Gambar Link-2 */
    glTranslatef(0,0, Link2/2);
    glPushMatrix();
    model_cylinder(obj, 0.05, 0.05, Link2, 2, pink6, yellow2);
    penunjuk();
    glPopMatrix();
    
    /* Menuju joint-3 */
    glTranslatef(0, 0, Link2/2);
    glRotatef(*tetha5*RTD, 0,0,1);
    glRotatef(*tetha6*RTD, 1,0,0);
    
    /* Gambar Link-3 */
    glTranslatef(0, 0, Link3/2);
    glPushMatrix();
    model_cylinder(obj, 0.05, 0.05, Link3, 2, blue1, yellow2);
    penunjuk();
    glPopMatrix();
    glPopMatrix(); /*End robot building */
}

// Draw Object
void display(void)
{
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT) ; // Clear The Screen and The Depth Buffer 
   
   glLoadIdentity();  // Reset View
   gluLookAt(*posx, *posy, *posz,  0, 0.0, 0.4,  0.0, 0.0, 1.0); //(position of eye point, position of ref point, direction of up vector)
   
   /* Create floor */
   disp_floor();
   /* Create robot */
   disp_robot();

   /* since window is double buffered, 
      Swap the front and back buffers (used in double buffering). */
   glutSwapBuffers() ; 
   
}


void Sim_main(void)
{
  glutSetWindow(window);
  display();
}

void keyboard(unsigned char key, int i, int j)
{
  switch(key)
  {
    /* Program Control */
    case ESCkey: exit(1); break;
    
    /* Joint Control */
    case 'a': q1 += 10*DTR; ;break;
    case 'A': q1 += -10*DTR; break;
    case 's': q2 += 5*DTR; break;
    case 'S': q2 += -5*DTR; break;
    case 'd': q3 += 5*DTR; break;
    case 'D': q3 += -5*DTR; break;
    case 'f': q4 += 5*DTR; break;
    case 'F': q4 += -5*DTR; break;
    case 'g': q5 += 5*DTR; break;
    case 'G': q5 += -5*DTR; break;
    case 'h': q6 += 5*DTR; break;
    case 'H': q6 += -5*DTR; break;
      
    /* Camera control */
    case 'j': px += 0.1; break;
    case 'J': px += -0.1; break;
    case 'k': py += 0.1; break;
    case 'K': py += -0.1; break;
    case 'l': pz += 0.1;break;
    case 'L': pz += -0.1; break;
    //case 't':
    //  controlRobot();
    }
}

void init(void) 
{ 
   obj = gluNewQuadric(); 
   /* Clear background to (Red, Green, Blue, Alpha) */
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
   glEnable(GL_DEPTH_TEST); // Enables Depth Testing
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(40.0, 1, 0.2, 8);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
  lighting();
   
   /* When the shading model is GL_FLAT only one colour per polygon is used, 
      whereas when the shading model is set to GL_SMOOTH the colour of 
      a polygon is interpolated among the colours of its vertices.  */
   glShadeModel(GL_SMOOTH) ; 

   glutDisplayFunc (&display) ;
   glutKeyboardFunc(&keyboard);

}


/* Main Program */
int main(int argc, char** argv)
{
 /* Initialize GLUT */
   /* Initialize GLUT state - glut will take any command line arguments 
      see summary on OpenGL Summary */  
   glutInit (&argc, argv);
   
   /* Berikut jika ingin menggunakan serial port */
   //fd = open_port();
   //init_port(fd);

   /* Select type of Display mode:   
      Double buffer 
      RGBA color
      Alpha components supported 
      Depth buffer */  
      
   //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
   
   /* Set a 400 (width) x 400 (height) window and its position */
   glutInitWindowSize(400,400); // Ukuran window
   glutInitWindowPosition (40, 100); // Posisi window

   /* Open a window */  
   window = glutCreateWindow ("6 DOF Robot"); // Bikin window dan judul

   /* Initialize our window. */
   init() ;
   init_robot();

   /* Register the function to do all our OpenGL drawing. */
   glutIdleFunc(&Sim_main); // fungsi untuk simulasi utama

   /* Start Event Processing Engine */ 
   glutMainLoop () ;
   return 0 ;
}           
