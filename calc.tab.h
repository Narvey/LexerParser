#define OUTPUTFILE "calc.tkn"

#define END       (0) // tokens
#define ID      (256) // R0 through R9
#define INT     (257) // value
#define FLT     (258) // value
#define OPAREN  (259) // [(]
#define CPAREN  (260) // [)]
#define ASSIGN  (261) // [=]
#define EXP     (262) // [^]
#define MUL     (263) // [*]
#define DIV     (264) // [/]
#define ADD     (265) // [+]
#define SUB     (266) // [-]
#define SEMI    (267) // [;]
#define BAD     (268) // [All characters with certain replacements]
#define NEWLINE (269) // [source code line number]
#define EOLCMT  (270) // [First twenty characters with certain replacements]
#define BLKCMT  (271) // [First twenty characters with certain replacements]

#include <stdio.h>
#include <stdlib.h>

int yyparse(const char *filename);

extern FILE *yyin;
extern char *yytext;

