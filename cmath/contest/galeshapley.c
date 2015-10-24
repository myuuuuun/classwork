#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#define RED "\033[31m" // 赤字
#define BOLD "\033[1m" // 太字
#define STANDARD "\033[0m" // 標準文字色・スタイル
#define STANDARD_C "\033[0m" // 標準文字色
#define MAT_EPSILON 1.0e-8
#define POINT 5


void sleep(int speed)
{
  clock_t c1=clock(),c2;
  do{
  if((c2=clock())==(clock_t)-1)
  break;
  }while(1000*(c2-c1)/CLOCKS_PER_SEC<speed);
}

typedef struct{
  int row;
  int col;
  int **element;
  int *base_element;
} matrix;


typedef struct{
  int row;
  int col;
  double **element;
  double *base_element;
} dmatrix;


typedef struct{
  int data;
  struct stack *next;
} stack;


typedef struct{
  int data;
  int order;
  struct ordered_node *next;
} ordered_node;


typedef struct{
  int length;
  struct ordered_node *node_root;
} ordered_list;


typedef struct{
  int x;
  int y;
  struct point_list *next;
} point_list;

point_list *point_root = NULL;


double f1(matrix score);
double f2(matrix score);

void make_scores(matrix *table, double ave, double var);
void sort_scores(matrix scores, double (*f)(matrix), matrix *ordered);
int double_sort(const void *a , const void *b);
void make_student_preferences(matrix *student_preferences);
void shuffle(matrix *mat);
int galeshapley(matrix student_preferences, matrix department_preferences, matrix capacity, 
                matrix *student_matchings, matrix *department_matchings);

// ソート
void argsort(matrix preferences, matrix *ordered);
int int_sort(const void *a, const void *b);

// スタック類
int pop(stack **stack_root);
void push(stack **stack_root, int n);
void stack_free(stack **stack_root);

// 順序付き線形リスト
void delete(ordered_list **ordered_list, int data);
void append(ordered_list **ordered_list, int data, int order);
void free_ordered_list(ordered_list **ordered_list);
int ref_ordered_list(ordered_list **ordered_list, int n);
int ref_ordered_list_order(ordered_list **ordered_list, int n);

// 描画系
void display(void);
void reshape(int w, int h);
void idle(void);
void init(void);

// 行列
void printe(char message[99]);
int mat_alloc(matrix *mat, int row, int col);
int dmat_alloc(dmatrix *mat, int row, int col);
void mat_free(matrix *mat);
void dmat_free(dmatrix *mat);
void mat_print(matrix mat);
void dmat_print(dmatrix mat);
int check(matrix mat1, matrix mat2);
int check_mul(matrix mat1, matrix mat2, matrix mat3);
int check_trans(matrix mat1, matrix mat2);
int check_square(matrix mat);
int mat_copy(matrix *mat1, matrix mat2);
int mat_row_copy(matrix *mat1, matrix mat2, int k);
int mat_trans(matrix *mat1, matrix mat2);

// 乱数
double standard_uniform_rv(void);
double standard_normal_rv(void);
double normal_rv(double mu, double sigma);


int window_width = 800;
int window_height = 1000;
int width, height;
int student_size = 50;
int department_size = 4;
int subject_size = 5;


matrix student_preferences, department_preferences, capacity, student_scores, ordered_students, ordered_students2, 
       student_matchings, department_matchings, ordered_student_preferences;

int sp_row, sp_col, dp_row, dp_col, 
    unmatch_department, unmatch_student;

int *matched_num, *worst_matched_indexes, *candidate_preference;
ordered_node *node;
ordered_list **department_matched_list;
stack *stack1 = NULL;



