# コンパイルオプションとリンクオプション
# 自分のディレクトリ構造にあわせて修正してください
# コンパイルエラーになる場合は，CLFLAGSの2行目を使ってください
CLFLAGS = -I../include -I/usr/include -Wno-deprecated -Wall
LDFLAGS = -L../lib -framework GLUT -framework OpenGL -Wno-deprecated

# 利用するオブジェクトファイルをまとめて定義
OBJS = galeshapley.o

# galeshapleyを作成するためのルール
galeshapley: $(OBJS)
	gcc -o galeshapley $(OBJS) $(LDFLAGS)

# .c から .o ファイルを作成するための一般的ルール
.c.o:
	gcc -c $(CLFLAGS) $<

clean:
	rm -rf *.o
	rm -rf *.bak
	rm -rf *.core
