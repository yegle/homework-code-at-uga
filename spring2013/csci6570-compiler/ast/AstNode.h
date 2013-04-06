// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __ASTNODE_H__
#define __ASTNODE_H__

/**
 * 
 * @mainpage AST: Abstract Syntax Tree Library for TinyJava
 *
 * @section section_toc Abstract Syntax Tree Library for TinyJava
 *
 * @subsection OVERVIEW Overview
 *
 * The AST Library can be used to create an then traverse abstract syntax 
 * trees representing TinyJava programs.  The construction of an abstract
 * syntax tree is performed during parsing, by placing suitable AST Library
 * constructor (and other method) calls within the semantic actions of a
 * Bison or YACC specification file for TinyJava.
 *
 * The root of the syntax tree for a TinyJava program is a node representing
 * the description of the class declaration included in the program.
 *
 * @subsection NODETYPES Abstract Syntax Tree Node Types
 *
 * The AstNode class is the root of the syntax tree classes hierarchy.
 * The class has three direct subclasses: Declaration, Expression, and Statement,
 * which subdivide the remaining classes into three sections: TinyJava declarations, 
 * expressions, and statements, respectively.  The four classes mentioned above (AstNode, 
 * Declaration, Expression, and Statement) are abstract and therefore are not 
 * intended to have any instances.
 *
 * The AST Library has a number of concrete classes intended to represent all syntactic
 * constructs of TinyJava.
 *
 * Subclasses of the Declaration class include:
 * - #ClassDeclaration, which represents a class declaration, including its name and member Declarations, 
 *    each of which must be either a #FieldDeclaration or a #MethodDeclaration.
 * - #FieldDeclaration, which represents a declaration of a class field, including its name, 
 *    an initialization literal, and type, as described in @ref TYPES
 * - #MethodDeclaration, which represents a method declaration, including its name, return type, 
 *    parameters (ParameterDeclaration), local variables (VariableDeclaration) and a BlockStatement, 
 *    representing the method body
 * - #ParameterDeclaration, which represents a declaration of a formal parameter of a method, including its name and type, and
 * - #VariableDeclaration, which represents a declaration of a local variable in a method, including its name, type and an initialization.
 *
 * Subclasses of the Expression class include:
 * - #LiteralExpression, which represents a literal, represented by the literal string and the literal type
 * - #ReferenceExpression, which represents an identifier reference, including the identifier and a symbol table ENTRY pointer
 * - #UnaryExpression, which represents a unary expression, including an operand Expression and a unary (prefix or postfix) operator, as defined in @ref TYPES
 * - #BinaryExpression, which represents a binary expression, including two operands (of type Expression) and a binary operator
 * - #CastExpression, which represents a type cast expression, including a type of the cast and an operand Expression
 * - #MethodCallExpression, which represents a method call expression, including the method name and the arguments, each of type Expression
 * - #NewExpression, which represents a new expression (used for array creation), including the base type and an Expression specifying the array size.
 *
 * Finally, subclasses of the Statement class include:
 * - #AssignStatement, which represents an assignment statement, including a left-hand side identifier and a right-hand side Expression
 * - #BlockStatement, which represents a block statement, including a sequence of Statement nodes
 * - #EmptyStatement, which represents an empty statement
 * - #ForStatement, which represents a FOR statement, including a left-hand side identifier, three Expression nodes, and a Statement (the body)
 * - #IfStatement, which represents an IF statement, including an Expression, and one or two Statement nodes.
 * - #MethodCallStatement, which represents a method call statement, including an identifier (method name) and a sequence of Expression nodes (arguments)
 * - #ReturnStatement, which represents a return statement, including an optional Expression (value to be returned)
 * - #WhileStatement, which represents a WHILE statement, including an Expression and a Statement node (the body)
 *
 * Some of the above (concrete) classes, for example ReferenceExpression, can represent a pointer 
 * to the symbol table entry corresponding to the represented element.  The type of the symbol 
 * table entry is defined as a pre-processor macro called ENTRY and it should be a legal C++ class.
 * The AST library is not making any assumptions as to the representation of the symbol table
 * entry or its interface, except that it is a class.  The default value of the macro is Entry.
 * If your symbol table entry is a different class, you should define the ENTRY macro accordingly
 * and place the definition before you include the Ast.h file.
 *
 * @subsection TYPES Types and operators in Abstract Syntax Trees
 *
 * TinyJava types and operators are represented as public constants defined in the AstNode class.  
 * These constants can be used in your symbol table entries for field types, method return types and 
 * formal parameter types, as well as the types of local variables.
 *
 * The AstNode class defines a number int constants that represent TinyJava types.  These include:
 * - #AstNode::TVOID (void type representation)
 * - #AstNode::TINT (int type representation)
 * - #AstNode::TFLOAT (float type representation)
 * - #AstNode::TBOOL (boolean type representation)
 * - #AstNode::TSTRING (String type representation)
 * - #AstNode::TINTA (int[] type representation)
 * - #AstNode::TFLOATA (float[] type representation)
 * - #AstNode::TSTRINGA (String[] type representation)
 * - #AstNode::TREF (basic reference type representation, to be used for null expression type)
 *
 * Also, the AstNode class defines a number of public constants that represent TinyJava operators.  These include:
 * - #AstNode::ADDOP, which represents the "+" operator (prefix or infix)
 * - #AstNode::SUBOP, which represents the "-" operator (prefix or infix)
 * - #AstNode::MULOP, which represents the "*" operator
 * - #AstNode::DIVOP, which represents the "/" operator
 * - #AstNode::EQOP, which represents the "==" operator
 * - #AstNode::NEOP, which represents the "!=" operator
 * - #AstNode::LTOP, which represents the "<" operator
 * - #AstNode::GTOP, which represents the ">" operator
 * - #AstNode::LEOP, which represents the "<=" operator
 * - #AstNode::GEOP, which represents the ">=" operator
 * - #AstNode::INCOP, which represents the "++" operator (postfix), and
 * - #AstNode::DECOP, which represents the "--" operator (postfix).
 *
 * @subsection EXCEPTIONS Abstract Syntax Trees exceptions
 *
 * The AstException class is used to represent some problem within the library.
 * If one of the available AST Library constructors or other methods throws 
 * an AstException, the message indicates the type of the problem encountered.
 * 
 * @subsection VISITORS Abstract Syntax Trees visitors
 *
 * The AST Libary provides the AstVisitor class, which is the base abstract class for 
 * implementing a variety of visitors to abstract syntax trees representing TinyJava 
 * programs.  The visitors follow the well-known
 * <a href="http://sourcemaking.com/design_patterns/visitor">Visitor Design Pattern</a>.
 * The AstVisitor class provides a number of pure virtual methods, one for each of the
 * concrete classes included in the AstNode hierarchy.
 *
 * Once an abstract syntax tree for a TinyJava class has been created, specialized visitors
 * can print out the entire syntax tree (for debugging purposes), construct the symbol table
 * for the program, perform its semantic analysis, and finally generate the intermediate code.
 *
 * The AST Library includes a simple PrintVisitor class.  This class illustrates how to
 * perform a traversal of the entire syntax tree.  Additional visitors can be implemented in
 * a similar fashion.
 *
 * @subsection USING Using the AST Library
 * 
 * You may copy the source code of the AST Library into your own space and use
 * make to compile the library.  Alternatively, you may use a copy of the library
 * on <code>nike</code>, which is in my directory <code>/home/profs/kochut/csx570/ast</code>.  You should use
 * appropriate options on your <code>g++</code> compilation commands (for 
 * example, <code>-I, -L</code> and <code>-l</code>) 
 * to use the library.  After compilation, the AST Library directory should contain
 * the library file called <code>libAst.a</code>, which is a statically linked library.  A simple
 * example of creating an Expression syntax tree within a Bison specification file
 * is provided on <code>nike</code> in my directory <code>/home/profs/kochut/csx570/ast-example</code>.  
 * You should examine the included Makefile to find out how to specify the -I, -L, and -l options to g++.
 * Additional explanations will be provided in class.
 *
 * In order to use the library, you should include the header file <code>Ast.h</code>, for example
 * in your Bison specification file for TinyJava.  If you would like to store 
 * references to your symbol table entries in a syntax tree, and if your symbol table
 * entry class is not called Entry (upper case E), you should define the <code>ENTRY</code> macro
 * ahead of the <code>#include "Ast.h"</code> directive.
 * 
 */


