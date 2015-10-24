# コンパイルオプションとリンクオプション
# 自分のディレクトリ構造にあわせて修正してください
# コンパイルエラーになる場合は，CLFLAGSの2行目を使ってください
CLFLAGS = -I../include -I/usr/include
# CLFLAGS = -I../include
LDFLAGS = -L../lib -lmatrix -framework GLUT -framework OpenGL -Wno-deprecated

# 利用するオブジェクトファイルをまとめて定義
OBJS = 5-13-main.o least_square.o

# 5-13-mainを作成するためのルール
5-13-main: $(OBJS)
	gcc -o 5-13-main $(OBJS) $(LDFLAGS)

# .c から .o ファイルを作成するための一般的ルール
.c.o:
	gcc -c $(CLFLAGS) $<

clean:
	rm -rf *.o
	rm -rf *.bak
	rm -rf *.core