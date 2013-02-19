// This file would normally be generated by Flex

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "calc.tab.h"

/*precondition: yytext has been allocated, but doesn't have enough space
postcondition: the contents of yytext have been moved to a new, bigger space
and the yytext pointer now points there. */
void expand(void)
{
    char* temp = (char *) malloc(sizeof(yytext)*2);//twice the original size.
    if(!temp)exit(1);
    int i;//index into both arrays
    for (i = 0; yytext[i]; i++) //for all characters in the original string
    {
        temp[i] = yytext[i];//copy a character over to the temp string.
    }
    temp[i] = 0;//null terminate the temp string.
    free(yytext);//don't leak memory
    yytext = temp;//and finally point at the new, bigger string!
    temp = NULL;
}// INSTEAD OF THIS, do yysize*=2; yytext = realloc(yytext, yysize);

//precondition: cur must be a digit '0'-'9', yytext allocated.
//postcondition: next char on yyin is not a digit.
void yydigit(char cur)
{
    printf("yydigit");
    while('0'<=cur&&'9'>=cur)
    {
        printf("here\n");
        sprintf(yytext+strlen(yytext), "%c", cur);//save to yytext
        if(strlen(yytext) >= (sizeof(yytext)/sizeof(char) - 1))expand();//again
        cur = fgetc(yyin);
    }//if you get out here, cur is not a digit, so...
    ungetc(cur, yyin);//put it back on the buffer.
}

/*precondition: This state should only be reached by reading a . after a digit,
 a digit after a period, or an E. So thie function assumes that cur is a
 period, digit, or E. Also assumes that yytext has been allocated.
 postcondition: We are past the valid float characters, so the next char is
 either a 5(which signalls a BAD token) or the beginning of the next lexeme.*/
void yyfloat(char cur)
{
    printf("yyfloat\n");
    if('E'==cur)//exponential. Sub-float state
    {
        sprintf(yytext+strlen(yytext), "%c", cur);//save to yytext
        cur = fgetc(yyin);
        if('+'==cur||'-'==cur)//look for sign
        {
            sprintf(yytext+strlen(yytext), "%c", cur);//save to yytext
            if(strlen(yytext)  >= (sizeof(yytext)/sizeof(char) - 1))expand();
            //expand if needed. ^shouldn't be greater, but just in case.
            cur = fgetc(yyin);//done with sign, moving on to (hopefully)digits
        }
        if('0'<=cur&&'9'>=cur)yydigit(cur);//should be numbers next
        else
        {
            ungetc(cur,yyin);//put it back
            ungetc('5',yyin);//signal that this is a BAD token.
        }
    }
    else //digit or period.  Main float state.
    {
        sprintf(yytext+strlen(yytext), "%c", cur);//save to yytext
        printf(yytext);
        if(strlen(yytext) >= (sizeof(yytext)/sizeof(char) - 1))expand();//again
        printf(yytext);
        cur = fgetc(yyin);//read next
        if('0'<=cur&&'9'>=cur)
        {
            yydigit(cur);//slurp up all digits
            cur = fgetc(yyin);//prepare for 'E' check.
        }
        if('E'==cur)yyfloat(cur);
        else ungetc(cur, yyin);//end of (valid) FLT token
    }
}