// define the ENTRY macro, if not yet provided
#ifndef ENTRY
#define ENTRY Entry
#endif



// forward declaration of the AstVisitor class
class AstVisitor;


/**
 * @brief This is the root of the Abstract Syntax Tree class hierarchy.
 * 
 * An AstNode object represents an Abstract Syntax Tree (AST) Node.
 * The AstNode class hierarchy is subdivided into:
 * - #declarations (subclasses of Declaration),
 * - #expressions (subclasses of Expression), and
 * - #statements (subclasses of Statement).
 *
 * There are several types (kinds) of concrete AST nodes.  More specifically, 
 * a node can be:
 * - #a declaration node (a subclass of Declaration) of type:
 *    DCLASS, DMETHOD, DFIELD, DPARAMETER, DVARIABLE
 * - #an expression node (a subclass of Expression) of type:
 *    ELITERAL, ENAME, EUNARY, EBINARY, ECAST, EMCALL,
 * - #a statement node (a subclass of Statement) of type:
 *    SASSIGN, SWHILE, SIF, SRETURN, SBLOCK, SMCALL, SEMPTY
 * 
 */
class AstNode {

public: 

  /**
   * This enumeration type elements are used to represent the kinds of Abstract Syntax Tree nodes.
   * 
   */
  enum AstNodeKind {
    DCLASS,			/**< a class declaration node */
    DFIELD,			/**< a field declaration node */
    DMETHOD,			/**< a method declaration node */
    DPARAMETER,			/**< a parameter declaration node */
    DVARIABLE,			/**< a variable declaration node */
    ELITERAL,			/**< a literal expression node */
    ENAME,			/**< a name reference expression node (may be a variable, parameter, or a field) */
    ENEW,			/**< a NEW expression node (just for array construction) */
    EUNARY,			/**< a unary expression node (may be prefix or postfix) */
    EBINARY,			/**< a binary expression node */
    ECAST,			/**< a type cast expression node */
    EMCALL,			/**< a method call expression node */
    SASSIGN,			/**< an assignment statement node */
    SWHILE,			/**< a WHILE statement node */
    SFOR,			/**< a FOR statement node */
    SIF,			/**< an IF statement node */
    SRETURN,			/**< a return statement node */
    SBLOCK,			/**< a block statement node */
    SMCALL,			/**< a method call statement (a wrapper for a method call expression node) */
    SEMPTY			/**< an empty statement node */
  };

