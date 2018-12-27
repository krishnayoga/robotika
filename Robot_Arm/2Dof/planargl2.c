/////////////////////////////////////////////////////////////
/* Template OpengGL sengaja dibuat untuk kuliah robotik 
*  di Departemen Teknik Elektro
*  Bagi yang ingin memodifikasi untuk keperluan yang lain,
*  dipersilahkan dengan menuliskan acknowledgement pada
*    Dr. Abdul Muis, MEng.
*    Autonomous Control Electronics (ACONICS) Research Group
*    http://www.ee.ui.ac.id/aconics
*////////////////////////////////////////////////////////////

#include <stdio.h> 
#include <stdlib.h> 
#include <GL/glut.h> // Header File For The GLUT Library
#include <GL/gl.h> // Header File For The OpenGL32 Library
#include <GL/glu.h> // Header File For The GLu32 Library
#include <unistd.h> // Header file for sleeping.
#include <math.h> 
#include <fcntl.h>			/* File control definitions */
#include <errno.h>			/* Error number definitions */
#include <termios.h>		/* POSIX terminal control definitions */
#include <sys/time.h>
#include "planar2.c"
#define q_text 113
#define w_text 119
#define e_text 101
#define r_text 114
#define akecil 97

/* ascii code for the escape key */
#define ESCkey	27

/* The number/handle of our GLUT window */
int window, wcam;  
 


/* To draw a quadric model */
GLUquadricObj *obj;

// ROBOT MODEL PARAMATER
#define Xoffset	0.0	
#define Yoffset	0.0
#define Zoffset	0.3

#define Link1 L1
#define Link2 L2

int state = 1;

float *tetha1=&q1;
float *tetha2=&q2;

float *x=&objx;
float *y=&objy;

char debug=0;

float ang = 0;
float rad = 0.1f;

float x_cen = Xoffset-0.16-0.15;;
float y_cen = Yoffset+0.23-0.0;

float mat[16];
float x_pos = 0.45;
float y_pos = 0;
float r;
float phi1;
float phi2;
float phi3;
float theta1;
float theta2;
float temp;
int choice = 2;

void Sim_main(void); // Deklarasi lebih awal agar bisa diakses oleh fungsi sebelumnya
void display(void); // fungsi untuk menampilkan gambar robot / tampilan camera awal
void controlRobot();
void putarRobot();

/* define color */  
GLfloat green1[4]  ={0.8, 1.0, 0.8, 1.0};
GLfloat blue1[4]  ={0.1, 0.1, 1.0, 1.0};
GLfloat blue2[4]  ={0.2, 0.2, 1.0, 1.0};
GLfloat blue3[4]  ={0.3, 0.3, 1.0, 1.0};
GLfloat yellow1[4]={0.1, 0.1, 0.0, 1.0};
GLfloat yellow2[4]={0.2, 0.2, 0.0, 1.0};
GLfloat pink6[4] ={0.8, 0.55, 0.6, 1.0};
GLfloat yellow5[4]={0.8, 0.8, 0.0, 1.0};
GLfloat abu2[4]={0.5,0.5,0.5,1.0};
GLfloat gray1[4]  ={0.1, 0.1, 0.1, 1.0};
GLfloat gray2[4]  ={0.2, 0.2, 0.2, 1.0};
GLfloat gray3[4]  ={0.3, 0.3, 0.3, 1.0};
GLfloat gray4[4]  ={0.4, 0.4, 0.4, 1.0};
GLfloat gray5[4]  ={0.5, 0.5, 0.5, 1.0};
GLfloat gray6[4]  ={0.6, 0.6, 0.6, 1.0};
GLfloat gray7[4]  ={0.7, 0.7, 0.7, 1.0};
GLfloat gray8[4]  ={0.8, 0.8, 0.7, 1.0};
GLfloat gray9[4]  ={0.9, 0.9, 0.7, 1.0};


