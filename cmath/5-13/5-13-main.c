/* ���̃t�@�C���̒��g�͏��������Ȃ����� */
/* ���g�̋�̓I�Ȃ��Ƃ͗��T�ȍ~�̎��Ƃ� */

#include <stdio.h>
#include <GLUT/glut.h>
#include "matrix.h"

#define ORDER 3        /* �ߎ��֐��̎��� */
#define WIDTH 400      /* ������ʂ̕� */
#define HEIGHT 400      /* ������ʂ̍��� */
#define MIN_X -10      /* ������ʂ̍��[���W */
#define MAX_X 10      /* ������ʂ̉E�[���W */
#define MIN_Y -10      /* ������ʂ̉��[���W */
#define MAX_Y 10      /* ������ʂ̏�[���W */
#define TICK_LEN 0.1    /* �ڐ���T�C�Y */
#define TICK_INTERVAL 1    /* �ڐ���Ԋu */
#define POINT_SIZE  0.01  /* �_��\���T�C�Y */

#define Number(arr)  ((int)(sizeof(arr)/sizeof(arr[0])))

/* ���U�_��̍��W */
double p[][2] = {
  {0.0, 0.0},
  {2.0, -5.1},
  {4.0, -9.5},
  {6.0, -6.9},
  {8.0, 7.0},
  {10.0, 38.0},
};

/* �֐��v���g�^�C�v�錾 */
extern int least_square(matrix *a, matrix points, int order);

double func(double x);
void init(void);
void set_ortho(void);
void DrawAxes(double x1, double x2, double y1, double y2);
void DrawTicks(double x1, double x2, double y1, double y2, double interval);
void DrawPoints(double x1, double y1);

/* �O���[�o���ϐ��錾 */
double dx;
double min_x, min_y, max_x, max_y;
int x_orgn, y_orgn, left_down, right_down;
int width, height;
double speed_x, speed_y;
int samples;
matrix a, points;

/* �ŏ��Q��ߎ��֐� */
double func(double x)
{
  int i;
#ifndef FROM0
  double y = mat_elem(a, ORDER+1, 1);
  for (i = ORDER; i >= 1; i--)
    y = y * x + mat_elem(a, i, 1);
#else
  double y = mat_elem(a, ORDER, 0);
  for (i = ORDER-1; i >= 0; i--)
    y = y * x + mat_elem(a, i, 0);
#endif
  return y;
}

void init(void)
{
  int i;
  
  min_x = MIN_X; max_x = MAX_X;
  min_y = MIN_Y; max_y = MAX_Y;
  set_ortho();
  left_down = 0;
  width = WIDTH;
  height = HEIGHT;
  
  if ( mat_alloc(&a, ORDER+1, 1) || mat_alloc(&points, Number(p), 2) ) {
    printf("Allocation error.\n");
    exit(-1);
  }
#ifndef FROM0
  for (i = 1; i <= Number(p); i++) {
    mat_elem(points, i, 1) = p[i-1][0];
    mat_elem(points, i, 2) = p[i-1][1];
  }
#else
  for (i = 0; i < Number(p); i++) {
    mat_elem(points, i, 0) = p[i][0];
    mat_elem(points, i, 1) = p[i][1];
  }
#endif
}

void set_ortho(void)
{
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(min_x, max_x, min_y, max_y, -1.0, 1.0);
  samples = width;
  dx = (max_x - min_x) / (double)samples;
  speed_x = (max_x - min_x) / (double)width;
  speed_y = (max_y - min_y) / (double)height;
}

void DrawAxes(double x1, double x2, double y1, double y2)
{
  glColor3f(0.5, 0.5, 0.0);
  glBegin(GL_LINES);
  glVertex2d(x1, 0);
  glVertex2d(x2, 0);
  glEnd();
  glBegin(GL_LINES);
  glVertex2d(0, y1);
  glVertex2d(0, y2);
  glEnd();
}

void DrawTicks(double x1, double x2, double y1, double y2, double interval)
{
  double i, min_tick;
  glColor3f(0.5, 0.0, 0.0);
  min_tick = (int)(x1 / interval) * interval;
  for (i = min_tick; i <= x2; i+=interval) {
    glBegin(GL_LINES);
    glVertex2d(i, -TICK_LEN);
    glVertex2d(i, TICK_LEN);
    glEnd();
  }
  min_tick = (int)(y1 / interval) * interval;
  for (i = min_tick; i <= y2; i+=interval) {
    glBegin(GL_LINES);
    glVertex2d(-TICK_LEN, i);
    glVertex2d(TICK_LEN, i);
    glEnd();
  }
}

void DrawPoints(double x1, double y1)
{
  double w, h;
  w = (max_x - min_x) * POINT_SIZE;
  h = (max_y - min_y) * POINT_SIZE;
  glBegin(GL_LINES);
  glVertex2d(x1 - w, y1 - h);
  glVertex2d(x1 + w, y1 + h);
  glEnd();
  
  glBegin(GL_LINES);
  glVertex2d(x1 - w, y1 + h);
  glVertex2d(x1 + w, y1 - h);
  glEnd();
}

void display(void)
{
  double i;
  int j;
  
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  
  DrawAxes(min_x, max_x, min_y, max_y);
  DrawTicks(min_x, max_x, min_y, max_y, TICK_INTERVAL);
  glColor3f(0.0, 0.0, 0.0);
  for (i = min_x; i < max_x+dx; i+=dx) {
    glBegin(GL_LINES);
    glVertex2d( i - dx/2., func(i-dx/2.));
    glVertex2d( i + dx/2., func(i+dx/2.));
    glEnd();
  }
  glColor3f(1.0, 0.0, 0.0);
  for (j = 0; j < Number(p); j++) {
    DrawPoints(p[j][0], p[j][1]);
  }
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  width = w; height = h;
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  set_ortho();
}

void mouse(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN) {
    x_orgn = x; y_orgn = y;
  }
  left_down = (button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN);
  right_down = (button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN);
}

void motion(int x, int y)
{
  if (left_down) {
    min_x -= (x - x_orgn) * speed_x;
    max_x -= (x - x_orgn) * speed_x;
    min_y += (y - y_orgn) * speed_y;
    max_y += (y - y_orgn) * speed_y;
    x_orgn = x; y_orgn =y;
  } else if (right_down) {
    min_x -= (x - x_orgn) * speed_x;
    max_x += (x - x_orgn) * speed_x;
    min_y += (y - y_orgn) * speed_y;
    max_y -= (y - y_orgn) * speed_y;
    x_orgn = x; y_orgn = y;
  }
  set_ortho();
  glutPostRedisplay();
}

int main(int argc, char *argv[])
{
  int err_code;
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow(argv[0]);
  init();
  
  err_code = least_square(&a, points, ORDER);
  if (err_code) {
    printf("Cannot solve. [err_code = %d]\n", err_code);
    exit(-1);
  }
  mat_print(a);
  
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  
  glutMainLoop();
  return 0;
}
