CLFLAGS = -I../include -I/usr/include -Wno-deprecated -Wall
LDFLAGS = -L../lib -lmatrix -luglyfont -framework GLUT -framework OpenGL -Wno-deprecated

OBJS = hw1-2.o

hw1-2: $(OBJS)
	g++ -o hw1-2 $(OBJS) $(LDFLAGS)

# .cpp から .o ファイルを作成するための一般的ルール
.cpp.o:
	g++ -c $(CLFLAGS) $<

clean:
	rm -rf *.o
	rm -rf *.bak
	rm -rf *.core