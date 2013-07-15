#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#define ESCAPE 27

/* Here we will put variables which we wish to be global */
GLint window;           /* The number of our GLUT window */
GLint Xsize=400;
GLint Ysize=400;

GLfloat xt=0.0,yt=0.0,zt=0.0;   /* x,y,z translation */
GLfloat xs=1.0,ys=1.0,zs=1.0;   /* x,y,z scale */
GLfloat xangle=0.0,yangle=0.0,zangle=0.0;   /* axis angles */

/*//angle of rotation with mouse
float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle=0.0;
float lastx, lasty;*/

/* Kamera dengan Mouse 
// angle of rotation for the camera direction
float angle=0.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float x=0.0f,z=5.0f;
float deltaAngle = 0.0f;
int xOrigin = -1;*/

//Kamera Mouse Move
double camera_angle_h = 0;
double camera_angle_v = 0;
int drag_x_origin;
int drag_y_origin;
int dragging = 0;

GLfloat sizes[2];       /* Store supported point size range */
GLfloat step;           /* Store supported point size increments */

//Transformasi
float shear[] = { //shear x 0.25
   1, 0, 0, 0, 
   0.25, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1};
float shear_tengah[] = { //shear x 0.15
   1, 0, 0, 0, 
   0.1, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1};
float refleksi_y[] = { //refleksi sumbu y
   -1, 0, 0, 0, 
   0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1};
    
float refleksi_z[] = { //refleksi sumbu z
   1, 0, 0, 0, 
   0, 1, 0, 0,
    0, 0, -1, 0,
    0, 0, 0, 1};

GLvoid Transform(GLfloat Width, GLfloat Height)
{
  glViewport(0, 0, Width, Height);              /* Set the viewport */
  glMatrixMode(GL_PROJECTION);                  /* Select the projection matrix */
  glLoadIdentity();				/* Reset The Projection Matrix */
  gluPerspective(45.0,Width/Height,0.1,100.0);  /* Calculate The Aspect Ratio Of The Window */
  glMatrixMode(GL_MODELVIEW);                   /* Switch back to the modelview matrix */
}

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
GLvoid InitGL(GLfloat Width, GLfloat Height)	
{
  glClearColor(1.0, 1.0, 1.0, 1.0);		/* This Will Clear The Background Colour To Black */
  glPointSize(4.0);                             /* Add point size, to make it clear */
  glLineWidth(2.0);                             /* Add line width,   ditto */
  glShadeModel(GL_SMOOTH);                      /* Set the color model */
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  Transform( Width, Height );                   /* Perform the transformation */
}

/* The function called when our window is resized  */
GLvoid ReSizeGLScene(GLint Width, GLint Height)
{
  if (Height==0)    Height=1;                   /* Sanity checks */
  if (Width ==0)    Width=1;
  Transform( Width , Height );                   /* Perform the transformation */
}

/*void camera (void) {
    glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on teh x-axis (left and right)
    glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
    glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
}*/

