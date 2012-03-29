/** This program handles the actual plugin code generation. */

#include <cstdlib>

#include "ClassDescription.h"
#include "Parser.h"
#include "ParaView3PluginGenerator.h"


int main( int argc, char* argv[] )
{

  if ( argc < 4 )
    {
    std::cout << "Usage: " << argv[0] << " <plugin JSON file> <output file prefix> [ --xml | --cxx ]" << std::endl;

    return EXIT_FAILURE;
    }

  ClassDescription *description = new ClassDescription();

  // Parse the class description
  Parser parser;
  parser.SetFileName( argv[1] );
  parser.Parse( description );

  description->PrintSelf( std::cout );

  // Generate a Slicer 4 plugin from this description
  ParaView3PluginGenerator * slicer4Generator = new ParaView3PluginGenerator();
  slicer4Generator->SetClassDescription( description );
  slicer4Generator->SetPluginPath( argv[2] );

  if ( strncmp( "--xml", argv[3], 6 ) == 0 )
    {
    bool success = slicer4Generator->GenerateXML();
    if ( !success )
      {
      std::cerr << "Error generating XML" << std::endl;
      return EXIT_FAILURE;
      }
    }
  else if ( strncmp( "--cxx", argv[3], 6 ) == 0 )
    {
    bool success = slicer4Generator->GenerateCode();
    if ( !success )
      {
      std::cerr << "Error generating CXX" << std::endl;
      return EXIT_FAILURE;
      }
    }

  delete slicer4Generator;
  delete description;

  return EXIT_SUCCESS;
}
