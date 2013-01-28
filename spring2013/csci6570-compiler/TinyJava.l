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
%}
Letter                  [a-zA-Z_]
Digit                   [0-9]
Ident                   {Letter}({Letter}|{Digit})*
NonZeroDigit            [1-9]
DecimalIntegerLiteral   (0|{NonZeroDigit}{Digit}*)
Exponent                [eE]([+-]{Digit}+|{Digit}+)
FloatingPointLiteral    {Digit}+\.{Digit}+[fF]|{Digit}+\.{Digit}+{Exponent}[Ff]
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
Equal                   =
Space                   [[:space:]]
Newline                 [\n\r]
 /*
DoubleQuote             \"
 */
Less                    \<
Greater                 \>
 /*
BlockComment            \/\*([^\*]|\n|*(?!/))+\*\/
    */
BlockCommentStart       \/\*
BlockCommentEnd         \*\/
Minus                   -
Plus                    \+
Multiply                \*
Divide                  \/
Return                  return
%x BlockComment
%%
{BlockCommentStart}                 BEGIN(BlockComment); return BCOMMENTSTART;
<BlockComment>[^*]*                 /*ignored*/;
<BlockComment>\*[^\/]               /*ignored*/;
<BlockComment>{BlockCommentEnd}     BEGIN(INITIAL); return BCOMMENTEND;
{Newline}                           yylineno++;
{Return}                            return RETURN;
{Ident}                             return IDENT;
{FloatingPointLiteral}              return FLOAT;
{Digit}+                            return CONST;
{InlineComment}                     return ICOMMENT;
{LeftParenthese}                    return LPAR;
{RightParenthese}                   return RPAR;
{Semicolon}                         return SEMI;
{Dot}                               return DOT;
{LeftBrace}                         return LBRACE;
{RightBrace}                        return RBRACE;
{Equal}                             return EQUAL;
{Less}                              return LESS;
{Greater}                           return GREATER;
{LeftBracket}                       return LBRACKET;
{RightBracket}                      return RBRACKET;
{StringLiteral}                     return STRING;
 /*
{DoubleQuote}                       return DQUOTE;
*/
{Space}+                            /* ignored character */
{Minus}                             return MINUS;
{Plus}                              return PLUS;
{Multiply}                          return MUL;
{Divide}                            return DIVIDE;
.                                   return ERROR;
 /*
{BlockComment}                      return BCOMMENT;
*/
