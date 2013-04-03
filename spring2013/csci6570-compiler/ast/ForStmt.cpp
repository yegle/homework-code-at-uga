#include "ForStmt.h"


ForStatement::ForStatement( int lno, const char *n, Expression *i, Expression *t, Expression *u, Statement *b )
{
  setKind( SFOR );
  setLineNo( lno );
  entry    = NULL;
  index    = NULL;

  if( n != NULL )
    lhsName  = n;
  else
    throw AstException( "ForStatement::ForStatement: NULL name expression\n" );

  if( i != NULL )
    init = i;
  else
    throw AstException( "ForStatement::ForStatement: NULL init expression\n" );

  if( t != NULL )
    term = t;
  else
    throw AstException( "ForStatement::ForStatement: NULL term expression\n" );

  if( u != NULL )
    upd = u;
  else
    throw AstException( "ForStatement::ForStatement: NULL update expression\n" );

  if( b != NULL )
    body = b;
  else
    throw AstException( "ForStatement::ForStatement: NULL statement body\n" );
}

// create a FOR statement; x is the index expression to an array ref
ForStatement::ForStatement( int lno, const char *n, Expression *x, Expression *i, Expression *t, Expression *u, Statement *b )
{
  setKind( SFOR );
  setLineNo( lno );
  entry    = NULL;

  if( n != NULL )
    lhsName  = n;
  else
    throw AstException( "ForStatement::ForStatement: NULL name expression\n" );

  if( x != NULL )
    index = x;
  else
    throw AstException( "ForStatement::ForStatement: NULL index expression\n" );

  if( i != NULL )
    init = i;
  else
    throw AstException( "ForStatement::ForStatement: NULL init expression\n" );

  if( t != NULL )
    term = t;
  else
    throw AstException( "ForStatement::ForStatement: NULL term expression\n" );

  if( u != NULL )
    upd = u;
  else
    throw AstException( "ForStatement::ForStatement: NULL update expression\n" );

  if( b != NULL )
    body = b;
  else
    throw AstException( "ForStatement::ForStatement: NULL statement body\n" );
}
