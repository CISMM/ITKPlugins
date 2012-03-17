/** This program handles the actual plugin code generation. */

#include <cstdlib>

#include "ClassDescription.h"
#include "Parser.h"


int main( int argc, char* argv[] )
{
  ClassDescription *description = new ClassDescription();

  Parser parser;
  parser.SetFileName( "/Users/quammen/dev/packages/SimpleITK/Code/BasicFilters/json/Convolution.json" );
  parser.Parse( description );

  description->PrintSelf( std::cout );

  delete description;

  return EXIT_SUCCESS;
}
