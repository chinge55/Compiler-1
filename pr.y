%{
void yyerror(char *s);
int yylex();
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int symbols[52];    // because only one character can be used as variable
int symbol_value(char symbol);
void updatesymbol(char symbol, int val);
%}
// Yacc declarations section
%union
{
    struct ast *a;
    double d;
    struct symbol *s;
    struct symlist *sl;
    int fn;
}
// tokens declarations
%token <d> NUMBER
%token <s> NAME 
%token EOL

%token THEN ELSE NABHAYESAMMAN BHAYESAMMAN 
%nonassoc ELSE
%nonassoc THEN
%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*' '/'
%nonassoc '|' UMINUS

// type le kun cheej k type ko ho bhanne declare garcha
%type <a> exp stmt 
%type <sl> symlist

%start calclist

%%
// description section

calclist:
        |calclist stmt EOL
        {
            printf("=%4.4g\n",eval($2));
            treefree($2);
            printf(">");
        }
/*
    Dangling else is not going to be a problem because we are going to use purnabirams
*/
stmt: exp THEN stmt                                 {$$ = newflow('I', $1, $3, NULL);}
    | exp THEN stmt ELSE stmt
    | exp BHAYESAMMAN stmt
    | exp NABHAYESAMMAN stmt
    | exp
    | PRINT exp
    | EXIT

exp: exp CMP exp                                   { /* Write AST FUNCTIONS*/}
   | exp '+' exp                                    {$$ = newast('+', $1, $3);}
   | exp '-' exp                                    {$$ = newast('-', $1, $3);}
   | exp '*' exp                                    {$$ = newast('*', $1, $3);}
   | exp '/' exp                                    {$$ = newast('/', $1, $3);}
   | '|' exp                                        {$$ = newast('|', $2, NULL);} 
   |'(' exp ')'                                     {$$ = $2;}
   | '-' exp %prec UMINUS                           {$$ = newast('M', $2, NULL);}
   | NUMBER                                         {$$ = newnum($1);}
   | NAME '=' exp                                   {$$ = newasgn($1, $3);}
   | NAME                                           {$$ = ref($1);}
   ;
%%

