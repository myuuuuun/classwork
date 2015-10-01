#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// エラトステネスの篩を使ってmaximum以下の素数を全てprint
void print_primes(unsigned long long maximum){
  int count;
  unsigned long long i, j, num, flag, d, *prime_list, listlen;

  if(maximum < 2){
    exit(-1);
  }

  flag = 0;

  // 2と3以降の奇数だけを配列に入れる
  listlen = maximum % 2 == 0 ? maximum / 2 : (maximum+1) / 2;
  prime_list = calloc(listlen, sizeof(unsigned long long));

  // 配列に初期値をセット
  prime_list[0] = 2;
  for(i=1; i<listlen; i++){
    prime_list[i] = i * 2 + 1;
  }

  // 3以降の奇数を順番に取り出し、倍数が配列に含まれていたら削除していく
  for(i=1; i<listlen; i++){
    d = prime_list[i];

    // √maximum よりも大きな数は処理する必要がない
    if(d > sqrt(maximum)){
      break;
    }

    // 既に割りきれていれば処理しない
    if(d != flag){
      // 自分よりも大きな数字で割りきれるものを探す
      for(j=i+1; j<listlen; j++){
        if(prime_list[j] != 0){
          if(prime_list[j] % d == 0){
            prime_list[j] = 0;
          }
        }
      }
    }
  }

  // 出てきた素数を順番にprint
  count = 1;
  for(i=0; i<listlen; i++){
    if(prime_list[i] != 0){
      printf("%d 番目の素数は %llu\n", count, prime_list[i]);
      count++;
    }
  }
}


int main(void){
  print_primes(100000000);
  return 0;
}










