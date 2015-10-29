#include <iostream>
#include <array>
#include <cmath>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "matrix.h"
#include "uglyfont.h"
#define EPSIRON 1e-5
#define POINT 5
#define SPEED 0.01
using namespace std;

/*
int main(){
  double r, x1, y1, x2, y2, vx1, vy1, vx2, vy2;

  r = 1;

  x1 = 0;
  y1 = 0;
  vx1 = 0.1;
  vy1 = 0;

  x2 = 10;
  y2 = 0;
  vx2 = -0.1;
  vy2 = 0;

  double step = 0.01;
  for(long t=0; t<10000; t+=1){
    long distance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    if(distance < 2*r + EPSIRON){
      cout << "clash! time is " << t << endl;
      break;
    }

    x1 += step * vx1;
    y1 += step * vy1;
    x2 += step * vx2;
    y2 += step * vy2;
  }

  cout << "finish" << endl;

  return 0;
}
*/

// 円を描画する
void circle(double center_x, double center_y, double radius, array<double, 3> &color){
  int i, n = 200;
  double x, y;

  glEnable(GL_LINE_STIPPLE);
  //glLineStipple(1, 0xACF3);
  glBegin(GL_POLYGON);
  glColor3d(color[0], color[1], color[2]);
  // 円周上の座標(x,y)を計算して円を描画
  for (i = 0; i < n; i++) {
    x = radius * cos(2.0 * 3.14 * ((double)i / n) ) + center_x;
    y = radius * sin(2.0 * 3.14 * ((double)i / n) ) + center_y;
    glVertex3f(x, y, 0.0);
  }
  glEnd();
}

// 衝突判定
bool is_clash()


void init(void);

int width, height; //ウィンドウの大きさ
int current_time; //
#define TIME_MAX 10000

void display(void)
{
  double x_center, y_center;
  x_center = width / 2.0;
  y_center = height / 2.0;

  glClear(GL_COLOR_BUFFER_BIT);
  array<double, 3> c = {0.0, 0.3, 0.3};
  circle(100, 100, 100, c);



  glPushMatrix();
  glTranslatef(200, 200, 0);
  glScalef(10.0, 10.0, 1.0); 
  YsDrawUglyFont("UPSIDE-DOWN", 0);
  glPopMatrix();



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
  if(current_time < TIME_MAX){
    current_time += 1;
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
