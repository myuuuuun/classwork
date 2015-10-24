#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "matrix.h"
#define POINT 5


typedef struct point_list {
  int x;
  int y;
  struct point_list *next;
} point_list;

point_list *point_root = NULL;


void init(void);
int new_point(int x, int y);
void free_point(point_list *point);
int show_pointer_list(void);
int delete_point(int deleteX, int deleteY);

int width, height;


void display(void){
  int i, x;
  point_list *point;

  // 画面表示をリセット
  glClear(GL_COLOR_BUFFER_BIT);

  // 点を表示
  for (point = point_root; point != NULL; point = point->next){
    glBegin(GL_POINTS);
    glColor3d(1.0, 0.6, 0.0);
    glVertex2d(point->x, point->y);
    glEnd();
  }

  // 線を表示
  if(point_root != NULL && point_root->next != NULL){
    glBegin(GL_LINE_STRIP);
    for (point = point_root; point != NULL; point = point->next){
      glColor3d(1.0, 0.6, 0.0);
      glVertex2d(point->x, point->y);
    }
    glEnd();
  }

  // 点と線を画面に描写
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
  point_list *point;
  point_list *before;

  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      new_point(x, height - y);
      show_pointer_list();
      glutPostRedisplay();
    }
  } else if (button == GLUT_RIGHT_BUTTON) {
    if (state == GLUT_DOWN) {
      delete_point(x, height-y);
      glutPostRedisplay();
    }
  }
}


void init(void){
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glPointSize(5.0);
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


// point_list型のアドレスを確保
int new_point(int x, int y){
  point_list *new_point;

  new_point = calloc(1, sizeof(point_list));
  if (new_point == NULL){
    printe("配列用の領域が確保できませんでした。 >>new_point()");
    return -1;
  }
  new_point->x = x;
  new_point->y = y;
  new_point->next = point_root;
  point_root = new_point;
  
  return 0;
}


// point_list型のメモリを解放
void free_point(point_list *point){
  free(point->next);
}


int show_pointer_list(void){
  point_list *point;

  printf("\nPointer List: \n");
  for (point = point_root; point != NULL; point = point->next){
    printf("%p -> ", point);
  }
  printf("NULL\n");

  return 0;
}


int delete_point(int deleteX, int deleteY){
  point_list *point;
  point_list *before;
  before = NULL;

  printf("\n----------SearchDeletePoint Start----------\n");
  printf("deleteX: %d±3, deleteY: %d±3\n\n", deleteX, deleteY);

  for (point = point_root; point != NULL; point = point->next){
    printf("Before: %p\n", before);
    printf("Current: %p, CurrentX: %d, CurrentY: %d\n", point, point->x, point->y);
    printf("Next: %p\n\n", point->next);
    if(abs(point->x - deleteX) <= 3 && abs(point->y - deleteY) <= 3){
      if(before != NULL){
        printf("Point Deleted. Before is not NULL.\n");
        before->next = point->next;
        break;
      } else{
        printf("Point Deleted. Before is NULL.\n");
        point_root = point->next;
        break;
      }
      free_point(point);
    } else{
      before = point;
    }
  }
  printf("----------SearchDeletePoint Stop----------\n");

  show_pointer_list();
  return 0;
}







