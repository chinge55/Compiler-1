#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "pr.h"

// Symbol table Part

static unsigned symhash(char *sym)
{
    unsigned int hash = 0;
    unsigned c;
    while(c = *sym++) hash = hash*9^c;
    return hash;
}
struct symbol *
lookup(char* sym)
{
  struct symbol *sp = &symtab[symhash(sym)%NHASH];
  int scount = NHASH;		/* how many have we looked at */

  while(--scount >= 0) {
    if(sp->name && !strcmp(sp->name, sym)) { return sp; }

    if(!sp->name) {		/* new entry */
      sp->name = strdup(sym);
      sp->value = 0;
      sp->func = NULL;
      sp->syms = NULL;
      return sp;
    }

    if(++sp >= symtab+NHASH) sp = symtab; /* try the next entry */
  }
  yyerror("symbol table overflow\n");
  abort(); /* tried them all, table is full */

}



struct symlist *
newsymlist(struct symbol *sym, struct symlist *next)
{
  struct symlist *sl = malloc(sizeof(struct symlist));
  
  if(!sl) {
    yyerror("out of space");
    exit(0);
  }
  sl->sym = sym;
  sl->next = next;
  return sl;
}



// Symbol table Part -End
// Called Functions Part

struct ast *
newast(int nodetype, struct ast *l, struct ast *r)
{
  struct ast *a = malloc(sizeof(struct ast));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = nodetype;
  a->l = l;
  a->r = r;
  return a;
}

struct ast *
newnum(double d)
{
  struct numval *a = malloc(sizeof(struct numval));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = 'K';
  a->number = d;
  return (struct ast *)a;
}

struct ast *
newcmp(int cmptype, struct ast *l, struct ast *r)
{
  struct ast *a = malloc(sizeof(struct ast));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = '0' + cmptype;
  a->l = l;
  a->r = r;
  return a;
}

struct ast *
newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *el)
{
  struct flow *a = malloc(sizeof(struct flow));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = nodetype;
  a->cond = cond;
  a->tl = tl;
  a->el = el;
  return (struct ast *)a;
}
        // These functions are not yet declared in pr.h
struct ast* ref(struct symbol *s)
{
  // Call the lookup Function
  if(lookup(s))
  {
    // (Work to be done)extract data from the symbol table
    struct symref *a = malloc(sizeof(struct symref));
    if(!a)
    {
      yyerror("Out of space");
      exit(0);
    }
    a->nodetype = 'N';
    a->s = s;
    return (struct ast*)a;
  }
  else
  {
      // call the error function
      yyerror("variable not assigned");
      exit(0);
  }
  
}
struct ast *
newasgn(struct symbol *s, struct ast *v)
{
  struct symasgn *a = malloc(sizeof(struct symasgn));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = '=';
  a->s = s;
  a->v = v;
  return (struct ast *)a;
  /*This logic is not yet finalized
  if(lookup(s,v))
    the declaration exists so update the symbol table
  else
    assign the data and also put it in symbol table by lookup
    struct symasgn *a = malloc(sizeof(struct symasgn));
  
    if(!a) {
      yyerror("out of space");
      exit(0);
    }
    a->nodetype = '=';
    a->s = s;
    a->v = v;
    call the lookup function and if both the code don't show errors, return the value 
    return (struct ast *)a;
  */
}
struct ast *newprint(struct ast *exp)
{

}
// Called functions part -End
//Evaluation Part




// Evaluation part -End
//Destruction Part
void treefree(struct ast *a)
{

}



//Destruction part -End
//Error checking part
void yyerror(char *s, ...)
{
  va_list ap;
  va_start(ap, s);
  fprintf(stderr, "%d Error", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr);
}

//Error checking part -End
//Main Function
int main()
{
  return yyparse();
}