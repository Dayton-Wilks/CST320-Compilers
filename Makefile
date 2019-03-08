##**************************************
## Makefile
##
## Build routine for lang compiler
##
## Author: Phil Howard 
## phil.howard@oit.edu
##
## Date: Nov. 28, 2015
##

COPTS=-Wall -g -c  -O0 -std=c++11
OBJS=main.o \
	 langlex.o \
	 langparse.o \
	 cVisitor.o \
	 emit.o \

all: lang

clean:
	rm -f $(OBJS)
	rm -f langlex.c
	rm -f langparse.c
	rm -f langparse.h
	rm -f lang
	rm -f out.xml
	rm -f out2.xml
	rm -f langout.sl
	rm -f langout.slb
	rm -f out

.cpp.o:
	g++ $(COPTS) $? -o $@

.c.o:
	g++ $(COPTS) $? -o $@

main.o: main.cpp langparse.c langlex.c 
	g++ $(COPTS) main.cpp -o $@

langlex.o: langlex.c
	g++ $(COPTS) -Wno-sign-compare $? -o $@

langlex.c: lang.l langparse.c
	flex -o langlex.c lang.l

langparse.c: lang.y
	bison --defines=langparse.h lang.y -o langparse.c

lang: $(OBJS)
	g++ $(OBJS) -o lang

