/*
scanner for TinyJava language
*/
%{
// place holder
#include "y.tab.h"
%}
Letter                  [a-zA-Z_]
Digit                   [0-9]
Ident                   {Letter}({Letter}|{Digit})*
NonZeroDigit            [1-9]
DecimalIntegerLiteral   (0|{NonZeroDigit}{Digit}*)
Exponent                [eE]([+-]{Digit}+|{Digit}+)
FloatingPointLiteral    {Digit}+\.{Digit}+[fF]|{Digit}+\.{Digit}+{Exponent}[Ff]
EscapedChar             \\[^n\\"]
StringLiteral           "([^\\]|{EscapedChar})+"
%%
{Ident}                 return IDENT;
{FloatingPointLiteral}  return FLOAT;
{Digit}+                return CONST;
{StringLiteral}         return STRING;