//Taruh fungsi objek primitif disini
//Diagonal Kubus
void diagonal_kubus_depan_belakang(void)
{  
 glBegin(GL_LINES);
   glColor3f(0.0,0.0,1.0);
   glVertex3f(-0.5,0.5,0.5);
   glVertex3f(0.5,-0.5,0.5);
   glVertex3f(0.5,0.5,0.5);
   glVertex3f(-0.5,-0.5,0.5);
  glEnd();
}
void diagonal_kubus_kiri_kanan(void)
{  
 glBegin(GL_LINES);
   glColor3f(0.0,0.0,1.0);
   glVertex3f(-0.5,0.5,0.5);
   glVertex3f(-0.5,-0.5,-0.5);
   glVertex3f(-0.5,0.5,-0.5);
   glVertex3f(-0.5,-0.5,0.5);
 glEnd();
}
// Kubus
void kubus(void)
{      
  glPushMatrix(); //Diagonal sisi depan
   diagonal_kubus_depan_belakang();
  glPopMatrix();
  glPushMatrix(); //Diagonal sisi belakang
   glMultMatrixf(refleksi_z);
   diagonal_kubus_depan_belakang();
  glPopMatrix();
  glPushMatrix(); //Diagonal sisi kiri
   diagonal_kubus_kiri_kanan();
  glPopMatrix();
  glPushMatrix(); //Diagonal sisi kanan
   glMultMatrixf(refleksi_y);
   diagonal_kubus_kiri_kanan();
  glPopMatrix();
  glPushMatrix();
   glColor3f(0.0,0.0,1.0); 
   glutWireCube(1.0f);
  glPopMatrix();
}
// Kaki Eiffel
void kaki(void)
{
  glPushMatrix(); //kubus tengah
   glTranslatef(0.0,2.0,0.0);
   glScalef(1.75,1.0,3.0);
   kubus();
  glPopMatrix();
  glPushMatrix(); //kubus penyambung depan
   glTranslatef(-0.75,2.0,1.5);
   glMultMatrixf(shear);
   glScalef(2.5,1.0,1.0);
   kubus();
  glPopMatrix();
  glPushMatrix(); //kubus penyambung belakang
   glTranslatef(-1.5,2.0,1.0);
   glMultMatrixf(shear);
   glScalef(1.0,1.0,2.0);
   kubus();
  glPopMatrix();
  glPushMatrix(); //kubus tengah kaki
   glTranslatef(-2.0,0.0,1.5); 
   glMultMatrixf(shear);  
   kubus();
  glPopMatrix();
  glPushMatrix(); //kubus atas kaki
   glTranslatef(-1.75,1.0,1.5); 
   glMultMatrixf(shear);  
   kubus();
  glPopMatrix();
  glPushMatrix(); //kubus bawah kaki
   glTranslatef(-2.25,-1.0,1.5); 
   glMultMatrixf(shear);  
   kubus();
  glPopMatrix();
  /*glPushMatrix(); //kubus panjang
   glTranslatef(-2.0,0.0,2.0); 
   glMultMatrixf(shear);
   glScalef(1.0,3.0,1.0);
   kubus();
  glPopMatrix();*/
}
void kaki_kecil(void)
{
  /*glPushMatrix(); //kubus tengah
   glTranslatef(0.0,2.0,0.0);
   glScalef(1.75,1.0,3.0);
   kubus();
  glPopMatrix();
  glPushMatrix(); //kubus penyambung depan
   glTranslatef(-0.75,2.0,1.5);
   glMultMatrixf(shear);
   glScalef(2.5,1.0,1.0);
   kubus();
  glPopMatrix();
  glPushMatrix(); //kubus penyambung belakang
   glTranslatef(-1.5,2.0,1.0);
   glMultMatrixf(shear);
   glScalef(1.0,1.0,2.0);
   kubus();
  glPopMatrix();*/
  glPushMatrix(); //kubus tengah kaki
   glTranslatef(-1.15,0.5,1.0); 
   glMultMatrixf(shear_tengah); 
   glScalef(0.75,1.0,0.75);  
   kubus();
  glPopMatrix();
  glPushMatrix(); //kubus atas kaki
   glTranslatef(-1.1,1.52,1.0); 
   glScalef(0.75,1.0,0.75);  
   kubus();
  glPopMatrix();
  glPushMatrix(); //kubus bawah kaki
   glTranslatef(-1.3,-0.5,1.0); 
   glMultMatrixf(shear);  
   glScalef(0.75,1.0,0.75);  
   kubus();
  glPopMatrix();
  /*glPushMatrix(); //kubus panjang
   glTranslatef(-2.0,0.0,2.0); 
   glMultMatrixf(shear);
   glScalef(1.0,3.0,1.0);
   kubus();
  glPopMatrix();*/
}
//Kaki Eiffle 4
void kaki4(void)
{ 
 //kaki kiri depan
  glPushMatrix();
   kaki();
  glPopMatrix(); 
  //kaki kanan depan
  glPushMatrix();
   glMultMatrixf(refleksi_y);
   kaki();
  glPopMatrix();
  //kaki kiri belakang
  glPushMatrix();
   glMultMatrixf(refleksi_z);
   kaki();
  glPopMatrix(); 
  //kaki kanan belakang
  glPushMatrix();
   glMultMatrixf(refleksi_y);
   glMultMatrixf(refleksi_z);
   kaki();
  glPopMatrix();
}
void kaki4_tengah(void)
{ 
 //kaki kiri depan
  glPushMatrix();
   kaki_kecil();
  glPopMatrix(); 
  //kaki kanan depan
  glPushMatrix();
   glMultMatrixf(refleksi_y);
   kaki_kecil();
  glPopMatrix();
  //kaki kiri belakang
  glPushMatrix();
   glMultMatrixf(refleksi_z);
   kaki_kecil();
  glPopMatrix(); 
  //kaki kanan belakang
  glPushMatrix();
   glMultMatrixf(refleksi_y);
   glMultMatrixf(refleksi_z);
   kaki_kecil();
  glPopMatrix();
}
//Bangunan tengah menara
void tengah(void)
{
 glPushMatrix();//Tengah bawah solid
  glColor3f(0.0,0.0,0.0); 
  glTranslatef(0.0,2.65,0.0);
  glScalef(4.0,0.25,4.0);
  glutSolidCube(1.0f);
 glPopMatrix();
 glPushMatrix();//Tengah atas solid
  glColor3f(0.0,0.0,0.0); 
  glTranslatef(0.0,3.0,0.0);
  glScalef(4.75,0.5,4.75);
  glutSolidCube(1.0f);
 glPopMatrix();
 glPushMatrix();
  glTranslatef(0.0,4.25,0.0);
  //glScalef(0.65,0.65,0.65);
  kaki4_tengah();
 glPopMatrix();
}
GLvoid DrawGLScene()
{
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	/* Clear screen, depth Buffer */
  //int i;
  //float xy;
  //glColor3ub(255,0,0);                   /* Set the colour rgb, 0-255 */
  //glBegin(GL_POINTS);                    /* Points */
  // for (i=0, xy=0.0 ;i<10;i++, xy+=0.1)  
  //   glVertex3f(xy,xy,-6.0);             /* where */
  //glEnd();
  //glColor3f(0.0,1.0,0.0);                /* Set colour rgb, 0.0 - 1.0 */
  //glBegin(GL_LINES);                     /* Lines this time */
  // for (i=0, xy=0.0 ;i<10;i++, xy+=0.1) {
  //   glVertex3f(xy,xy,-6.0);             /* need start and end points */
  //   glVertex3f(xy+1.0,xy,-6.0);
  // }
  //glEnd();
  //glBegin(GL_QUADS);                     /* Quads this time */
  // for (i=0, xy=0.0 ;i<10;i++, xy-=0.1) { 
  //   glColor4f(1.0,1.0+xy,0.0,0.4);      /* Set colour rgba, 0.0 - 1.0 */
  //   glVertex3f(xy,xy,-6.0);             /* Now define 4 corners */
  //   glVertex3f(xy-1.0,xy,-6.0);
  //   glVertex3f(xy-1.0,xy-1.0,-6.0);
  //   glVertex3f(xy,xy-1.0,-6.0);
  // }
  //glEnd();
  
  //glBegin(GL_LINES);                     /* Lines */
  //   glColor3f(1.0,0.0,0.0);             /* Set colour Red */
  //   glVertex3f(0.0,0.0,-6.0);           /* need start and end points */
  //   glColor3f(1.0,1.0,0.0);             /* Set colour Yellow */
  //   glVertex3f(2.0,2.0,-6.0);
  //glEnd();

  //glBegin(GL_QUADS);                     /* Quads */
  //   glColor3f(1.0,0.0,0.0);             /* Set colour red */
  //   glVertex3f(-2.0,-2.0,-6.0);         
  //   glColor3f(1.0,1.0,0.0);             /* Set colour yellow */
  //   glVertex3f(-2.0,2.0,-6.0);
  //  glColor3f(1.0,0.0,0.0);             /* Set colour red */
  // glVertex3f(0.0,2.0,-6.0);
  // glColor3f(1.0,1.0,0.0);             /* Set colour yellow  */
  // glVertex3f(0.0,-2.0,-6.0);
  //End();
  
  /*koding Quads
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(0.0,0.0,-6.0);
  glTranslatef(xt,yt,zt);
  glScalef(xs,ys,zs);
  glRotatef(xangle,1.0,0.0,0.0);
  glRotatef(yangle,0.0,1.0,0.0);
  glRotatef(zangle,0.0,0.0,1.0);
  glBegin(GL_QUADS);                     
    glColor3f(1.0,1.0,0.0);     
    glVertex3f( 1.0f, 1.0f, 1.0f);       
    glColor3f(1.0,0.0,0.0);     
    glVertex3f(-1.0f, 1.0f, 1.0f);        
    glColor3f(1.0,1.0,0.0);     
    glVertex3f(-1.0f,-1.0f, 1.0f);       
    glColor3f(1.0,0.0,0.0);     
    glVertex3f( 1.0f,-1.0f, 1.0f);    
   glEnd();
  glPopMatrix();*/
  
  /* Koding Cube Kerangka
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(0.0,0.0,-6.0);
  glRotatef(xangle,1.0,0.0,0.0);
  glRotatef(yangle,0.0,1.0,0.0);
  glRotatef(zangle,0.0,0.0,1.0);
  glBegin(GL_LINE_STRIP);                    //  Linestrip this time 
    glColor3f(0.0,1.0,0.0);                // Set green 
    glVertex3f(-1.0,-1.0,-1.0);
    glVertex3f(1.0,-1.0,-1.0);
    glVertex3f(1.0,1.0,-1.0);
    glVertex3f(-1.0,1.0,-1.0);
    glVertex3f(-1.0,-1.0,-1.0);
  glEnd();
  glBegin(GL_LINE_STRIP);                     // Linestrip this time 
    glColor3f(1.0,0.0,0.0);                // Set colour red 
    glVertex3f(-1.0,-1.0,1.0);
    glVertex3f(1.0,-1.0,1.0);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(-1.0,1.0,1.0);
    glVertex3f(-1.0,-1.0,1.0);
  glEnd();
  glBegin(GL_LINES);                     // Lines this time 
    glColor3f(0.0,0.0,1.0);                // Set colour blue 
    glVertex3f(-1.0,-1.0,1.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glVertex3f(-1.0,1.0,1.0);
    glVertex3f(-1.0,1.0,-1.0);
    glVertex3f(1.0,-1.0,1.0);
    glVertex3f(1.0,-1.0,-1.0);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(1.0,1.0,-1.0);
  glEnd();
  glPopMatrix();*/
  
  /* Koding Cube Solid
  glColor3f(0.0f,1.0f,0.0f);            // Set The Color To Blue
  glVertex3f( 1.0f, 1.0f,-1.0f);        // Top Right Of The Quad (Top)
  glVertex3f(-1.0f, 1.0f,-1.0f);        // Top Left Of The Quad (Top)
  glVertex3f(-1.0f, 1.0f, 1.0f);        // Bottom Left Of The Quad (Top)
  glVertex3f( 1.0f, 1.0f, 1.0f);        // Bottom Right Of The Quad (Top)*/
  
  /*koding untuk latihan */
  /*glBegin(GL_LINES);
   glVertex3f(0.0,0.0,-10.0);
   glVertex3f(2.0,0.0,-10.0);
  glEnd();*/
  
  /*koding eiffel*/
  //glPushMatrix();
  glLoadIdentity();
  //set Kamera 
  gluLookAt(0.0, 0.0, 25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glRotated(camera_angle_v, 1.0, 0.0, 0.0);
  glRotated(camera_angle_h, 0.0, 1.0, 0.0);
  /* Set the camera
	gluLookAt(	x, 1.0f, z,
			x+lx, 1.0f,  z+lz,
			0.0f, 1.0f,  0.0f);*/
  //camera();
  /* Lama jangan dihapus
  glTranslatef(0.0,0.0,-6.0);
  glTranslatef(xt,yt,zt);
  glScalef(xs,ys,zs);
  glRotatef(xangle,1.0,0.0,0.0);
  glRotatef(yangle,0.0,1.0,0.0);
  glRotatef(zangle,0.0,0.0,1.0);
  glLineWidth(3.0);   
  // Garis dasar bawah besar
  KotakKerangka();
  glScalef(0.75,1.0,0.75);
  glTranslatef(0.0,0.0,-1.25);
  KotakKerangka();
  glPopMatrix(); */
  // garis bantu koordinat
  glColor3f(0.0,1.0,0.0); 
  glBegin(GL_LINES);
   glVertex3f(0.0,-10.0,0.0);
   glVertex3f(0.0,10.0,0.0);
   glVertex3f(-10.0,0.0,0.0);
   glVertex3f(10.0,0.0,0.0);
   glVertex3f(0.0,0.0,-10.0);
   glVertex3f(0.0,0.0,10.0);
  glEnd();
  
  //glColor3f(0.0,0.0,1.0);
  kaki4();
  tengah();
  glFlush(); 
  printf("drawing called\n");
}

/* Mouse Move tapi bellum sempurna
void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void computeDir(float deltaAngle) {

	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}*/

void NormalKey(GLubyte key, GLint x, GLint y) 
{
    switch ( key )    { 
     case ESCAPE :
        printf("escape pressed. exit.\n");
	    glutDestroyWindow(window);	/* Kill our window */
	    exit(0); 			/* Very dirty exit */                  
        break;
      case 'b':
        glEnable(GL_BLEND);
        glutPostRedisplay();
        break;
     case 'B':
        glDisable(GL_BLEND);
        glutPostRedisplay();
        break;				/* Do we need this??? */
     case 'P':
     case 'p':
        glGetFloatv(GL_POINT_SIZE_RANGE,sizes);
        glGetFloatv(GL_POINT_SIZE_GRANULARITY,&step);
        printf("Point size ranges from %f to %f at %f steps.\n",
               sizes[0],sizes[1],step);
        break;
     case 'W':
     case 'w':                          /* Tell us about the window */
        printf("Current window size is %d x %d pixels in %s mode\n",
               glutGet(GLUT_WINDOW_WIDTH),
               glutGet(GLUT_WINDOW_HEIGHT),
               glutGet(GLUT_WINDOW_DOUBLEBUFFER) ? "double buffer" :
                                                   "single buffer");
        break;
     case 'x':                          /* Move to right */
        xt += 0.2;
        glutPostRedisplay();
        break;
     case 'X':
        xt -= 0.2;                      /* Move to left */
        glutPostRedisplay();
        break;

      case 'y':                          /* Move up */
        yt += 0.2;
        glutPostRedisplay();
        break;
     case 'Y':
        yt -= 0.2;                      /* Move down */
        glutPostRedisplay();
        break;

      case 'z':                          /* Move forward */
        zt += 0.2;
        glutPostRedisplay();
        break;
     case 'Z':
        zt -= 0.2;                      /* Move away */
        glutPostRedisplay();
        break;
     case 'j':                          /* scale x up */
        xs += 0.2;
        glutPostRedisplay();
        break;
     case 'J':
        xs -= 0.2;                      /* scale x down */
        glutPostRedisplay();
        break;
      case 'k':                          /* scale y up */
        ys += 0.2;
        glutPostRedisplay();
        break;
     case 'K':
        ys -= 0.2;                      /* scale y down */
        glutPostRedisplay();
        break;
      case 'l':                          /* scale z up */
        zs += 0.2;
        glutPostRedisplay();
        break;
     case 'L':
        zs -= 0.2;                      /* scale z down */
        glutPostRedisplay();
        break;
     case 'u':
        xangle += 5.0;
        glutPostRedisplay();
        break;
     case 'U':
        xangle -= 5.0;
        glutPostRedisplay();
        break;
     case 'i':
        yangle += 5.0;
        glutPostRedisplay();
        break;
     case 'I':
        yangle -= 5.0;
        glutPostRedisplay();
        break;
     case 'o':
        zangle += 5.0;
        glutPostRedisplay();
        break;
     case 'O':
        zangle -= 5.0;
        glutPostRedisplay();
        break;
     default:
	    break;
    }
}

/*
void mouseMovement(int x, int y) {
    int diffx=x-lastx; //check the difference between the current x and the last x position
    int diffy=y-lasty; //check the difference between the current y and the last y position
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position
    xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
    yrot += (float) diffx;// set the xrot to yrot with the addition of the difference in the x position
}*/

/* Mouse Move belum sempurna
void mouseButton(int button, int state, int x, int y) {
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}

void mouseMove(int x, int y) {

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}*/

void mouse_click(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON) {
        if(state == GLUT_DOWN) {
            dragging = 1;
            drag_x_origin = x;
            drag_y_origin = y;
        }
        else
            dragging = 0;
    }
}

