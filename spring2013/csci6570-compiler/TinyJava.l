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
Assign                  =
Equal                   ==
Space                   [[:space:]]
Newline                 [\n\r]
LessEqual               \<=
GreaterEqual            \>=
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
Keyword                 (return|if|else|for|new|null|int|float|void|public|static|while|class)
%x BlockComment
%x DigitNotAllowed
%%
{BlockCommentStart}                 BEGIN(BlockComment); return BCOMMENTSTART;
<BlockComment>[^*]*                 /*ignored*/;
<BlockComment>\*[^\/]               /*ignored*/;
<BlockComment>{BlockCommentEnd}     BEGIN(INITIAL); return BCOMMENTEND;
{Keyword}                           return KEYWORD;
{Newline}                           yylineno++;
{Return}                            return RETURN;
{Identifier}                        return IDENT;
{FloatingPointLiteral}              return FLOAT;
{DecimalIntegerLiteral}             BEGIN(DigitNotAllowed); return INTEGER;
<DigitNotAllowed>{Digit}            {
    reason = "Digit not allowed right after a DecimalIntegerLiteral";
    return ERROR;
}
<DigitNotAllowed>{NonDigit}         BEGIN(INITIAL); unput(yytext[0]);
{InlineComment}                     return ICOMMENT;
{LeftParenthese}                    return LPAR;
{RightParenthese}                   return RPAR;
{Semicolon}                         return SEMI;
{Dot}                               return DOT;
{LeftBrace}                         return LBRACE;
{RightBrace}                        return RBRACE;
{Assign}                            return ASSIGN;
{Equal}                             return EQUAL;
{LessEqual}                         return LESSEQUAL;
{GreaterEqual}                      return GREATEREQUAL;
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
.                                   {
    reason="Unrecognized token";
    return ERROR;
}
 /*
{BlockComment}                      return BCOMMENT;
*/
