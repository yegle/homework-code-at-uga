/*
scanner for TinyJava language
*/
%{
#include "y.tab.h"
int inBlockComment = 0;
%}
Letter                  [a-zA-Z_]
Digit                   [0-9]
Ident                   {Letter}({Letter}|{Digit})*
NonZeroDigit            [1-9]
DecimalIntegerLiteral   (0|{NonZeroDigit}{Digit}*)
Exponent                [eE]([+-]{Digit}+|{Digit}+)
FloatingPointLiteral    {Digit}+\.{Digit}+[fF]|{Digit}+\.{Digit}+{Exponent}[Ff]
EscapedChar             \\[^n\\"]
StringLiteral           \"([^\\"]|{EscapedChar})+\"
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
DoubleQuote             \"
Less                    \<
Greater                 \>
BlockCommentBegin       \/\*
BlockCommentEnd         \*\/
Minus                   -
Plus                    \+
Multiply                \*
Divide                  \/
Return                  return
%%
{Newline}               ;
\n                      ;
{Return}                return RETURN;
{Ident}                 return IDENT;
{FloatingPointLiteral}  return FLOAT;
{Digit}+                return CONST;
{InlineComment}         return ICOMMENT;
{LeftParenthese}        return LPAR;
{RightParenthese}       return RPAR;
{Semicolon}             return SEMI;
{Dot}                   return DOT;
{LeftBrace}             return LBRACE;
{RightBrace}            return RBRACE;
{Equal}                 return EQUAL;
{DoubleQuote}           return DQUOTE;
{Less}                  return LESS;
{Greater}               return GREATER;
{LeftBracket}           return LBRACKET;
{RightBracket}          return RBRACKET;
{StringLiteral}         return STRING;
({Space}|{Newline})+    /* ignored character */
{Minus}                 return MINUS;
{Plus}                  return PLUS;
{Multiply}              return MUL;
{Divide}                return DIVIDE;
{BlockCommentBegin} {
    inBlockComment = 1;
    return BCOMMENT;
}
{BlockCommentEnd} {
    inBlockComment = 0;
    return BCOMMENT;
}
