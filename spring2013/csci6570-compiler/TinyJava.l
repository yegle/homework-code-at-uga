/*
scanner for TinyJava language
*/
%{
#include "y.tab.h"
int yywrap(void){
    return 1;
}

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

char* reason;
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
Space                   " "
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
{Int}                               return INT;
{Float}                             return FLOAT;
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
{Return}                            return RETURN;
{Identifier}                        return IDENT;
{FloatingPointLiteral}              BEGIN(TokenNotAllowed); return FLOATLITERAL;
{DecimalIntegerLiteral}             BEGIN(TokenNotAllowed); return INTLITERAL;
<TokenNotAllowed>[_a-zA-Z0-9]       {
    reason = "This token is not allowed right after the previous token";
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
{StringLiteral}                     return STRING;
{Space}+                            /* ignored character */
{Minus}                             return MINUS;
{Plus}                              return PLUS;
{Increment}                         return INCREMENT;
{Decrement}                         return DECREMENT;
{Multiply}                          return MUL;
{Divide}                            return DIVIDE;
{Comma}                             return COMMA;
.                                   {
    reason="Unrecognized token";
    return ERROR;
}