int main(int argc, char **argv){
  srand(time(0));
  int i, j, d;

  // 生徒側には拒否権あり
  mat_alloc(&student_preferences, student_size, department_size+1);
  // 学部側には拒否権なし
  mat_alloc(&department_preferences, department_size, student_size);
  mat_alloc(&student_scores, student_size, subject_size);
  mat_alloc(&ordered_students, 1, student_size);
  mat_alloc(&ordered_students2, 1, student_size);
  mat_alloc(&capacity, 1, department_size);
  mat_alloc(&student_matchings, 1, student_size);
  mat_alloc(&department_matchings, department_size, student_size);
  mat_alloc(&ordered_student_preferences, student_size, department_size+1);

  // 得点表
  make_scores(&student_scores, 75.0, 10.0);
  printf("\n生徒の得点は\n");
  printf("  ma   en   fr   ch   ph\n");
  mat_print(student_scores);

  // 学部の選好表
  sort_scores(student_scores, f1, &ordered_students);
  sort_scores(student_scores, f2, &ordered_students2);
  for(i=0; i<department_size-1; i++){
    mat_row_copy(&department_preferences, ordered_students, i);
  }
  mat_row_copy(&department_preferences, ordered_students2, department_size-1);
  printf("各学部の選好表は\n");
  mat_print(department_preferences);

  // 生徒の選好表
  make_student_preferences(&student_preferences);
  printf("生徒の選好表は\n");
  mat_print(student_preferences);
  mat_copy(&ordered_student_preferences, student_preferences);
  argsort(student_preferences, &student_preferences);

  // 学部の定員
  capacity.element[0][0] = 5;
  capacity.element[0][1] = 5;
  capacity.element[0][2] = 5;
  capacity.element[0][3] = 2;

  sp_row = student_preferences.row;
  sp_col = student_preferences.col; // アンマッチマークを含む
  dp_row = department_preferences.row;
  dp_col = department_preferences.col;

  if(sp_row != dp_col || sp_col-1 != dp_row){
    printe("引数の行列のサイズが一致しません。 >>galeshapley()");
    return -1;
  }

  student_size = sp_row;
  department_size = dp_row;

  unmatch_department = sp_col - 1; // 選好表の行で1番大きい数がunmatch
  unmatch_student = dp_col;  // 選好表の行で1番大きい数+1がunmatch

  // 選好表を初期化
  for(j=0; j<student_size; j++){
    student_matchings.element[0][j] = unmatch_department;
  }

  for(i=0; i<department_size; i++){
    for(j=0; j<student_size; j++){
      department_matchings.element[i][j] = -1;
    }
  }

  // 学部の受入者リストを初期化
  department_matched_list = calloc(department_size, sizeof(ordered_list*));
  for(i=0; i<department_size; i++){
    department_matched_list[i] = calloc(1, sizeof(ordered_list));
    department_matched_list[i]->node_root = calloc(1, sizeof(ordered_node));
    department_matched_list[i]->length = 0;
    department_matched_list[i]->node_root = NULL;
  }

  // スタックを初期化
  for(j=0; j<student_size; j++){
    push(&stack1, j);
  }

  // 学部の選好表の各行を、ランキング順→学生の番号順に並び替え
  argsort(department_preferences, &department_preferences);
  
  // Gale-Shapley実行
  glutInit(&argc, argv);
  glutInitWindowSize(window_width, window_height);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  init();
  glutMainLoop();


  mat_print(student_matchings);
  mat_print(department_matchings);

  mat_free(&student_preferences);
  mat_free(&department_preferences);
  mat_free(&student_scores);
  mat_free(&ordered_students);
  mat_free(&ordered_students2);
  mat_free(&capacity);
  mat_free(&student_matchings);
  mat_free(&department_matchings);
  mat_free(&ordered_student_preferences);

  return 0;
}


