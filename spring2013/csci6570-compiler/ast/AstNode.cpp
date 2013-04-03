// Gnu Emacs C++ mode:  -*- C++ -*-


#include "AstNode.h"
#include "AstException.h"



const char *
AstNode::type2string( int tp )
{
  switch( tp ) {
  case AstNode::TVOID:
    return "void";
  case AstNode::TINT:
    return "int";
  case AstNode::TFLOAT:
    return "float";
  case AstNode::TBOOL:
    return "boolean";
  case AstNode::TSTRING:
    return "String";
  case AstNode::TINTA:
    return "int[]";
  case AstNode::TFLOATA:
    return "float[]";
  case AstNode::TSTRINGA:
    return "String[]";
  case AstNode::TREF:
    return "anonref";
  default:
    return "AstNode::type2string: unkown type";
  }
}

const char *
AstNode::operator2string( int op )
{
  switch( op ) {
  case AstNode::ADDOP:
    return "ADD";
  case AstNode::SUBOP:
    return "SUB";
  case AstNode::MULOP:
    return "MUL";
  case AstNode::DIVOP:
    return "DIV";
  case AstNode::EQOP:
    return "EQ";
  case AstNode::NEOP:
    return "NE";
  case AstNode::LTOP:
    return "LT";
  case AstNode::GTOP:
    return "GT";
  case AstNode::LEOP:
    return "LE";
  case AstNode::GEOP:
    return "GE";
  case AstNode::INCOP:
    return "INCR";
  case AstNode::DECOP:
    return "DECR";
  default:
    return "AstNode::operator2string: unkown operator";
  }
}

const char *
AstNode::kind2string( AstNode::AstNodeKind kind )
{
  switch( kind ) {
  case AstNode::DCLASS:
    return "DCLASS";
  case AstNode::DMETHOD:
    return "DMETHOD";
  case AstNode::DFIELD:
    return "DFIELD";
  case AstNode::DPARAMETER:
    return "DPARAMETER";
  case AstNode::DVARIABLE:
    return "DVARIABLE";
  case AstNode::ELITERAL:
    return "ELITERAL";
  case AstNode::ENAME:
    return "ENAME";
  case AstNode::ENEW:
    return "ENEW";
  case AstNode::EUNARY:
    return "EUNARY";
  case AstNode::EBINARY:
    return "EBINARY";
  case AstNode::ECAST:
    return "ECAST";
  case AstNode::EMCALL:
    return "EMCALL";
  case AstNode::SASSIGN:
    return "SASSIGN";
  case AstNode::SWHILE:
    return "SWHILE";
  case AstNode::SFOR:
    return "SFOR";
  case AstNode::SIF:
    return "SIF";
  case AstNode::SRETURN:
    return "SRETURN";
  case AstNode::SBLOCK:
    return "SBLOCK";
  case AstNode::SMCALL:
    return "SMCALL";
  case AstNode::SEMPTY:
    return "SEMPTY";
  default:
    return "AstNode::kind2string: unkown AstNode kind";
  }
}
