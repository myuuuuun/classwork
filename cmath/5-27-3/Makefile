# コンパイルオプションとリンクオプション
# 自分のディレクトリ構造にあわせて修正してください
# コンパイルエラーになる場合は，CLFLAGSの2行目を使ってください
CLFLAGS = -I../include -I/usr/include -Wno-deprecated
LDFLAGS = -L../lib -lmatrix -framework GLUT -framework OpenGL -Wno-deprecated

# 利用するオブジェクトファイルをまとめて定義
OBJS = 5-27-3.o

# 5-27-3を作成するためのルール
5-27-3: $(OBJS)
	gcc -o 5-27-3 $(OBJS) $(LDFLAGS)

# .c から .o ファイルを作成するための一般的ルール
.c.o:
	gcc -c $(CLFLAGS) $<

clean:
	rm -rf *.o
	rm -rf *.bak
	rm -rf *.core
