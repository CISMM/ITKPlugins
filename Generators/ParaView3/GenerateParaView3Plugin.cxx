/** This program handles the actual plugin code generation. */

#include <cstdlib>

#include "ClassDescription.h"
#include "Parser.h"
#include "ParaView3PluginGenerator.h"


int main( int argc, char* argv[] )
{

  if ( argc < 3 )
    {
    std::cout << "Usage: " << argv[0] << " <plugin JSON file> <output file prefix> " << std::endl;

    return EXIT_FAILURE;
    }

  ClassDescription *description = new ClassDescription();

  // Parse the class description
  Parser parser;
  parser.SetFileName( argv[1] );
  parser.Parse( description );

  description->PrintSelf( std::cout );

  // Generate a Slicer 4 plugin from this description
  ParaView3PluginGenerator * paraview3Generator = new ParaView3PluginGenerator();
  paraview3Generator->SetClassDescription( description );
  paraview3Generator->SetPluginPath( argv[2] );

  bool success = paraview3Generator->Generate();
  if ( !success )
    {
    std::cerr << "Error generating source files!" << std::endl;
    return EXIT_FAILURE;
    }

  delete paraview3Generator;
  delete description;

  return EXIT_SUCCESS;
}