int yylex(void)
{
    static int tokens = 0;
    static int linenum = 1;

    int token;
    int yychar;
    yychar = fgetc(yyin); // File assumed to be open and ready
    switch(yychar)
    {
    case '\n':
        printf("newline\n");
        token = NEWLINE;
        yytext = (char *) malloc(11*sizeof(char));
        if(!yytext)exit(1);
        sprintf(yytext, "%i", ++linenum);
        break;
    case EOF:
        printf("EOF\n");
        //This is from the placeholder code
        token = END;
        yytext = (char *) malloc(11*sizeof(char));
        if(!yytext)exit(1);
        sprintf(yytext, "%i", tokens);
        //end placeholder code
        break;
    case 'R':
        printf("R\n");
        token = ID;
        yytext = (char *) malloc(3*sizeof(char));//3 chars: R,number,null
        if(!yytext)exit(1);
        yychar = fgetc(yyin);//look for the number
        if('0'<=yychar && '9'>=yychar)
        {
            sprintf(yytext, "R%c", yychar);//we know the first one is 'R'.
        }
        else //if we didn't get a digit as expected
        {
            token = BAD;
            sprintf(yytext, "R");//for this case, we know it was just 'R'.
            ungetc(yychar,yyin);//put back that thing that wasn't a digit.
        }
        break;
    case ' ':
        token = SPACE;
        printf("SPACE\n");
        break;
    case '(':
        token = OPAREN;
        yytext = (char *) malloc(2*sizeof(char));//1 for symbol, 1 for null
        if(!yytext)exit(1);
        sprintf(yytext, "%c", yychar);
        break;
    case ')':
        token = CPAREN;
        yytext = (char *) malloc(2*sizeof(char));//1 for symbol, 1 for null
        if(!yytext)exit(1);
        sprintf(yytext, "%c", yychar);
        break;
    case '=':
        token = ASSIGN;
        yytext = (char *) malloc(2*sizeof(char));//1 for symbol, 1 for null
        if(!yytext)exit(1);
        sprintf(yytext, "%c", yychar);
        break;
    case '^':
        token = EXP;
        yytext = (char *) malloc(2*sizeof(char));//1 for symbol, 1 for null
        if(!yytext)exit(1);
        sprintf(yytext, "%c", yychar);
        break;
    case '*':
        printf("*\n");
        token = MUL;
        yytext = (char *) malloc(2*sizeof(char));//1 for symbol, 1 for null
        if(!yytext)exit(1);
        sprintf(yytext, "%c", yychar);
        break;
    case '+':
        token = ADD;
        yytext = (char *) malloc(2*sizeof(char));//1 for symbol, 1 for null
        if(!yytext)exit(1);
        sprintf(yytext, "%c", yychar);
        break;
    case '-':
        token = SUB;
        yytext = (char *) malloc(2*sizeof(char));//1 for symbol, 1 for null
        if(!yytext)exit(1);
        sprintf(yytext, "%c", yychar);
        break;
    case ';':
        token = SEMI;
        yytext = (char *) malloc(2*sizeof(char));//1 for symbol, 1 for null
        if(!yytext)exit(1);
        sprintf(yytext, "%c", yychar);
        break;
    case '/':
        yytext = (char *) malloc(2*sizeof(char));
        if(!yytext)exit(1);
        token = DIV;
        sprintf(yytext, "/");
        break;
        break;
    case '.':
        token = FLT;
        if(!yytext)
        {
            yytext = (char *) malloc(5*sizeof(char));//allocate
            if(!yytext)exit(1);
            yytext[0]=0;//null terminate so strlen doesn't segfault.
        }
        sprintf(yytext+strlen(yytext), "%c", yychar);//save the dot
        yychar = fgetc(yyin);
        if('0'<=yychar&&'9'>=yychar)//make sure there is a digit after the '.'
        {
            yyfloat(yychar);
            yychar = fgetc(yyin);//check the next char
            if('5'!=yychar)//if the char is not the "error code"
            {
                ungetc(yychar,yyin);//save it for the next lexeme.
                break;//we have a valid FLT.
            }
            else yychar = ' '; //put a harmless space instead of 5.
        }
        //We get here if there was an error code or lonely '.'
        //Fall through to BAD.
    default://INT, FLT, or BAD
        if(!yytext)
        {
            yytext = (char *) malloc(5*sizeof(char));//allocate
            if(!yytext)exit(1);
            yytext[0]=0;//null terminate so strlen doesn't segfault.
        }
        if('0'<=yychar && '9'>=yychar)
        {
            token = INT;//it isn't a float until we see . or E
            yydigit(yychar);//slurp up all the digits
            yychar = fgetc(yyin);//read what was pushed back onto the buffer.
            if('.'==yychar||'E'==yychar)
            {
                token = FLT;
                yyfloat(yychar);
                yychar = fgetc(yyin);//check the next char
                if('5'!=yychar)//if the char is not the "error code"
                {
                    ungetc(yychar,yyin);//save it for the next lexeme.
                    break;//we have a valid FLT.
                }
                else yychar = ' '; //send a harmless space instead of 5.
                //then we fall through to BAD.
            }
            else
            {
                ungetc(yychar,yyin);
                break;//valid integer, let the next token take over
            }
        }
        token = BAD;
        if(strlen(yytext) >= (sizeof(yytext)/sizeof(char) - 4))expand();
        //make sure there is enough room for the next char to be non-printable

        //begin code from placeholder (modified with +strlen(yytext) )
        if (isgraph(yychar)&&('#' != yychar))
            sprintf(yytext+strlen(yytext), "%c", yychar);
        else
            sprintf(yytext+strlen(yytext), "#%02X", (yychar&0xFF));
        //end placeholder code
        break;
    }
    if(BAD>token)tokens++;//using the fact that all debug tokens are >BAD.

    return token;
}

int main(int argc, char *argv[])
{

    if (argc <= 1)
    {
        printf("USAGE: calc inputfilename\n");
        fgetc(stdin);
        exit(EXIT_FAILURE);
    }

    yyparse(argv[1]);

    printf("Hit ENTER to exit.");
    fgetc(stdin);

    return 0;
}