void display(void){
  int i, x, right, left, student, department, rank, length, worst_rank;
  point_list *point;

  right = window_width - 100;
  left = 100;

  // 画面表示をリセット
  glClear(GL_COLOR_BUFFER_BIT);


  for(i=0; i<student_size; i++){
    student = i;
    department = student_matchings.element[0][i];

    if(department != unmatch_department){
      glBegin(GL_LINE_STRIP);
      glColor3d(0.8, 0.8, 0.8);
      glVertex2d(left, 50 + (window_height - 100.0) / (student_size - 1) * student);
      glVertex2d(right, 100 + (window_height - 200.0) / (department_size - 1) * department);
      glEnd();
    }
  }
  
  // 生徒側の点を表示
  glPointSize(5.0);
  for (i=0; i<student_size; i++){
    glBegin(GL_POINTS);
    department = student_matchings.element[0][i];
    rank = ordered_student_preferences.element[i][department];
    if(department == unmatch_department || rank > 2){
      glColor3d(1.0, 0.0, 0.0);
    }
    else if(rank > 1){
      glColor3d(1.0, 0.5, 0.0);
    }
    else{
      glColor3d(0.0, 1.0, 0.0);
    }
    
    glVertex2d(left, 50 + (window_height - 100.0) / (student_size - 1) * i);
    glEnd();
  }

  // 学部側の点を表示
  glPointSize(10.0);

  for (i=0; i<department_size; i++){
    glBegin(GL_POINTS);
    length = department_matched_list[i] -> length;

    worst_rank = ref_ordered_list_order(&department_matched_list[i], -1);
    printf("学部%dのworst_rank: %d\n", i, worst_rank);

    if(length < capacity.element[0][i] || worst_rank > student_size * 4.0 / 5){
      glColor3d(1.0, 0.0, 0.0);
    }
    else if(worst_rank > department_size * 2.0 / 5){
      glColor3d(1.0, 0.5, 0.0);
    }
    else{
      glColor3d(0.0, 1.0, 0.0);
    }
    
    glVertex2d(right, 100 + (window_height - 200.0) / (department_size - 1) * i);
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

void idle(void){
  int i, j, d;
  int applicant, student, department, candidate, matched, worst_matched, worst_matched_rank, applicant_rank, candidate_rank;
  while(stack1 != NULL){
    // stackから1人studentをとり出す
    applicant = pop(&stack1);
    printf("\n生徒: %d\n", applicant);
    sleep(300);
    // 生徒の選好表から1つdepartmentをとり出す
    for(d=0; d<sp_col; d++){
      //sleep(1000);
      candidate = student_preferences.element[applicant][d];
      candidate_preference = department_preferences.element[candidate];
      printf("大学: %d に申し込みます\n", candidate);

      // アンマッチ・マークならその人の処理は終了
      if(candidate == unmatch_department){
        printf("candidateはアンマッチで終了です\n");
        break;
      }

      // 定員に達していなければ受け入れる
      else if(department_matched_list[candidate]->length < capacity.element[0][candidate]){
        // スタックを更新
        append(&department_matched_list[candidate], applicant, candidate_preference[applicant]);
        // マッチングを更新
        student_matchings.element[0][applicant] = candidate;
        printf("candidateを受け入れます\n");
        goto end;
      }

      // 既に定員に達していれば、一番ランクの低い人と比較
      else if(department_matched_list[candidate]->length == capacity.element[0][candidate]){
        worst_matched = ref_ordered_list(&department_matched_list[candidate], -1);
        worst_matched_rank = ref_ordered_list_order(&department_matched_list[candidate], -1);
        applicant_rank = candidate_preference[applicant];
        printf("applicant: %d - %d位, worst_matched: %d - %d位\n", applicant, applicant_rank, worst_matched, worst_matched_rank);
        
        if(applicant_rank < worst_matched_rank){
          // matched_listを更新
          delete(&department_matched_list[candidate], worst_matched);
          append(&department_matched_list[candidate], applicant, applicant_rank);
          // スタックにworst_matchedを戻す
          push(&stack1, worst_matched);
          // マッチングを更新
          student_matchings.element[0][applicant] = candidate;
          student_matchings.element[0][worst_matched] = unmatch_department;
          printf("現在の受け入れ者を1人削除してapplicantを受け入れます\n");
          goto end;
        }
        printf("この大学とはマッチングしませんでした。次のループへ\n");
      }
    }
  }
  if(stack1 == NULL){
    printf("\n-------------------------\n       終了しました       \n-------------------------\n");
    sleep(100000);
    exit(-1);
  }
  
  end:
    glutPostRedisplay();
}

void init(void){
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glPointSize(4.0);
}


// 単純合計点で評価
double f1(matrix score){
  int i, size;
  double rst;

  size = score.col;
  rst = 0;
  for(i=0; i<size; i++){
    rst += score.element[0][i];
  }

  return rst;
}


// 1科目目のウェイトを5、他を1として評価
double f2(matrix score){
  int i, size;
  double rst;

  size = score.col;
  rst = score.element[0][0] * 5.0;
  for(i=1; i<size; i++){
    rst += score.element[0][i];
  }

  return rst;
}


// 平均ave, 分散varの正規分布からのランダムサンプルを用いて得点表を作る
void make_scores(matrix *table, double ave, double var){
  int i, j, col, row;
  int score;

  row = table->row;
  col = table->col;

  for(i=0; i<row; i++){
    for(j=0; j<col; j++){
      score = floor(normal_rv(ave, var));
      if(score > 100){
        score = 100;
      }
      if(score < 0){
        score = 0;
      }
      table->element[i][j] = score;
    }
  }
}


// 適当な関数f（引数: scores  返値: double）の値が大きい順に、生徒を並び替える
// scores: (生徒数×科目数), ordered: (1×生徒数)
void sort_scores(matrix scores, double (*f)(matrix), matrix *ordered){
  int i, j, row, col;
  matrix score;
  dmatrix points;

  row = scores.row;
  col = scores.col;
  mat_alloc(&score, 1, col);
  dmat_alloc(&points, row, 2);

  for(i=0; i<row; i++){
    for(j=0; j<col; j++){
      score.element[0][j] = scores.element[i][j];
    }
    points.element[i][0] = f(score);
    points.element[i][1] = i;
  }

  qsort(points.base_element, row, sizeof(double) * 2, double_sort);

  for(i=0; i<row; i++){
    ordered->element[0][i] = points.element[i][1];
  }

  mat_free(&score);
  dmat_free(&points);
}


// quicksort用ソート関数
// elementの0列目でソート
int double_sort(const void *a, const void *b){
  double *p, *q, r;

  p = (double*) a;
  q = (double*) b;
  r = p[0] - q[0];

  // 型キャスト
  // 降順に並び替え
  if(r > 0){
    return -1;
  }
  else if(r == 0){
    return 0;
  }
  else{
    return 1;
  }
}


// 学部側の選好表を作る
void make_department_preferences(matrix *department_preferences){
  //
}


void make_student_preferences(matrix *student_preferences){
  int col, row, i;
  matrix base_mat, shuffled;

  row = student_preferences -> row;
  col = student_preferences -> col;
  mat_alloc(&base_mat, 1, col);
  mat_alloc(&shuffled, 1, col);

  for(i=0; i<col; i++){
    base_mat.element[0][i] = i;
  }

  for(i=0; i<row; i++){
    mat_copy(&shuffled, base_mat);
    shuffle(&shuffled);
    mat_row_copy(student_preferences, shuffled, i);
  }

  mat_free(&base_mat);
  mat_free(&shuffled);
}


void shuffle(matrix *mat){
  int i, size, j, t;

  size = mat->col;

  for(i=0; i<size; i++){
    j = rand() % size;
    t = mat->element[0][i];
    mat->element[0][i] = mat->element[0][j];
    mat->element[0][j] = t;
  }
}


int galeshapley(matrix student_preferences, matrix department_preferences, matrix capacity, 
                matrix *student_matchings, matrix *department_matchings)
{
  int i, j, k, m, n, s, applicant, d, sp_row, sp_col, dp_row, dp_col, student, department, candidate, matched, 
    student_size, department_size, unmatch_department, unmatch_student,
    worst_matched, worst_matched_rank, applicant_rank, candidate_rank;

  int *matched_num, *worst_matched_indexes, *candidate_preference;
  ordered_node *node;

  sp_row = student_preferences.row;
  sp_col = student_preferences.col; // アンマッチマークを含む
  dp_row = department_preferences.row;
  dp_col = department_preferences.col;

  if(sp_row != dp_col || sp_col-1 != dp_row){
    printe("引数の行列のサイズが一致しません。 >>galeshapley()");
    return -1;
  }

  student_size = sp_row;
  department_size = dp_row;

  unmatch_department = sp_col - 1; // 選好表の行で1番大きい数がunmatch
  unmatch_student = dp_col;  // 選好表の行で1番大きい数+1がunmatch

  // 選好表を初期化
  for(j=0; j<student_size; j++){
    student_matchings->element[0][j] = unmatch_department;
  }

  for(i=0; i<department_size; i++){
    for(j=0; j<student_size; j++){
      department_matchings->element[i][j] = -1;
    }
  }

  // 学部の受入者リストを初期化
  ordered_list **department_matched_list;
  department_matched_list = calloc(department_size, sizeof(ordered_list*));
  
  for(i=0; i<department_size; i++){
    department_matched_list[i] = calloc(1, sizeof(ordered_list));
    department_matched_list[i]->node_root = calloc(1, sizeof(ordered_node));
    department_matched_list[i]->length = 0;
    department_matched_list[i]->node_root = NULL;
  }
  
  // スタックを初期化
  stack *stack = NULL;
  for(j=0; j<student_size; j++){
    push(&stack, j);
  }
  
  // 学部の選好表の各行を、ランキング順→学生の番号順に並び替え
  argsort(department_preferences, &department_preferences);

  // メイン関数
  while(stack != NULL){
    // stackから1人studentをとり出す
    applicant = pop(&stack);
    printf("\n生徒: %d\n", applicant);
    //sleep(1000);
    // 生徒の選好表から1つdepartmentをとり出す
    for(d=0; d<sp_col; d++){
      //sleep(1000);
      candidate = student_preferences.element[applicant][d];
      candidate_preference = department_preferences.element[candidate];
      printf("大学: %d に申し込みます\n", candidate);

      // アンマッチ・マークならその人の処理は終了
      if(candidate == unmatch_department){
        printf("candidateはアンマッチで終了です\n");
        break;
      }

      // 定員に達していなければ受け入れる
      else if(department_matched_list[candidate]->length < capacity.element[0][candidate]){
        // スタックを更新
        append(&department_matched_list[candidate], applicant, candidate_preference[applicant]);
        // マッチングを更新
        student_matchings->element[0][applicant] = candidate;
        printf("candidateを受け入れます\n");
        break;
      }

      // 既に定員に達していれば、一番ランクの低い人と比較
      else if(department_matched_list[candidate]->length == capacity.element[0][candidate]){
        worst_matched = ref_ordered_list(&department_matched_list[candidate], -1);
        worst_matched_rank = ref_ordered_list_order(&department_matched_list[candidate], -1);
        applicant_rank = candidate_preference[applicant];
        printf("applicant: %d - %d位, worst_matched: %d - %d位\n", applicant, applicant_rank, worst_matched, worst_matched_rank);
        
        if(applicant_rank < worst_matched_rank){
          // matched_listを更新
          delete(&department_matched_list[candidate], worst_matched);
          append(&department_matched_list[candidate], applicant, applicant_rank);
          // スタックにworst_matchedを戻す
          push(&stack, worst_matched);
          // マッチングを更新
          student_matchings->element[0][applicant] = candidate;
          student_matchings->element[0][worst_matched] = unmatch_department;
          printf("現在の受け入れ者を1人削除してapplicantを受け入れます\n");
          break;
        }
        printf("この大学とはマッチングしませんでした。次のループへ\n");
      }
    }
  }

  for(i=0; i<department_size; i++){
    j=0;
    for(node=department_matched_list[i]->node_root; node != NULL; node = node->next){
      department_matchings->element[i][j] = node->data;
      j++;
    }
  }
  
  stack_free(&stack);
  free_ordered_list(&department_matched_list);

  return 0;
}


// matrixを行ごとにソートして、もとのインデックスを返す
void argsort(matrix preferences, matrix *ordered){
  int i, j, r, row, col;
  matrix value_index;

  row = preferences.row;
  col = preferences.col;
  mat_alloc(&value_index, col, 2);

  for(i=0; i<row; i++){
    for(j=0; j<col; j++){
      value_index.element[j][0] = preferences.element[i][j];
      value_index.element[j][1] = j;
    }
    qsort(value_index.base_element, col, sizeof(int) * 2, int_sort);
    for(j=0; j<col; j++){
      ordered->element[i][j] = value_index.element[j][1];
    }
  }

  mat_free(&value_index);
}


// quicksort用ソート関数
// elementの0列目でソート
int int_sort(const void *a, const void *b){
  int *p, *q, r;

  p = (int*) a;
  q = (int*) b;
  r = p[0] - q[0];

  // 型キャスト
  // 昇順に並び替え
  if(r < 0){
    return -1;
  }
  else if(r == 0){
    return 0;
  }
  else{
    return 1;
  }
}


// stackの先頭の要素をとり出す
int pop(stack **stack_root){
  int n;
  stack *next, *fr;

  next = calloc(1, sizeof(stack));
  if(next == NULL){
    printf("ERROR\n");
    exit(-1);
  }

  // NULLポインタならエラー(-1)を返す
  if(*stack_root == NULL){
    return -1;
  }

  n = (*stack_root) -> data;
  next = (*stack_root) -> next;
  fr = *stack_root;
  *stack_root = next;
  free(fr);

  return n;
}


// stackの先頭に要素を追加
void push(stack **stack_root, int n){
  stack *new_stack;

  new_stack = calloc(1, sizeof(stack));
  if(new_stack == NULL){
    printf("ERROR\n");
    exit(-1);
  }

  new_stack->data = n;
  new_stack->next = *stack_root;
  *stack_root = new_stack;
}


// stackのメモリを解放
void stack_free(stack **stack_root){
  int i;
  stack *s, *fr;

  s = *stack_root;
  while(s != NULL){
    fr = s;
    s = s->next;
    *stack_root = s;
    free(fr);
  }
}


// ordered listの要素を削除
void delete(ordered_list **ordered_list, int data){
  int n;
  ordered_node *node, *before;

  node = (*ordered_list)->node_root;
  if(node != NULL && node->data == data){
    (*ordered_list)->length--;
    (*ordered_list)->node_root = node->next;
    free(node);
  }
  else{
    while(1){
      if(node == NULL){
        printf("指定されたデータが見つかりませんでした\n");
        break;
      }

      if(node->data == data){
        (*ordered_list)->length--;
        before->next = node->next;
        free(node);
        break;
      }

      before = node;
      node = node->next;
    }
  }
}


// ordered listに要素を追加
void append(ordered_list **ordered_list, int data, int order){
  int i;
  ordered_node *new_node, *node, *before, *a;

  new_node = calloc(1, sizeof(ordered_node));
  if(new_node == NULL){
    printf("メモリ確保に失敗しました\n");
    exit(-1);
  }

  new_node->data = data;
  new_node->order = order;
  node = (*ordered_list)->node_root;
  (*ordered_list)->length++;

  // 最初がNULLポインタか、最初の要素よりもorderが小さければ、先頭を更新
  // beforeが無いので別処理
  if(node == NULL || order < node->order){
    new_node->next = node;
    (*ordered_list)->node_root = new_node;
  }

  // そうでなければ、次以降を比べていく
  else{
    before = node;
    node = node->next;

    while(1){
      if(node == NULL || order < node->order){
        before->next = new_node;
        new_node->next = node;
        break;
      }

      if(order == node->order){
        printf("同じ順序のデータが既に存在します\n");
        exit(-1);
      }

      before = node;
      node = node->next;
    }
  }
}


void free_ordered_list(ordered_list **ordered_list){
  ordered_node *next, *node;
  (*ordered_list)->length = 0;
  node = (*ordered_list)->node_root;
  while(1){
    if(node == NULL){
      free(node);
      break;
    }
    
    next = node->next;
    free(node);
    node = next;
  }

  free(*ordered_list);
}


int ref_ordered_list(ordered_list **ordered_list, int n){
  int i;
  ordered_node *node;

  if(n == -1 && (*ordered_list)->node_root != NULL){
    n = (*ordered_list)->length - 1;
  }
  else if(n < 0 || (*ordered_list)->length - 1 < n){
    //printf("順位の指定が不適切です\n");
    return -1;
  }

  node = (*ordered_list)->node_root;
  for(i=0; i<n; i++){
    node = node->next;
  }

  return node->data;
}


int ref_ordered_list_order(ordered_list **ordered_list, int n){
  int i;
  ordered_node *node;

  if(n == -1 && (*ordered_list)->node_root != NULL){
    n = (*ordered_list)->length - 1;
  }
  else if(n < 0 || (*ordered_list)->length - 1 < n){
    //printf("順位の指定が不適切です\n");
    return -1;
  }

  node = (*ordered_list)->node_root;
  for(i=0; i<n; i++){
    node = node->next;
  }

  return node->order;
}


// エラー出力用関数
void printe(char message[99]){
  fprintf(stderr, "\n%s %s\n%s", RED BOLD "ERROR:" STANDARD_C BOLD, message, STANDARD"");
}


// 配列のアドレスを確保
int mat_alloc(matrix *mat, int row, int col){
  int i, j;

  mat->row = row;
  mat->col = col;
  mat->element = calloc(row, sizeof(int*));
  mat->base_element = calloc(col * row, sizeof(int));

  if (mat->element == NULL || mat->base_element == NULL){
    printe("配列用の領域が確保できませんでした。 >>mat_alloc()");
    return -1;
  }

  for(i=0; i<row; i++){
    mat->element[i] = &mat->base_element[i * col];
  }

  return 0;
}


int dmat_alloc(dmatrix *mat, int row, int col){
  int i, j;

  mat->row = row;
  mat->col = col;
  mat->element = calloc(row, sizeof(double*));
  mat->base_element = calloc(col * row, sizeof(double));

  if (mat->element == NULL || mat->base_element == NULL){
    printe("配列用の領域が確保できませんでした。 >>mat_alloc()");
    return -1;
  }
  
  for(i=0; i<row; i++){
    mat->element[i] = &mat->base_element[i * col];
  }

  return 0;
}


// 配列用のメモリを解放
void mat_free(matrix *mat){
  free(mat->base_element);
  free(mat->element);
}


void dmat_free(dmatrix *mat){
  free(mat->base_element);
  free(mat->element);
}


// 配列を行列形式でprintf
void mat_print(matrix mat){
  int i, j;

  printf("\n");
  for(i=0; i<mat.row; i++){
    for(j=0; j<mat.col; j++){
      printf("%4d ", mat.element[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}


// 配列を行列形式でprintf
void dmat_print(dmatrix mat){
  int i, j;

  printf("\n");
  for(i=0; i<mat.row; i++){
    for(j=0; j<mat.col; j++){
      printf("%4f ", mat.element[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}


// 行列のサイズが同じかチェック
int check(matrix mat1, matrix mat2){
  if(mat1.col != mat2.col || mat1.row != mat2.row){
    return -1;
  }
  return 0;
}


// mat1 = mat2 * mat3
int check_mul(matrix mat1, matrix mat2, matrix mat3){
  if(mat2.col != mat3.row || mat2.row != mat1.row || mat3.col != mat1.col){
    return -1;
  }
  return 0;
}


// 行列1の行列数と行列2の列行数が一致するかチェック（同順, 転置用）
int check_trans(matrix mat1, matrix mat2){
  if(mat1.col != mat2.row || mat1.row != mat2.col){
    return -1;
  }
  return 0;
}


// 正方行列かをチェック
int check_square(matrix mat){
  if(mat.col != mat.row){
    return -1;
  }
  return 0;
}


// mat2をmat1へコピー
int mat_copy(matrix *mat1, matrix mat2){
  int i,j;

  if(check(*mat1, mat2) != 0){
    printe("引数の行列のサイズが一致しません。 >>mat_copy()");
    return -1;
  }

  for(i=0; i < mat2.row; i++){
    for(j=0; j < mat2.col; j++){
      mat1->element[i][j] = mat2.element[i][j];
    }
  }

  return 0;
}


// mat2（1行n列）をmat1（m行n列）のk行目へコピー
int mat_row_copy(matrix *mat1, matrix mat2, int k){
  int i;

  if(mat2.row != 1 || mat1->col != mat2.col){
    printe("引数の行列のサイズが不適切です。 >>mat_row_copy()");
    return -1;
  }

  for(i=0; i < mat2.col; i++){
    mat1->element[k][i] = mat2.element[0][i];
  }

  return 0;
}


int mat_trans(matrix *mat1, matrix mat2){
  int i, j;
  matrix mat2_w;

  if(check_trans(*mat1, mat2) != 0){
    printe("引数の行列のサイズが不適切です。 >>mat_trans()");
    return -1;
  }

  mat_alloc(&mat2_w, mat2.row, mat2.col);
  mat_copy(&mat2_w, mat2);
  
  for(i=0; i < mat1->row; i++){
    for(j=0; j < mat1->col; j++){
      mat1->element[i][j] = mat2_w.element[j][i];
    }
  }
  mat_free(&mat2_w);

  return 0;
}


// 0〜1までの一様分布 U(0, 1)からのランダム変数を返す
double standard_uniform_rv(void){
  return 1.0 * rand() / RAND_MAX;
}


// 標準正規分布 N(0, 1)からのランダム変数を返す
// Box-Muller変換
double standard_normal_rv(void){
  double z, uni1, uni2;

  uni1 = standard_uniform_rv();
  uni2 = standard_uniform_rv();
  z = sqrt(-2.0 * log(uni1)) * sin(2.0 * M_PI * uni2);

  return z;
}


// 正規分布 N(μ, σ^2)からのランダム変数を返す
double normal_rv(double mu, double sigma){
  double z, n;

  z = standard_normal_rv();
  n = mu + z * sigma;

  return n;
}




