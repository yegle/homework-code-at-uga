#include "BinaryExpression.h"


BinaryExpression::BinaryExpression( int lno, int op, Expression *left, Expression *right )
{
  setKind( EBINARY );
  setLineNo( lno );
  oper         = op;
  setType( 0 );   // set as the default expression type
  if( left != NULL )
    leftOperand = left;
  else
    throw AstException( "BinaryExpression::BinaryExpression: NULL left operand\n" );
  if( right != NULL )
    rightOperand = right;
  else
    throw AstException( "BinaryExpression::BinaryExpression: NULL right operand\n" );
}