void mouse_move(int x, int y) {
    if(dragging) {
        camera_angle_v += (y - drag_y_origin)*0.3;
        camera_angle_h += (x - drag_x_origin)*0.3;
        drag_x_origin = x;
        drag_y_origin = y;
    }
}

int main(int argc, char *argv[])
{
  /* Initialisation and window creation */
  glutInit(&argc, argv);               /* Initialize GLUT state. */

  glutInitDisplayMode(GLUT_RGBA |      /* RGB and Alpha */
                      GLUT_SINGLE|     /* Single buffer */
                      GLUT_DEPTH);     /* Z buffer (depth) */

  glutInitWindowSize(400,400);         /* set initial window size. */
  glutInitWindowPosition(0,0);         /* upper left corner of the screen. */
  window = glutCreateWindow("L0");     /* Open a window with a title. */ 
  InitGL(400,400);                     /* Initialize our window. */
/* Now register the various callback functions */
  glutDisplayFunc(DrawGLScene);        /* Function to do all our OpenGL drawing. */
  glutReshapeFunc(ReSizeGLScene);
  glutIdleFunc(DrawGLScene);
  //glutPassiveMotionFunc(mouseMovement); //check for mouse movement
  /* here are the two new functions berhasil belum sempurna
  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMove);*/
  glutMouseFunc(mouse_click);
  glutMotionFunc(mouse_move);
  glutKeyboardFunc(NormalKey);         /*Normal key is pressed */
  
  glEnable(GL_DEPTH_TEST);
/* Now drop into the event loop from which we never return */
  glutMainLoop();                      /* Start Event Processing Engine. */ 
   return 1;
   
   
  /*system("PAUSE");	
  return 0;*/
}
