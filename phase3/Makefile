# Makefile: A simple makefile for phase3.
default:
	bison -d -t phase3.ypp
	flex phase3.l
	g++ -o phase3 phase3.tab.cpp lex.yy.c
	./phase3 $(file)
