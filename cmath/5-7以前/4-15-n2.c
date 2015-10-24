#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#define SUBJECT_TEXT_LENGTH 30
#define MAX_STUDENTS 9
#define MAX_SUBJECTS 9
#define CHAR_SIZE 6
#define MAX_STUDENTNAME_LENGTH 20


double average(int sum, int n);


typedef struct{
  char name[MAX_STUDENTNAME_LENGTH * CHAR_SIZE];
  int *kamoku_score;
} StudentData;


int main(){
  StudentData *student;
  int sum, all_sum, len_students, len_subjects, i, j, k, i1, i2, len_text;
  char len_students_input[CHAR_SIZE], len_subjects_input[CHAR_SIZE], *error, input_temporal[CHAR_SIZE * 300];
  char **subjects, *subjects_container;


  // 生徒数を入力してlen_studentsに代入
  printf("\n総人数を入力してください（1〜%d人）：", MAX_STUDENTS);
  fgets(len_students_input, sizeof(len_students_input), stdin);
  len_students_input[strlen(len_students_input) - 1] = '\0'; // 最後の改行コードを削除
  len_students = strtol(len_students_input, &error, 10); // 文字列を数字に変換（整数でない場合は*errorにエラー対象文字列へのポインタを格納）
  if(*error || len_students < 1 || len_students > MAX_STUDENTS){ // 入力値が数でない or 適当な範囲に収まっていない場合エラーを返す
    printf("エラーが発生しました\n\n");
    exit(-1);
  }
  fflush(stdin);


  // 科目数を入力してlen_subjectsに代入
  printf("科目数を入力してください（1〜%d人）：", MAX_SUBJECTS);
  fgets(len_subjects_input, sizeof(len_subjects_input), stdin);
  len_subjects_input[strlen(len_subjects_input) - 1] = '\0'; // 最後の改行コードを削除
  len_subjects = strtol(len_subjects_input, &error, 10); // 文字列を数字に変換（整数でない場合は*errorにエラー対象文字列へのポインタを格納）
  if(*error || len_students < 1 || len_students > MAX_STUDENTS){ // 入力値が数でない or 適当な範囲に収まっていない場合エラーを返す
    printf("エラーが発生しました\n\n");
    exit(-1);
  }
  fflush(stdin);


  // 科目名を入力して配列subjectsに代入
  subjects = calloc(len_subjects, sizeof(char*));
  subjects_container = calloc(CHAR_SIZE * (SUBJECT_TEXT_LENGTH + 1) * len_subjects, sizeof(char));

  for(i=0; i<len_subjects; i++){
    subjects[i] = &subjects_container[i * CHAR_SIZE * (SUBJECT_TEXT_LENGTH + 1)];

    printf("%d番目の科目名を入力してください：", i+1);

    fgets(input_temporal, CHAR_SIZE * (SUBJECT_TEXT_LENGTH + 1), stdin);
    input_temporal[strlen(input_temporal) - 1] = '\0'; // 最後の改行コードを削除
    if(strlen(input_temporal) > CHAR_SIZE * SUBJECT_TEXT_LENGTH + 1){
      printf("入力文字数が長過ぎます\n\n");
      exit(-1);
    }

    strcpy(subjects[i], input_temporal);
    fflush(stdin);
  }


  // 生徒名とそれぞれの生徒の各科目の点数を入力
  student = calloc(len_students * len_subjects, sizeof(StudentData));
  for(j=0; j<len_students; j++){
    printf("\n%d番目の生徒の名前を入力してください：", j+1);
    scanf("%s", student[j].name);
    student[j].kamoku_score = calloc(len_subjects, sizeof(int));
    for(k=0; k<len_students; k++){
      printf("-%sさんの%sの点数を入力してください：", student[j].name, subjects[k]);
      scanf("%d", &student[j].kamoku_score[k]);
    }
  }


  // 出力
  printf("\n");
  all_sum = 0;
  for(i1=0; i1<len_students; i1++){
    sum = 0;
    printf("%sさん：", student[i1].name);
    for(i2=0; i2<len_subjects; i2++){
      sum += student[i1].kamoku_score[i2];
    }
    all_sum += sum;
    printf("合計点%d点、平均点%f点\n", sum, average(sum, len_subjects));
  }
  printf("\n全体の合計点は%d点、平均点%f点でした。\n\n", all_sum, average(all_sum, len_subjects * len_students));


  // メモリ解放
  free(student);
  free(subjects_container);
  free(subjects);

  return 0;
}


// 平均を求める
double average(int sum, int n){
  return (double) sum / (double) n;
}






