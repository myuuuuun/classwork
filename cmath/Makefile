# practice という出力ファイルを得るための方法と，必要となる（practice が依存する）ファイル群を定義する．
# ２行目が具体的なコンパイルコマンドを表す．２行目の行頭は，タブ文字である（スペースだとエラーとなる）．
practice: 5-7-n4_main.o 5-7-n4_matrix.o 5-7-n4.h
	gcc -o practice 5-7-n4_main.o 5-7-n4_matrix.o

5-7-n4_main.o: 5-7-n4_main.c 5-7-n4.h
	gcc -c 5-7-n4_main.c

5-7-n4_matrix.o: 5-7-n4_matrix.c 5-7-n4.h
	gcc -c 5-7-n4_matrix.c

5-7-n4.h:
# matrix.h に関しては，修正してもコンパイルしなおす必要が無いので，依存関係もコマンドも記述する必要がない．
# 無意味なルールではあるが，エラー発生を防ぐために，このようなダミーのルールを書くことがある