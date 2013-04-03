// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __BLOCKSTMT_H__
#define __BLOCKSTMT_H__



#include <vector>


#include "AstException.h"
#include "Statement.h"
#include "AstVisitor.h"





/**
 * @brief This class represents a TinyJave block statement; local variables are not represented within a BlockStatement, but with a MethodDeclaration.
 * 
 * A block statement is a sequence of statements represented as a std::vector of Statement nodes.
 */
class BlockStatement: public Statement {

private:
  std::vector<Statement *>  *stmtList;
  
public:

  /** 
   * Create a new block statement node.
   * 
   * @param lno is a source line number (should be the beginning of the block).
   */
  BlockStatement( int lno );

  /** 
   * Destroy this object.
   */
  ~BlockStatement()
  {
    if( stmtList != NULL )
      delete stmtList;
  }

  /** 
   * Return the statements of this block.
   * 
   * @return a pointer to the std::vector with the Statement nodes
   */  
  std::vector<Statement *> *getStatements()
  {
    return stmtList;
  }

  /** 
   * Add a statement to this block statement at the end.
   * 
   * @param stmt a Statement to be added as the last in sequence.
   * @throws AstException if the argument is NULL
   */  
  void addStatement( Statement *stmt );

  /** 
   * Add another statement to this block statement in the front.
   * 
   * @param stmt a Statement to be added as the first in sequence.
   * @throws AstException if the argument is NULL
   */  
  void prependStatement( Statement *stmt );

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
