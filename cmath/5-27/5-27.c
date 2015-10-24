#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "matrix.h"
#define POINT 5
#define SPEED 0.01 

void init(void);

int width, height; //ウィンドウの大きさ
double size; // 描画する正方形のサイズ
int increase; // 正方形が大きくなるときは 1，小さくなるときは 0


void display(void)
{
  double x_center, y_center;
  x_center = width / 2.0;
  y_center = height / 2.0;

  glClear(GL_COLOR_BUFFER_BIT);
  //glRectd(x_center - size, y_center - size, x_center + size, y_center + size);

  // window size = (800, 450)

  // A
  glColor3d(1.0, 0.6, 0.0);
  glBegin(GL_TRIANGLES);
  glVertex2d(x_center - size * 200, y_center + 125 * size);
  glVertex2d(x_center - size * 285, y_center + size * 60);
  glVertex2d(x_center - size * 115, y_center + size * 60);
  glEnd();

  glBegin(GL_TRIANGLES);
  glVertex2d(x_center - size * 200, y_center + size * 105);
  glVertex2d(x_center - size * 285, y_center + size * 40);
  glVertex2d(x_center - size * 115, y_center + size * 40);
  glEnd();

  glColor3d(1.0, 1.0, 1.0);
  glBegin(GL_TRIANGLES);
  glVertex2d(x_center - size * 200, y_center + size * 81);
  glVertex2d(x_center - size * 255, y_center + size * 40);
  glVertex2d(x_center - size * 145, y_center + size * 40);
  glEnd();

  glColor3d(1.0, 0.6, 0.0);
  glRectd(x_center - size * 285, y_center - size * 140, x_center - size * 255, y_center + size * 60);
  glRectd(x_center - size * 145, y_center - size * 140, x_center - size * 115, y_center + size * 60);

  glColor3d(1.0, 0.6, 0.0);
  glRectd(x_center - size * 255, y_center - size * 55, x_center - size * 145, y_center - size * 25);


  // dot
  glPointSize(20.0 * size);
  glBegin(GL_POINTS);
  glVertex2d(400, y_center - size * 130);
  glEnd();


  // M
  glRectd(x_center + size * 80, y_center - size * 140, x_center + size * 110, y_center + size * 105);
  glRectd(x_center + size * 290, y_center - size * 140, x_center + size * 320, y_center + size * 105);

  glBegin(GL_TRIANGLES);
  glVertex2d(x_center + size * 200, y_center - size * 64);
  glVertex2d(x_center + size * 80, y_center + size * 105);
  glVertex2d(x_center + size * 320, y_center + size * 105);
  glEnd();

  glColor3d(1.0, 1.0, 1.0);
  glBegin(GL_TRIANGLES);
  glVertex2d(x_center + size * 200, y_center - size * 25);
  glVertex2d(x_center + size * 110, y_center + size * 105);
  glVertex2d(x_center + size * 290, y_center + size * 105);
  glEnd();

  glutSwapBuffers();
}


void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
  width = w;
  height = h;
}


void idle(void){
  if(increase){
    size += SPEED;
    if (size >= 1) {
      size = 1;
      increase = 0;
    }
  } else {
    size -= SPEED;
    if (size <= 0) {
      size = 0;
      increase = 1;
    }
  }
  glutPostRedisplay();
}


void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 0.0);
  size = 1;
  increase = 1;
}


int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize(800, 450);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  init();
  glutMainLoop();

  return 0;
}




