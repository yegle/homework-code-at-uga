#include "BlockStmt.h"



BlockStatement::BlockStatement( int lno )
{
  setKind( SBLOCK );
  setLineNo( lno );
}

void 
BlockStatement::addStatement( Statement *stmt )
{
  if( stmtList == NULL ) {
    stmtList = new std::vector<Statement *>();
    if( stmtList == NULL )
      throw AstException( "Internal: BlockStatement::addStatement: failed to allocate stmtList std::vector\n" );
  }
  if( stmt == NULL )
    throw AstException( "BlockStatement::addStatement: NULL statement\n" );
  else
    stmtList->push_back( stmt );
}

void 
BlockStatement::prependStatement( Statement *stmt )
{
  std::vector<Statement *>::iterator pos;
  if( stmtList == NULL ) {
    stmtList = new std::vector<Statement *>();
    if( stmtList == NULL )
      throw AstException( "Internal: BlockStatement::prependStatement: failed to allocate stmtList std::vector\n" );
  }
  if( stmtList == NULL )
    throw AstException( "BlockStatement::prependStatement: NULL statement\n" );
  else {
    pos = stmtList->begin();
    stmtList->insert( pos, stmt );
  }
}
