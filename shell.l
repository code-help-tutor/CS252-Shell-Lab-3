
/*
 *
 * CS-252 
 * shell.l: lexical analyzer for shell
 * You have to extend it.
 *
 */

%{

#include <cstring>
#include "y.tab.hh"

static  void yyunput (int c,char *buf_ptr  );

void myunputc(int c) {
  unput(c);
}

%}

%option noyywrap
 /* start of def section */
%%

\n {
  return NEWLINE;
}

[ \t] {
  /* Discard spaces and tabs */
}

">" {
  return GREAT;
}

[^ \t\n][^ \t\n]*  {
  /* Assume that file names have only alpha chars one or more char is a WORD */
  yylval.cpp_string = new std::string(yytext);
  return WORD;
}

"|" {
  return PIPE;

}

"<" {
  return LESS;
}

">>" {
  return GREATGREAT;
}
">&" {
return GREATAMP;

}

">>&" {
  return GREATGREATAMP;
}

"&" {
return AMPERSAND;
}
"2>" {
return TWOGREAT;
}

%%

// subroutines section
