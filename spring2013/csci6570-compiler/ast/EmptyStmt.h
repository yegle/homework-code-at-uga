// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __EMPTYSTMT_H__
#define __EMPTYSTMT_H__


#include "AstVisitor.h"
#include "Statement.h"




/**
 * @brief This class represents a TinyJava empty statement.
 * 
 */
class EmptyStatement: public Statement {

public:

  /** 
   * Create a EmptyStatement object.
   * 
   * @param lno a source code line number with the empty statement.
   * 
   */
  EmptyStatement( int lno )
  {
    setKind( SEMPTY );
    setLineNo( lno );
  }

  /**
  * Accept a visitor to this node.
  * @param aVisitor is a visitor object of type AstVisitor.
  */
  void accept( AstVisitor *aVisitor )
  {
    aVisitor->visit( this );
  }

};




#endif
