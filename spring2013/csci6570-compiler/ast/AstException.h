// Gnu Emacs C++ mode:  -*- C++ -*-



#ifndef __ASTEXCEPTION_H__
#define __ASTEXCEPTION_H__


#include <string>
#include <stdexcept>



/**
 * @brief This class represents an exception that may occur within the Abstract Syntax Tree Library for TinyJava.
 * 
 * An AstException includes a message indicating the cause for the exception.
 */
class AstException: public std::runtime_error {

public:

  /** 
   * Construct an AstException exception object indicating
   * a problem within an Abstract Syntax Tree.
   * 
   * @param msg a message describing the error.
   */
  AstException( const std::string &msg )
    : std::runtime_error( msg )
  {}

};


#endif