void  drawOneLine(double x1, double y1, double x2, double y2) 
   {glBegin(GL_LINES); glVertex3f((x1),(y1),0.0); glVertex3f((x2),(y2),0.0); glEnd();}
   
void  model_cylinder(GLUquadricObj * object, GLdouble lowerRadius,
  GLdouble upperRadius, GLdouble length, GLint res, GLfloat *color1, GLfloat *color2)
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

void  model_box(GLfloat width, GLfloat depth, GLfloat height, GLfloat *color1, GLfloat *color2, GLfloat *color3, int color)
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



void disp_floor(void)
{
  int i,j,flagc=1;

  glPushMatrix();
  
  GLfloat dx=4.5,dy=4.5;
  GLint amount=15;
  GLfloat x_min=-dx/2.0, x_max=dx/2.0, x_sp=(GLfloat) dx/amount, y_min=-dy/2.0, y_max=dy/2.0, y_sp=(GLfloat) dy/amount;

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green1);
  for(i = 0; i<=48; i++){
     drawOneLine(-2.4+0.1*i, -2.4,       -2.4+0.1*i,  2.4);
     drawOneLine(-2.4,       -2.4+0.1*i,  2.4,       -2.4+0.1*i);
  }

  glPopMatrix();
}

void  lighting(void)
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
  glPushMatrix();
    model_box(0.3, 0.5, 0.05, gray8, gray7, gray6,1);
    glTranslatef(Xoffset, Yoffset, Zoffset/2);
    // Draw base
    model_cylinder(obj, 0.1, 0.1, Zoffset, 2, blue1, yellow2);
    // Menuju joint-1
    glTranslatef(0, 0, Zoffset/2);
    glRotatef(*tetha1*RTD,0,0,1);
    glPushMatrix();
      // Gambar link1-1
      glRotatef(-90,1,0,0);
      glTranslatef(0,0,Link1/2);
      model_cylinder(obj, 0.03, 0.03, Link1, 2, pink6, yellow2);
    glPopMatrix();
    // Menuju joint-2
    glTranslatef(0,Link1, 0);
    glRotatef(*tetha2*RTD,0,0,1);
    glPushMatrix();
      // Gambar link1-1
      glRotatef(-90,1,0,0);
      glTranslatef(0,0,Link2/2);
      model_cylinder(obj, 0.03, 0.03, Link2, 2, yellow5, yellow2);
    glPopMatrix();
    /*
    glTranslatef(0,Link1, 0);
    glRotatef(*tetha1*RTD,0,0,1);
    glPushMatrix();
      // Gambar link1-1
      glRotatef(-90,1,0,0);
      glTranslatef(0,0,Link2/2);
      model_cylinder(obj, 0.03, 0.03, Link2, 2, pink6, yellow2);
    glPopMatrix();
    **/
 	 glPopMatrix();
  glPushMatrix();
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    glTranslatef(Xoffset-0.16-0.15, Yoffset+0.23-0.09, Zoffset);
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

// Draw Object
void display(void)
{
//   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT) ; // Clear The Screen and The Depth Buffer 
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT) ; // Clear The Screen and The Depth Buffer 
   //glLoadIdentity();  // Reset View
   disp_floor();

   disp_robot();
   if(choice == 1){
   	 	putarRobot();
   }
   else{
   		controlRobot();
   }
   //printf("x %f y % f z % f \n",mat[12],mat[13],mat[14]);
   //printf("Q1 : %f Q2 : %f x_pos : %f y_pos : %f\n",q1,q2,x_pos,y_pos);
   printf("ang = %f,x_pos = %f,y_pos = %f,r = %f,phi1 = %f,phi2 = %f,phi3 = %f,temp = %f\n",ang,x_pos,y_pos,r,phi1,phi2,phi3,temp);
   /* since window is double buffered, 
      Swap the front and back buffers (used in double buffering). */
   glutSwapBuffers() ; 
   

}

