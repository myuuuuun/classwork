#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "matrix.h"


#define SPEED 0.2       /* オブジェクト移動の速度 */
#define Z_INIT  100     /* カメラ初期位置 */
#define SIZE 50
#define PI 3.141592

void init_usr(void);
void init(void);
void draw_XY_plane(double scale);

int width, height;                              /* ウィンドウの大きさ */
int left_down, right_down;              /* マウスボタンの状態 */
int mx0, my0;                                   /* マウス押下時のマウス座標 */
double z, z0;                                   /* 描画座標 Z軸原点位置 */
double angle_x, angle_y;                /* オブジェクトの回転量 */
double angle_x0, angle_y0;
double phase = 0;

/************************************/
/*** ↑ここより上の部分は変更禁止 ***/
/************************************/




void init_usr(void)
{
  return;
}

void display(void){
  int i;
  double co, si;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  /*
  glColor3d(1.0, 0.0, 0.0);
  glNormal3d(0, 0, -1);
  glBegin(GL_TRIANGLES);
  glVertex3d(0, SIZE, 0);
  glVertex3d(-SIZE, -SIZE/2.0, 0);
  glVertex3d(SIZE, -SIZE/2.0, 0);
  glEnd();
  
  glColor3d(1.0, 0.0, 1.0);
  glNormal3d(0, -2, 1);
  glBegin(GL_TRIANGLES);
  glVertex3d(0, 0, SIZE);
  glVertex3d(-SIZE, -SIZE/2.0, 0);
  glVertex3d(SIZE, -SIZE/2.0, 0);
  glEnd();
  
  glColor3d(0.0, 0.0, 1.0);
  glNormal3d(1.5, 1, 1);
  glBegin(GL_TRIANGLES);
  glVertex3d(0, SIZE, 0);
  glVertex3d(0, 0, SIZE);
  glVertex3d(SIZE, -SIZE/2.0, 0);
  glEnd();
  
  glColor3d(0.0, 1.0, 0.0);
  glNormal3d(-1.5, 1, 1);
  glBegin(GL_TRIANGLES);
  glVertex3d(0, SIZE, 0);
  glVertex3d(-SIZE, -SIZE/2.0, 0);
  glVertex3d(0, 0, SIZE);
  glEnd();
  */

  
  glColor3d(1.0, 0.0, 0.0);
  glutSolidTeapot(10);
  glTranslatef(0, 0, 0);


  
  for(i=0; i<20; i++){
    co = 30 * cos(phase + 2 * PI * i / 20);
    si = 30 * sin(phase + 2 * PI * i / 20);

    glTranslatef(-co, -si, 0);
    glColor3d(0.0, 1.0, 0.0);
    glutSolidSphere(3, 20, 20);
    glTranslatef(co, si, 0);
  }
  
  glutSwapBuffers();
}

void idle(void){
  phase += 0.01;
  glutPostRedisplay();
}



/****************************************************/
/*** ↓ここより下は特別な場合を除き変更の必要無し ***/
/****************************************************/

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  width = w;
  height = h;

        /* 透視法射影に設定 */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -h/(double)w, h/(double)w, 1.0, 250.0);
  glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y)
{
  switch (button) {
    case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      left_down = 1;
      mx0 = x;
      my0 = y;
    } else if (state == GLUT_UP) {
      left_down = 0;
      angle_x0 = angle_x;
      angle_y0 = angle_y;
    }
    break;
    case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN) {
      right_down = 1;
      mx0 = x;
      my0 = y;
    } else if (state == GLUT_UP) {
      right_down = 0;
      z0 = z;
    }
  }
}

void motion(int x, int y)
{
        /* オブジェクト移動量計算 */
  if (right_down) {
    z = z0 - (y - my0) * SPEED;
  } else if (left_down) {
    angle_x = angle_x0 + (y - my0);
    angle_y = angle_y0 + (x - mx0);
  }

        /* オブジェクト移動・回転 */
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -z);
  glRotatef(angle_x, 1.0, 0.0, 0.0);
  glRotatef(angle_y, 0.0, 1.0, 0.0);

  glutPostRedisplay();
}

void draw_XY_plane(double scale)
{
  int i;

        glColor3d(0.5, 0.5, 0.5);       /* メッシュの色 */
        glNormal3d(0.0, 0.0, 1.0);      /* メッシュの法線方向＝どちらから見たとき明るく見えるか */
  for (i = 0; i <= 10; i++) {
    glBegin(GL_LINES);
    glVertex2d(-scale, scale * (i/5.0-1) );
    glVertex2d(scale, scale * (i/5.0-1) );
    glEnd();
  }
  for (i = 0; i <= 10; i++) {
    glBegin(GL_LINES);
    glVertex2d(scale * (i/5.0-1), -scale);
    glVertex2d(scale * (i/5.0-1), scale);
    glEnd();
  }
}

void init(void)
{
  GLfloat light_position[] = {20.0, 20.0, 100.0, 0.0};    /* 照明の位置：必要に応じて変更すること */

  glClearColor(0.0, 0.0, 0.0, 0.0);       /* 背景色は 黒：必要に応じて変更すること */
  glShadeModel(GL_SMOOTH);

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  angle_x0 = angle_x = 0;
  angle_y0 = angle_y = 0;
  z0 = z = Z_INIT;
  right_down = left_down = 0;
  motion(0,0);
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize(800, 800);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutIdleFunc(idle);     /* アニメーションをするなら，コメントを外す */
  init();                         /* テンプレートプログラムの初期化処理 */
  init_usr();                     /* ユーザ用の初期化処理 */
  glutMainLoop();
  return 0;
}



