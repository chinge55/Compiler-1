extern int yylineno;
void yyerror(char *s, ...);

// Things for Symbol table
struct symbol
{
    char *name;
    double value;
};
#define NHASH 9997;
struct symbol symtab[NHASH];
bool *lookup(char*);


// Things for symbol table -end
struct ast
{
    int nodetype;
    struct ast *l;
    struct ast *r;
};
struct numval
{
    int nodetype;
    double number;
};
struct symref
{
    int nodetype;
    struct symbol *s;
}
/*
Node types:
I= If condition
B= While, Bhayesamman
N= While, Nabhayesamman
*/
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newnum(double d);
struct ast *newflow(int nodetype, struct ast *cond, struct ast *l, struct ast *r);

double eval(strut ast *);

void treefree(struct ast *);