void Sim_main(void)
{
	unsigned long Xr=0,Yr=0, Xg=0,Yg=0, Xb=0,Yb=0; // titik untuk menghitung sum
	int Nr=0, Ng=0, Nb=0;
	static unsigned int Rx,Ry, Gx,Gy, Bx,By; // untuk menyimpan hasil titik berat
	unsigned int i,j,k;
  glutSetWindow(window);
  display();
  
  	
	
}

void keyboard(unsigned char key, int i, int j)
{
  	
  switch(key){
      case ESCkey: exit(1); break;
      
      case q_text:
       choice = 1;
       break;
       
    case w_text:
       choice = 2;
       break;
       
    case e_text:
		break;
       
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
   	gluLookAt(1, 0.0, 1.5,  -0.0, 0.0, 0.6,  0.0, 0.0, 1.0); 
	lighting();
	 
   /* When the shading model is GL_FLAT only one colour per polygon is used, 
      whereas when the shading model is set to GL_SMOOTH the colour of 
      a polygon is interpolated among the colours of its vertices.  */
   glShadeModel(GL_SMOOTH) ; 

   glutDisplayFunc (&display) ;
   glutKeyboardFunc(&keyboard);

}

void controlRobot(){
	if(state == 1)
		{
			x_pos = x_pos - 0.004;
    		y_pos = -x_pos + 0.45;
    		if(x_pos < 0.02){
    			state = 2;
			}
		}    
    	else{
    		x_pos = x_pos +0.004;
    		y_pos = -x_pos + 0.45;
    		if(x_pos > 0.42){
    			state = 1;
			}
		}
    	r = (float)sqrt((double)(x_pos*x_pos + y_pos*y_pos));
    	if(r >0.1 && r <0.5){
			temp = (Link1*Link1+r*r-Link2*Link2)/(2.0f*Link1*r);
			phi2 = (float)atan((double)(y_pos/x_pos));
			phi1 = (float)acos((double)((Link1*Link1+r*r-Link2*Link2)/(2.0f*Link1*r)));
			phi3 = (float)acos((double)((Link1*Link1-r*r+Link2*Link2)/(2.0f*Link1*Link2)));
			q1 = (phi2 - phi1);
			q2 = (180*DTR - phi3);
		}
		usleep(10000);
}

void putarRobot(){
		rad = rad +0.03;
		x_pos = 0.12 + 0.1*cos(rad);
    	y_pos = 0.32 + 0.1*sin(rad);
    	r = (float)sqrt((double)(x_pos*x_pos + y_pos*y_pos));
    	if(r >0.1 && r <0.5){
			temp = (Link1*Link1+r*r-Link2*Link2)/(2.0f*Link1*r);
			phi2 = (float)atan((double)(y_pos/x_pos));
			phi1 = (float)acos((double)((Link1*Link1+r*r-Link2*Link2)/(2.0f*Link1*r)));
			phi3 = (float)acos((double)((Link1*Link1-r*r+Link2*Link2)/(2.0f*Link1*Link2)));
			q1 = (phi2 - phi1);
			q2 = (180*DTR - phi3);
		}
		usleep(10000);
}

// Main Program
int main(int argc, char** argv)
{
 // Initialize GLUT
   /* Initialize GLUT state - glut will take any command line arguments 
      see summary on OpenGL Summary */  
   glutInit (&argc, argv);
   
   // Berikut jika ingin menggunakan serial port
   //fd = open_port();
   //init_port(fd);

   /* Select type of Display mode:   
      Double buffer 
      RGBA color
      Alpha components supported 
      Depth buffer */  
   //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
   /* set a 400 (width) x 400 (height) window and its position */
   glutInitWindowSize(400,400);	
   glutInitWindowPosition (200, 100);

   /* Open a window */  
   window = glutCreateWindow ("Simple Window");

   /* Initialize our window. */
   init() ;
   init_robot();

   /* Register the function to do all our OpenGL drawing. */
   glutIdleFunc(&Sim_main); // fungsi untuk simulasi utama
	
   /* Start Event Processing Engine */ 
   glutMainLoop () ;
   return 0 ;
}           

