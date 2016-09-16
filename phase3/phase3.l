/********************************************************
 * phase3.l 
 ********************************************************/
%{
#include "phase3.tab.hpp"
#include <iostream>
using namespace std;
%}

%option noyywrap

%%
"int"						{ return INT; }
"float"						{ return FLOAT; }
"if"						{ return IF; }
"else"						{ return ELSE; }
"while"						{ return WHILE; }
[a-zA-Z][a-zA-Z0-9]*		{ yylval.node.val = new string(strdup(yytext)); return ID; }
[0-9]+						{ yylval.node.ival = atoi((char *)yytext); yylval.node.type = new string("INT"); return NUM; }
[0-9]+\.[0-9]+(\L|E[0-9]+)?	{ yylval.node.fval = atof((char *)yytext); yylval.node.type = new string("FLOAT"); return NUM; }
==|!=|>|>=|<|<=				{ yylval.node.val = new string(strdup(yytext)); return RELOP; }
"="							{ return ASSIGN; }
";"							{ return SEMICOLON; }
","							{ return COMMA; }
"("							{ return OP; }
")"							{ return CL; }
"{"							{ return OPC; }
"}"							{ return CLC; }
"+"							{ return PLUS; }
"-"							{ return MINUS; }
"*"							{ return MULT; }
"/"							{ return DIV; }
<<EOF>>  					{ return 0; }
[ \t\n]+ 					{ }
.        					{ cerr << "Unrecognized token!" << endl; exit(1); }
%%