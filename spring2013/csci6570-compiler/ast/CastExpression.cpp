#include "CastExpression.h"


CastExpression::CastExpression( int lno, int ct, Expression *arg )
{
  setKind( ECAST );
  setLineNo( lno );
  castTo = ct;
  setType( 0 );   // set as the default expression type
  if( arg != NULL )
    operand = arg;
  else
    throw AstException( "CastExpression::CastExpression: NULL arg\n" );
}
