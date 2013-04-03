#include "IfStmt.h"


IfStatement::IfStatement( int lno, Expression *e, Statement *ts, Statement *es )
{
  setKind( SIF );
  setLineNo( lno );

  if( e != NULL )
    expr = e;
  else
    throw AstException( "IfStatement::IfStatement: NULL expression\n" );

  if( ts != NULL )
    thenStmt = ts;
  else
    throw AstException( "IfStatement::IfStatement: NULL then statement node\n" );

  elseStmt = es;
}
