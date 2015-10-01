#include <stdio.h>
#include <stdlib.h>


typedef struct{
  int data;
  int order;
  struct ordered_node *next;
} ordered_node;


typedef struct{
  int length;
  struct ordered_node *node_root;
} ordered_list;


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
    printf("順位の指定が不適切です\n");
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
    printf("順位の指定が不適切です\n");
    return -1;
  }

  node = (*ordered_list)->node_root;
  for(i=0; i<n; i++){
    node = node->next;
  }

  return node->order;
}


void print_ordered_list(ordered_list *ordered_list){
  int i;
  ordered_node *node;

  if(ordered_list->node_root != NULL){
    for(node = ordered_list->node_root; node != NULL; node = node->next){
      printf("%d -> ", node->data);
    }
  }
  printf("NULL\n");
}


int main(){
  int i;
  ordered_list *ordered_list;
  ordered_node *node;

  ordered_list = calloc(1, sizeof(ordered_list));
  ordered_list->node_root = calloc(1, sizeof(ordered_node));
  ordered_list->length = 0;
  ordered_list->node_root = NULL;


  print_ordered_list(ordered_list);


  printf("----------------\n");
  printf("%d\n", ref_ordered_list(&ordered_list, 0));
  printf("%d\n", ref_ordered_list(&ordered_list, 3));
  printf("%d\n", ref_ordered_list(&ordered_list, -1));


  free_ordered_list(&ordered_list);
  
  return 0;
}

/*実行結果

2 
3 
-1 
1 

*/









