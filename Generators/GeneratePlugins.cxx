/** This program handles the actual plugin code generation. */

#include <cstdlib>

#include "ClassDescription.h"
#include "Parser.h"
#include "Slicer4PluginGenerator.h"


int main( int argc, char* argv[] )
{
  ClassDescription *description = new ClassDescription();

  // Parse the class description
  Parser parser;
  parser.SetFileName( "/Users/quammen/dev/packages/SimpleITK/Code/BasicFilters/json/AddConstantTo.json" );
  parser.Parse( description );

  description->PrintSelf( std::cout );

  // Generate a Slicer 4 plugin from this description
  Slicer4PluginGenerator * slicer4Generator = new Slicer4PluginGenerator();
  slicer4Generator->SetClassDescription( description );
  slicer4Generator->SetPluginPath( "/tmp/Plugins" );
  slicer4Generator->Generate();

  delete slicer4Generator;
  delete description;

  return EXIT_SUCCESS;
}
