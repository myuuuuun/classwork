/*
n個の素数を出力する
2015 @myuuuuun
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


typedef struct{
  unsigned long long int prime;
  struct prime_list *next;
} prime_list;

prime_list *prime_root = NULL;
prime_list *prime_end;


void sleep(int speed)
{
  clock_t c1 = clock(), c2;
  do{
    if((c2=clock())==(clock_t)-1)
    break;
  } while(1000 * (c2-c1) / CLOCKS_PER_SEC < speed);
}


void append_prime(unsigned long long int num){
  prime_list *new_prime;

  new_prime = calloc(1, sizeof(prime_list));
  if(new_prime == NULL){
    printf("メモリ確保に失敗しました\n");
    exit(-1);
  }

  new_prime -> prime = num;

  if(prime_root == NULL){
    new_prime -> next = prime_end;
    prime_root = new_prime;
    prime_end = new_prime;
  }
  else{
    new_prime -> next = NULL;
    prime_end -> next = new_prime;
    prime_end = new_prime;
  }
}


void free_prime(void){
  prime_list *current, *next;

  current = prime_root;
  while(1){
    if(current == NULL){
      break;
    }
    next = current->next;
    free(current);
    current = next;
  }
}


int prime_loop(int count, unsigned long long num){
  unsigned long long int answer;

  while(1){
    printf("%d 番目の素数は何でしょうか？\n>> ", count, num);
    scanf("%llu", &answer);
    if(answer == num){
      printf("あたり! %d 番目の素数は %llu です\n", count, num);
      return 1;
    }
    else{
      printf("ぶぶー！はずれです\n");
    }
  }
}


int prime_loop_pass(int count, unsigned long long num){
  unsigned long long int answer;

  printf("%d 番目の素数は何でしょうか？\n>> ", count, num);
  printf("あたり! %d 番目の素数は %llu です\n", count, num);
  return 1;
}


void print_primes(int size){
  int i, count, flag;
  unsigned long long int num, answer;
  prime_list *prime;

  // 最初の素数は2
  count = 1;
  num = 2;
  //prime_loop(count, num);

  // 3以降は奇数だけを調べる
  ++count;
  num = 3;
  while(count <= size){
    // sleep(800);

    // 素数判定
    flag = 1;
    for(prime = prime_root; prime != NULL; prime = prime->next){
      if(prime->prime > sqrt(num)){
        break;
      }
      if(num % prime->prime == 0){
        flag = 0;
      }
    }

    // 素数なら
    if(flag){
      //printf("%d 番目の素数は %llu\n", count, num);
      if(count < 15324){
        prime_loop_pass(count, num);
      }
      else{
        prime_loop(count, num);
      }
      append_prime(num);
      ++count;
    }
    else{
      //printf("%llu は素数ではありません\n", num);
    }

    num += 2;
  }

  printf("%d 番目の素数は %llu\n", count, num);
  free_prime();
}


int main(void){
  
  print_primes(100000);
  return 0;
}










