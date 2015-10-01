    #include <stdio.h>
    
    // 通常の変数を参照渡し
    void addone(int *n){
      (*n)++;
    }
    
    int main(void){
      int i=1;
    
      addone(&i);
    
      printf("%d\n", i);
      return 0;
    }
    
    // 実行結果
    // 2
    
    
    // ポインタ変数を参照渡し
    void addone(int **n){
      (**n)++;
    }
    
    int main(void){
      int *i;
    
      *i = 1;
      addone(&i);
    
      printf("%d\n", *i);
      return 0;
    }
    
    // 実行結果
    // 2

