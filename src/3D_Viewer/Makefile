CC=gcc
FLAGS=-Wall -Werror 
FILENAME=test.out
PROJECTNAME=viewer
TESTNAME=test.c
FULL_PACK=*.c
DOCUMENT_DIR=./readme
GCOV=--coverage
LIBS=`pkg-config --cflags --libs check`
OS=$(shell uname)

ifeq ($(OS), Linux)
  OPEN=xdg-open
else
  OPEN=open
endif

all: $(TESTNAME) $(PROJECTNAME).a install gcov_report dvi

install: 
	@mkdir -p ../build
	@cd ../build && qmake ../3D_Viewer/3D_Viewer.pro && make

uninstall:
	@rm -r ../build

dist:
	@mkdir -p ../build
	@tar -cvzf ../build/3DViewer_v1.tgz Makefile *.c *.h *.pro *.cpp *.ui readme/readme.texi gifanim/gifanim.cpp gifanim/gifanim.h

dvi:
	@makeinfo -o $(DOCUMENT_DIR) --html --no-warn --no-validate --force ./readme/readme.texi
	@open $(DOCUMENT_DIR)/index.html


test: clean $(PROJECTNAME).a
	@$(CC) $(FLAGS) -g $(TESTNAME) -L . $(PROJECTNAME).a $(LIBS) -o $(FILENAME) -std=c11
	@./test.out

gcov_report: $(FULL_PACK) $(PROJECTNAME).h
	@gcc $(CFLAGS) $(GCOV) -g $(FULL_PACK) -o $(FILENAME) -std=c11 $(LIBS)
	@./test.out
	@lcov -t "$(PROJECTNAME)" -o $(PROJECTNAME).info -c -d .
	@genhtml -o report $(PROJECTNAME).info
	@$(OPEN) ./report/index.html

$(PROJECTNAME).a: $(PROJECTNAME).o
	@ar -rcs $(PROJECTNAME).a *.o

$(PROJECTNAME).o: $(PROJECTNAME).c $(PROJECTNAME).h
	@$(CC) $(FLAGS) -c -g $(FULL_PACK)

clean:
	@rm -rf *.dSYM *.out *.gcda *.gcno *.info *.gcov *.a *.o *FILE valgrind.txt dvi.log dvi.pdf dvi.aux report readme/index.html

style:
	@cp ../../materials/linters/.clang-format .
	clang-format -n *.c *.h *.cpp
	@rm .clang-format


.PHONY: all install
