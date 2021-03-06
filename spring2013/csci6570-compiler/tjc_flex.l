/*
scanner for TinyJava language
%option yylineno 
*/


%{

#include "Ast.h"
#include "y.tab.h"

//int yywrap(void){
//    return 1;
//}

int yylex(void);

char reason[1024];

int set_file(char* filename){
    yyin = fopen(filename, "r");
    if (yyin==NULL){
        fprintf(stderr, "Error opening file. Reason: %s\n",
                strerror(errno));
        return -1;
    }
    else{
        return 0;
    }
}

%}
Letter                  [a-zA-Z_]
Digit                   [0-9]
Identifier              {Letter}({Letter}|{Digit})*
NonZeroDigit            [1-9]
NonDigit                [^0-9]
DecimalIntegerLiteral   (0|{NonZeroDigit}{Digit}*)
Exponent                [eE]([+-]{Digit}+|{Digit}+)
FloatingPointLiteral    {DecimalIntegerLiteral}\.{Digit}+[fF]|{Digit}+\.{Digit}+{Exponent}[Ff]
EscapedChar             \\[n\\"]
StringLiteral           \"([^\"\\\n]|{EscapedChar})+\"
InlineComment           \/\/.*$
LeftParenthese          \(
RightParenthese         \)
LeftBracket             \[
RightBracket            \]
LeftBrace               \{
RightBrace              \}
Semicolon               ;
Dot                     \.
Assign                  =
Equal                   ==
NotEqual                !=
Space                   [ \t]
Newline                 [\n\r]
LessEqual               \<=
GreaterEqual            \>=
Less                    \<
Greater                 \>
BlockCommentStart       \/\*
BlockCommentEnd         \*\/
Minus                   -
Plus                    \+
Increment               \+\+
Decrement               --
Multiply                \*
Divide                  \/
Comma                   ,
Return                  return
If                      if
Else                    else
For                     for
New                     new
Null                    null
Int                     int
Float                   float
Void                    void
Public                  public
Static                  static
While                   while
Class                   class
%x BlockComment
%x TokenNotAllowed
%%
{Return}                            return RETURN;
{If}                                return IF;
{Else}                              return ELSE;
{For}                               return FOR;
{New}                               return NEW;
{Null}                              return NULL_;
{Int}                               yylval.sval = strdup(yytext); return INT;
{Float}                             yylval.sval = strdup(yytext); return FLOAT;
{Void}                              return VOID;
{Public}                            return PUBLIC;
{Static}                            return STATIC;
{While}                             return WHILE;
{Class}                             return CLASS;
{BlockCommentStart}                 BEGIN(BlockComment);
<BlockComment>[^*\n\r]*             /*ignored*/;
<BlockComment>\*[^\/]               /*ignored*/;
<BlockComment>{BlockCommentEnd}     BEGIN(INITIAL);
<*>{Newline}                        yylineno++;
{Identifier}                        yylval.sval = strdup(yytext); return IDENT;
{FloatingPointLiteral}              yylval.sval = strdup(yytext); BEGIN(TokenNotAllowed); return FLOATLITERAL;
{DecimalIntegerLiteral}             yylval.sval = strdup(yytext); BEGIN(TokenNotAllowed); return INTLITERAL;
<TokenNotAllowed>[_a-zA-Z0-9]       {
    snprintf(reason, 1024, "This token is not allowed right after the previous token");
    return ERROR;
}
<TokenNotAllowed>[^_a-zA-Z0-9]      BEGIN(INITIAL); unput(yytext[0]);
{InlineComment}                     //return ICOMMENT;
{LeftParenthese}                    return LPAR;
{RightParenthese}                   return RPAR;
{Semicolon}                         return SEMI;
{Dot}                               return DOT;
{LeftBrace}                         return LBRACE;
{RightBrace}                        return RBRACE;
{Assign}                            return ASSIGN;
{Equal}                             return EQUAL;
{NotEqual}                          return NOTEQUAL;
{LessEqual}                         return LESSEQUAL;
{GreaterEqual}                      return GREATEREQUAL;
{Less}                              return LESS;
{Greater}                           return GREATER;
{LeftBracket}                       return LBRACKET;
{RightBracket}                      return RBRACKET;
{StringLiteral}                     yylval.sval = strdup(yytext); return STRING;
{Space}+                            /* ignored character */
{Minus}                             return MINUS;
{Plus}                              return PLUS;
{Increment}                         return INCREMENT;
{Decrement}                         return DECREMENT;
{Multiply}                          return MUL;
{Divide}                            return DIVIDE;
{Comma}                             return COMMA;
.                                   {
    snprintf(reason, 1024, "Unrecognized token");
    return ERROR;
}
