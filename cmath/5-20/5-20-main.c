#include <stdio.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>


int width, height;

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  // A
  glColor3d(1.0, 0.6, 0.0);
  glBegin(GL_TRIANGLES);
  glVertex2d(200, 350);
  glVertex2d(115, 285);
  glVertex2d(285, 285);
  glEnd();

  glBegin(GL_TRIANGLES);
  glVertex2d(200, 330);
  glVertex2d(115, 265);
  glVertex2d(285, 265);
  glEnd();

  glColor3d(1.0, 1.0, 1.0);
  glBegin(GL_TRIANGLES);
  glVertex2d(200, 306);
  glVertex2d(145, 265);
  glVertex2d(255, 265);
  glEnd();

  glColor3d(1.0, 0.6, 0.0);
  glRectd(115, 85, 145, 285);
  glRectd(255, 85, 285, 285);

  glColor3d(1.0, 0.6, 0.0);
  glRectd(145, 170, 255, 200);


  // dot
  glPointSize(20.0);
  glBegin(GL_POINTS);
  glVertex2d(400, 95);
  glEnd();


  // M
  glRectd(480, 85, 510, 330);
  glRectd(690, 85, 720, 330);

  glBegin(GL_TRIANGLES);
  glVertex2d(600, 161);
  glVertex2d(480, 330);
  glVertex2d(720, 330);
  glEnd();

  glColor3d(1.0, 1.0, 1.0);
  glBegin(GL_TRIANGLES);
  glVertex2d(600, 200);
  glVertex2d(510, 330);
  glVertex2d(690, 330);
  glEnd();



  /*
  // 赤い線
  glColor3d(1.0, 0.0, 0.0);
  glBegin(GL_LINES);
  glVertex2d( 100, 100 );
  glVertex2d( 200, 200 );
  glEnd();
  
  // 黒い線（一筆書き
  glColor3d(0.0, 0.0, 0.0);
  glBegin(GL_LINE_STRIP);
  glVertex2d(20, 200);
  glVertex2d(80, 150);
  glVertex2d(120, 250);
  glVertex2d(40, 230);
  glVertex2d(40, 120);
  glEnd();
  
  // 白い三角形
  glColor3d(1.0, 1.0, 0.9);
  glBegin(GL_TRIANGLES);
  glVertex2d(0, 120);
  glVertex2d(0, 30);
  glVertex2d(700, 90);
  glEnd();
  
  // 緑の長方形
  glColor3d(0.0, 1.0, 0.0);
  glRectd(180, 20, 200, 40);
  
  // 太い線 
  glColor3d(1.0, 0.0, 0.0);
  glLineWidth(5.0);
  glBegin(GL_LINES);
  glVertex2d(100, 200);
  glVertex2d(200, 100);
  glEnd();
  */
  
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


void init(void){
  glClearColor(1.0, 1.0, 1.0, 0.0);
}


int main(int argc, char **argv){

  glutInit(&argc, argv);
  glutInitWindowSize(800, 450);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  init();

  glutMainLoop();
  return 0;
}