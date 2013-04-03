#include "AssignStmt.h"


AssignStatement::AssignStatement( int lno, const char *name, Expression *rhs )
{
  setKind( SASSIGN );
  setLineNo( lno );
  lhsName      = name;
  entry        = NULL;
  index        = NULL;
  if( rhs != NULL )
    expr = rhs;
  else
    throw AstException( "AssignStatement::AssignStatement: NULL rhs expression\n" );
}

AssignStatement::AssignStatement( int lno, const char *name, Expression *idx, Expression *rhs )
{
  setKind( SASSIGN );
  setLineNo( lno );
  lhsName      = name;
  entry        = NULL;
  index        = idx;
  if( rhs != NULL )
    expr = rhs;
  else
    throw AstException( "AssignStatement::AssignStatement: NULL rhs expression\n" );
}
