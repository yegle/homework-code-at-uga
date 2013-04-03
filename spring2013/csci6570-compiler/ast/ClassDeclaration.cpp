#include "ClassDeclaration.h"


ClassDeclaration::ClassDeclaration( int lno, const char *n )
{
  setKind( DCLASS );
  setLineNo( lno );
  name     = n;
  members  = new std::vector<Declaration *>();
  if( members == NULL )
    throw AstException( "Internal error: ClassDeclaration::ClassDeclaration: failed to create members std::vector\n" );
}

void
ClassDeclaration::addMember( Declaration *member )
{
  if( members == NULL ) {
    members = new std::vector<Declaration *>();
    if( members == NULL )
      throw AstException( "Internal: ClassDeclaration::addMember: failed to allocate operands std::vector\n" );
  }
  if( members == NULL ) {
    throw AstException( "Internal: ClassDeclaration::addMember: NULL member\n" );
  }
  else {
    if( member->getKind() != AstNode::DFIELD &&
	member->getKind() != AstNode::DMETHOD )
      throw AstException( "ClassDeclaration::addMember: illegal argument type; must be a pointer to FieldDeclaration or MethodDeclaration" );
    members->push_back( member );
  }
}
