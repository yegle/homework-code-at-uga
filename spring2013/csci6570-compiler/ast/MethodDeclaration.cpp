// Gnu Emacs C++ mode:  -*- C++ -*-


#include "MethodDeclaration.h"


MethodDeclaration::MethodDeclaration( int lno, const char *nm, int rt )
{
  setKind( DMETHOD );
  setLineNo( lno );
  if( nm != NULL )
    name     = nm;
  else
    throw AstException( "MethodDeclaration::MethodDeclaration: the method name is NULL\n" );
  retType  = rt;
  isPublic = false;
  params   = new std::vector<Declaration *>();
  if( params == NULL )
    throw AstException( "Internal: MethodDeclaration::MethodDeclaration: failed to create a params std::vector\n" );
  vars     = new std::vector<Declaration *>();
  if( vars == NULL )
    throw AstException( "Internal: MethodDeclaration::MethodDeclaration: failed to create a vars std::vector\n" );
  entry    = NULL;
  body     = NULL;
}

void
MethodDeclaration::setParameters( std::vector<Declaration *> *ps )
{
  // check if all is correct
  for( int i = 0; i < ps->size(); i++ )
    if( ps->at( i ) == NULL )
      throw AstException( "MethodDeclaration::setParameters: one of the Declaration pointers is NULL\n" );
    else if( ps->at( i )->getKind() != AstNode::DPARAMETER )
      throw AstException( "MethodDeclaration::setParameters: one of the Declaration pointers is not a ParameterDeclaration\n" );

  params = ps;
}

void
MethodDeclaration::setVariables( std::vector<Declaration *> *vs )
{
  // check if all is correct
  for( int i = 0; i < vs->size(); i++ )
    if( vs->at( i ) == NULL )
      throw AstException( "MethodDeclaration::setVariables: one of the Declaration pointers is NULL\n" );
    else if( vs->at( i )->getKind() != AstNode::DVARIABLE )
      throw AstException( "MethodDeclaration::setVariables: one of the Declaration pointers is not a VariableDeclaration\n" );

  vars = vs;
}

