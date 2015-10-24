#include <stdio.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "matrix.h"
#define POINT 5


int width, height;
matrix points;
int next;

void init(void);
double lagrange(double x, matrix points);


void display(void){
  int i, x;
  float red, green, blue;

  glClear(GL_COLOR_BUFFER_BIT);

  // 点を表示
  for (i = 1; i <= POINT; i++) {
    glBegin(GL_POINTS);
    glColor3d(0.3, 0.3, 0.3);
    glVertex2d(mat_elem(points, i, 1), mat_elem(points, i, 2));
    glEnd();
  }

  // 線を表示
  if(*points.element[4] != 0.0){
    glBegin(GL_LINE_STRIP);
    for(x=0; x<width; x++){
      green = 0.5 * x / (width + 1);
      glColor3d(1.0, green, 0.0);
      glVertex2d(x, lagrange(x, points));
    }
    glEnd();
  }

  glFlush();
}


void reshape(int w, int h){
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
  width = w;
  height = h;
}


void mouse(int button, int state, int x, int y){
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      mat_elem(points, next, 1) = x;
      mat_elem(points, next, 2) = height - y;
      next++;
      if (next > POINT) next = 1;
      glutPostRedisplay();
    }
  }
}


void init(void){
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glPointSize(5.0);
  if ( mat_alloc(&points, POINT, 2) ) exit (-1);
  next = 1;
}


double lagrange(double x, matrix points){
  int i, j;
  double sigma, product;
  
  sigma = 0;
  for(i=0; i<points.row; i++){
    product = points.element[i][1];

    for(j=0; j<points.row; j++){
      if(i != j){
        product *= (x - points.element[j][0]) / (points.element[i][0] - points.element[j][0]);
      }
    }
    sigma += product;
  }
  return sigma;
}


int main(int argc, char **argv){
  glutInit(&argc, argv);
  glutInitWindowSize(800, 450);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  init();
  glutMainLoop();

  return 0;
}










