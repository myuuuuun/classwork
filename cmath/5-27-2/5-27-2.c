#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "matrix.h"
#define INIT_SIZE 50 
#define SPEED 0.1 

void init(void);
int width, height;
int right_down;
int color; // 緑は0，赤なら1
int is_display; // 0なら描写しない、1なら描写する
int point_x, point_y;


void point(void)
{
  double x_center, y_center;
  x_center = width / 2.0;
  y_center = height / 2.0;

  glClear(GL_COLOR_BUFFER_BIT);

  if(is_display == 1){
    if(color == 0){
      glColor3d(0.0, 1.0, 0.0);
    } else {
      glColor3d(1.0, 0.0, 0.0);
    }
    glPointSize(10.0);
    glBegin(GL_POINTS);
    glVertex2d(point_x, point_y);
    glEnd();
  }

  glutSwapBuffers();
}


void reshape(int w, int h){
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
  width = w;
  height = h;
}


void idle(void){
  //
  glutPostRedisplay();
}


void mouse(int button, int state, int x, int y){
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      // 0なら1に、1なら0に
      color = color==0 ? 1 : 0;
      glutPostRedisplay();
    }
  }

  if(button == GLUT_RIGHT_BUTTON) {
    // GLUT_DOWNなら1, GLUT_DOWNなら0
    right_down = (state == GLUT_DOWN) ? 1 : 0;
  }
}


void motion(int x, int y){
  //right_down中なら1、それ以外なら0
  if(right_down){
    is_display = 1;
    point_x = x;
    point_y = height - y;
    glutPostRedisplay();
  } else{
    is_display = 0;
  }  
}



void init(void){
  glClearColor(1.0, 1.0, 1.0, 0.0);
  right_down = 0;
  color = 0;
  is_display = 0;
  point_x = 0;
  point_y = 0;
}


int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize(800, 450);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(point);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  //glutIdleFunc(idle);
  init();
  glutMainLoop();

  return 0;
}




