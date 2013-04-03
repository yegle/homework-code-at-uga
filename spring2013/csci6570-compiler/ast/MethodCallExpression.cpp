#include "MethodCallExpression.h"


MethodCallExpression::MethodCallExpression( int lno, const char *cName, const char *mName, std::vector<Expression *> *args )
{
  setKind( EMCALL );
  setLineNo( lno );
  className    = cName;
  if( mName != NULL )
    methodName = mName;
  else
    throw AstException( "MethodCallExpression::MethodCallExpression: NULL method name\n" );
  entry        = NULL;
  arguments    = args;
  setType( TINT );   // set as the default expression type
}

void 
MethodCallExpression::addArgument( Expression *argument )
{
  if( arguments == NULL ) {
    arguments = new std::vector<Expression *>();
    if( arguments == NULL )
      throw( "Internal: MethodCallExpression::addArgument: failed to allocate arguments std::vector\n" );
  }
  if( argument == NULL )
    throw( "MethodCallExpression::addArgument: NULL argument\n" );

  arguments->push_back( argument );
}

void
MethodCallExpression::prependArgument( Expression *argument )
{
  std::vector<Expression *>::iterator pos;
  if( arguments == NULL ) {
    arguments = new std::vector<Expression *>();
    if( arguments == NULL )
      throw( "Internal: MethodCallExpression::prependArgument: failed to allocate arguments std::vector\n" );
  }
  if( argument == NULL )
    throw( "Internal: MethodCallExpression::prependArgument: NULL argument\n" );
  pos = arguments->begin();
  arguments->insert( pos, argument );
}