  /**
   * These constants are used to represent the TinyJava types.
   * 
   */
  static const int TVOID    = 0; /**< void type representation */
  static const int TINT     = 1; /**< int type representation */
  static const int TFLOAT   = 2; /**< float type representation */
  static const int TBOOL    = 3; /**< boolean type representation */
  static const int TSTRING  = 4; /**< String type representation */
  static const int TINTA    = 5; /**< int[] type representation */
  static const int TFLOATA  = 6; /**< float[] type representation */
  static const int TSTRINGA = 7; /**< String[] type representation */
  static const int TREF     = 8; /**< basic reference type representation (to be used for null expression type) */

  /**
   * These constants are used to represent the TinyJava operators.
   * 
   */
  static const int ADDOP = 0;	/**< '+' operator representation (prefix or infix) */
  static const int SUBOP = 1;	/**< '-' operator representation (prefix or infix) */
  static const int MULOP = 2;	/**< '*' operator */
  static const int DIVOP = 3;	/**< '*' operator */
  static const int EQOP  = 4;	/**< '==' operator */
  static const int NEOP  = 5;	/**< '!=' operator */
  static const int LTOP  = 6;	/**< '<' operator */
  static const int GTOP  = 7;	/**< '>' operator */
  static const int LEOP  = 8;	/**< '<=' operator */
  static const int GEOP  = 9;	/**< '>=' operator */
  static const int INCOP = 10;	/**< '++' operator (postfix) */
  static const int DECOP = 11;	/**< '--' operator (postfix) */

private:
  AstNodeKind       kind;   // node kind
  int		    lineNo; // corresponding line number in source code

protected:
  // constructor is protected; only construct objects of specialized classes
  AstNode() {}

public:

  /**
  * Return the type (kind) of this node.
  * @return the node kind (of type AstNodeKind)
  */
  AstNodeKind getKind()
  {
    return kind;
  }

  /**
  * Set the type (kind) of this node.
  * @param newKind is a new node type (kind).
  */
  void setKind( AstNodeKind newKind )
  {
    kind = newKind;
  }

  /**
  * Return the source line number of this node.
  * @return the line number
  */
  int getLineNo()
  {
    return lineNo;
  }

  /**
  * Set the source line number for this node
  * @param lno is a source code line number for this AstNode (a positive integer).
  */
  void setLineNo( int lno )
  {
    lineNo = lno;
  }

  /**
  * Accept a visitor to this node.
  * @param aVisitor is a visitor object of type AstVisitor.
  */
  virtual void accept( AstVisitor *aVisitor ) = 0;

  /** 
   * Return a string representation of a TinyJava type
   * 
   * @param tp a type representation, as defined in the AstNode class
   * 
   * @return a string representation of the type
   */
  static const char *type2string( int tp );

  /** 
   * Return a string representation of a TinyJava operator
   * 
   * @param op an operator representation, as defined in the AstNode class
   * 
   * @return a string representation of the operator
   */
  static const char *operator2string( int op );

  /** 
   * Return a string representation of an AstNode kind
   * 
   * @param kind an AstNodeKind value, as defined in the AstNode class
   * 
   * @return a string representation of the AstNode kind
   */
  static const char *kind2string( AstNodeKind kind );

};




#endif
