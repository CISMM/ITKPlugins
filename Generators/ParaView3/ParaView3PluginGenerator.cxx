#include "ParaView3PluginGenerator.h"

#include <fstream>
#include <iostream>
#include <string>

#include "ITKPluginsConfigure.h"
#include "ClassDescription.h"
#include "Enumeration.h"
#include "MemberDescription.h"

#include "sitkConfigure.h"

ParaView3PluginGenerator
::ParaView3PluginGenerator()
{
}

ParaView3PluginGenerator
::~ParaView3PluginGenerator()
{
}

bool
ParaView3PluginGenerator
::Generate()
{
  if ( !this->GetClassDescription() )
    {
    std::cerr << "No ClassDescription set in Slicer4PluginGenerator" << std::endl;
    return false;
    }

  if ( !this->GenerateXML() )
    {
    std::cerr << "Failed to generate XML description for plugin '" << this->GetPluginPath() << "/"
              << this->GetClassDescription()->GetPluginName() << "'.";
    return false;
    }

  if ( !this->GenerateCode() )
    {
    std::cerr << "Failed to generate code for plugin '" << this->GetClassDescription()->GetPluginName()
              << "/" << this->GetClassDescription()->GetPluginName() << "'.";
    return false;
    }

  return true;
}

bool
ParaView3PluginGenerator
::GenerateXML()
{
  ClassDescription * classDescription = this->GetClassDescription();

  // Open file for writing
  std::string filePath( this->GetPluginPath() );
  char lastChar = *( this->GetPluginPath().rbegin() );
  if ( lastChar != '/' && lastChar != '\\' )
    {
    // Add delimiter
#ifdef WIN32
    filePath.append( "\\" );
#else
    filePath.append( "/" );
#endif
    }
  filePath.append( classDescription->GetPluginName() );
  filePath.append( ".xml" );

  std::cout << "Writing ParaView3 plugin XML file for " << classDescription->GetPluginName()
            << std::endl;

  std::ofstream os( filePath.c_str() );

  os << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  os << "<ServerManagerConfiguration>\n";
  os << "  <ProxyGroup name=\"filters\">\n";

  os << "    <SourceProxy name=\"" << classDescription->GetPluginName() << "\" class=\"vtkITK"
     << classDescription->GetPluginName() << "\" label=\""
     << this->SplitCAMLCaseString( classDescription->GetPluginName() ) << "\">\n";

  std::string shortHelp = classDescription->GetBriefDescription();
  shortHelp = this->SubstituteString( "\n", "\\n",  shortHelp);
  shortHelp = this->SubstituteString( "\"", "&quot;", shortHelp);

  os << "      <Documentation\n";
  os << "        long_help=\"\"\n"; // Should put detailed description here
  os << "        short_help=\""
     << shortHelp << "\">\n";
  os << "      </Documentation>\n\n";

  // Write input descriptions
  for (int i = 0; i < this->GetNumberOfInputs(); ++i)
    {
    os << "      <InputProperty\n";

    os << "          name=\"Input";
    if ( this->GetNumberOfInputs() > 1 )
      {
      os << i+1;
      }
    os << "\"\n";

    os << "         port_index=\"" << i << "\"\n";
    os << "         command=\"SetInputConnection\">\n";

    os << "        <ProxyGroupDomain name=\"groups\">\n";
    os << "          <Group name=\"sources\"/>\n";
    os << "          <Group name=\"filters\"/>\n";
    os << "        </ProxyGroupDomain>\n";

    os << "        <DataTypeDomain name=\"input_type\">\n";
    os << "          <DataType value=\"vtkImageData\"/>\n";
    os << "        </DataTypeDomain>\n";

    os << "        <InputArrayDomain name=\"input_array" << i+1 << "\"\n";
    os << "                          attribute_type=\"point\"\n";
    os << "                          number_of_components=\"1\"/>\n";

    os << "        <Documentation>\n";
    os << "          Set input image " << i+1 << "\n";
    os << "        </Documentation>\n";

    os << "      </InputProperty>\n\n";
    }

  // Give user the choice of which scalar to use from the input images
  for (int i = 0; i < this->GetNumberOfInputs(); ++i)
    {
    os << "      <StringVectorProperty\n";
    os << "          name=\"SelectInputScalars" << i+1 << "\"\n";
    os << "          command=\"SetInputArrayToProcess\"\n";
    os << "          number_of_elements=\"5\"\n";
    os << "          element_types=\"0 0 0 0 2\"\n";
    os << "          animateable=\"0\"\n";
    os << "          label=\"Scalar to Process";
    if ( this->GetNumberOfInputs() > 1 )
      {
      os << " from Input " << i+1;
      }
    os << "\">\n";
    os << "        <ArrayListDomain\n";
    os << "           name=\"array_list\"\n";
    os << "           attribute_type=\"Scalars\"\n";
    os << "           input_domain_name=\"input_array" << i+1 << "\">\n";
    os << "          <RequiredProperties>\n";
    os << "            <Property name=\"Input";
    if ( this->GetNumberOfInputs() > 1 )
      {
      os << i+1;
      }
    os << "\" function=\"Input\"/>\n";
    os << "          </RequiredProperties>\n";
    os << "        </ArrayListDomain>\n";
    os << "        <Documentation>Set active scalar field for input";
    if ( this->GetNumberOfInputs() > 1 )
      {
      os << i+1;
      }
    os << "</Documentation>\n";
    os << "      </StringVectorProperty>\n\n";
    }

  for (int i = 0; i < classDescription->GetNumberOfMemberDescriptions(); ++i)
    {
    const MemberDescription * member = classDescription->GetMemberDescription( i );

    std::string typeName = member->GetTypeName();
    std::string xmlTypeName;
    std::string xmlRangeDomainName;

    if ( this->IsIntVectorType( typeName ) || classDescription->IsEnumerationType( typeName ) )
      {
      xmlTypeName = "IntVectorProperty";
      xmlRangeDomainName = "IntRangeDomain";
      }
    else if ( this->IsDoubleVectorType( typeName ) )
      {
      xmlTypeName = "DoubleVectorProperty";
      xmlRangeDomainName = "DoubleRangeDomain";
      }
    else
      {
      std::cerr << "Unknown type '" << typeName << "' encountered!" << std::endl;
      return false;
      }

    os << "      <" << xmlTypeName << "\n";
    os << "         name=\"" << member->GetMemberName() << "\"\n";
    os << "         command=\"Set" << member->GetMemberName() << "\"\n";
    int numberOfElements = member->GetNumberOfElements();
    os << "         number_of_elements=\"" << numberOfElements << "\"\n";
    os << "         default_values=\"";

    // Print default values
    std::string elementDefaultValue = member->GetElementDefaultValue();
    for (int i = 0; i < numberOfElements; ++i)
      {
      if ( elementDefaultValue == "true" )
        {
        os << "1";
        }
      else if ( elementDefaultValue == "false" )
        {
        os << "0";
        }
      else if ( classDescription->IsEnumerationType( member->GetTypeName() ) )
        {
        const Enumeration * enumeration = classDescription->GetEnumeration( member->GetTypeName() );
        os << enumeration->GetEnumerantValueForName( member->GetDefaultValue() );
        }
      else
        {
        os << elementDefaultValue;
        }
      if ( i < numberOfElements-1 )
        {
        os << " ";
        }
      else
        {
        os << "\">\n";
        }
      }

    bool isVectorType = this->IsBoolVectorType( typeName );
    if ( isVectorType )
      {
      os << "        <BooleanDomain name=\"bool\"/>\n";
      }

    bool isEnumerationType = classDescription->IsEnumerationType( typeName );
    if ( isEnumerationType )
      {
      os << "        <EnumerationDomain name=\"enum\">\n";

      const Enumeration * enumeration = classDescription->GetEnumeration( typeName );
      for (int j = 0; j < enumeration->GetNumberOfEnumerants(); ++j )
        {
        os << "          <Entry value=\"" << enumeration->GetEnumerantValue( j )
           << "\" text=\"" << enumeration->GetEnumerantName( j ) << "\"/>\n";
        }

      os << "        </EnumerationDomain>\n";
      }

    // Finish the XML element
    if ( !isVectorType && !isEnumerationType )
      {
      os << "        <" << xmlRangeDomainName << " name=\"range\"/>\n\n";
      }

    os << "      </" << xmlTypeName << ">\n\n";

    }

  os << "    </SourceProxy>\n";
  os << "  </ProxyGroup>\n";
  os << "</ServerManagerConfiguration>\n";

  os.flush();
  os.close();

  return true;
}

bool
ParaView3PluginGenerator
::GenerateCode()
{
  bool success = this->GenerateHeaderCode();
  if ( !success )
    {
    return false;
    }

  success = this->GenerateCXXCode();
  if ( !success )
    {
    return false;
    }

  return true;
}

bool
ParaView3PluginGenerator
::GenerateHeaderCode()
{
  // Useful variables
  ClassDescription * classDescription = this->GetClassDescription();
  std::string itkClassName( this->GetITKClassName() );
  std::string vtkClassName( this->GetVTKClassName() );

  // Open file for writing
  std::string headerFilePath( this->GetPluginPath() );
  char lastChar = *( this->GetPluginPath().rbegin() );
  if ( lastChar != '/' || lastChar != '\\' )
    {
    // Add delimiter
#ifdef WIN32
    headerFilePath.append( "\\" );
#else
    headerFilePath.append( "/" );
#endif
    }
  headerFilePath.append( vtkClassName );
  headerFilePath.append( ".h" );

  std::ofstream os( headerFilePath.c_str() );

  os << "#ifndef __" << vtkClassName << "_h_\n";
  os << "#define __" << vtkClassName << "_h_\n\n";

  os << "#include <vtkImageAlgorithm.h>\n";

  os << "class VTK_EXPORT " << vtkClassName << " : public vtkImageAlgorithm\n";
  os << "{\n";
  os << "public:\n";
  os << "  static " << vtkClassName << "* New();\n";
  os << "  vtkTypeMacro(" << vtkClassName << ", vtkImageAlgorithm);\n\n";

  os << "  typedef " << vtkClassName << " Self;\n\n";

  os << "  void SetActiveScalar( char * scalar);\n\n";

  // Write members
  for (int i = 0; i < classDescription->GetNumberOfMemberDescriptions(); ++i)
    {
    const MemberDescription * member = classDescription->GetMemberDescription( i );

    const Enumeration * enumeration = classDescription->GetEnumeration( member->GetTypeName() );
    if ( enumeration )
      {
      // Write enumeration
      os << "  enum {\n";
      int j = 0;
      for (j = 0; j < enumeration->GetNumberOfEnumerants()-1; ++j)
        {
        os << "    " << enumeration->GetEnumerantName( j ) << " = "
           << enumeration->GetEnumerantValue(j ) << ",\n";
        }
      os << "    " << enumeration->GetEnumerantName( j ) << " = "
         << enumeration->GetEnumerantValue( j ) << "\n";
      os << "  };\n\n";

      os << "  typedef int " << member->GetTypeName() << ";\n\n";
      }

    if ( member->GetNumberOfElements() == 1 )
      {
      os << "  // Set/get " << member->GetMemberName() << " variable\n";
      os << "  vtkSetMacro(" << member->GetMemberName() << ", ";
      if ( enumeration)
        {
        os << "int);\n";
        }
      else
        {
        os << this->GetVTKTypeName( member->GetTypeName() ) << ");\n";
        }
      os << "  vtkGetMacro(" << member->GetMemberName() << ", ";
      if ( enumeration )
        {
        os << "int);\n\n";
        }
      else
        {
        os << this->GetVTKTypeName( member->GetTypeName() ) <<");\n\n";
        }
      }
    else
      {
      os << "  // Set/get " << member->GetMemberName() << " variable\n";
      os << "  vtkSetVectorMacro(" << member->GetMemberName() << ", "
         << this->GetVTKTypeName( member->GetTypeName() )
         << ", " << member->GetNumberOfElements() << ");\n";
      os << "  vtkGetVectorMacro(" << member->GetMemberName() << ", "
         << this->GetVTKTypeName( member->GetElementTypeName() )
         << ", " << member->GetNumberOfElements() << ");\n\n";
      }

    }

  // Protected members
  os << "protected:\n";
  os << "  " << vtkClassName << "();\n";
  os << "  virtual ~" << vtkClassName << "();\n\n";

  os << "  virtual int FillInputPortInformation(int port, vtkInformation *info);\n\n";

  os << "  virtual int RequestUpdateExtent(vtkInformation *, vtkInformationVector **, vtkInformationVector *);\n\n";

  os << "  virtual int RequestInformation(vtkInformation *vtkNotUsed(request), vtkInformationVector **inputVector, vtkInformationVector * outputVector);\n\n";

  os << "  int RequestData(vtkInformation * info, vtkInformationVector **inputVector, vtkInformationVector *outputVector);\n\n";

  os << "  void PrintSelf(ostream& os, vtkIndent indent);\n\n";

  // Private members
  os << "private:\n";

  os << "  " << vtkClassName << "(const " << vtkClassName << "&); // Purposely not implemented.\n";
  os << "  void operator=(const " << vtkClassName << "&); // Purposely not implemented.\n\n";

  // Run method declaration
  if ( classDescription->GetNumberOfInputs() == 1 )
    {
    os << "  template< class TInputPixel >\n";
    os << "  bool Run(TInputPixel *, vtkImageData * input, vtkImageData * output);\n\n";
    }
  else
    {
    os << "  template< class TInputPixel0, class TInputPixel1 >\n";
    os << "  bool Run(TInputPixel0 *, TInputPixel1 *, vtkImageData * input0, vtkImageData * input1, vtkImageData * output);\n\n";
    }

  for (int i = 0; i < classDescription->GetNumberOfMemberDescriptions(); ++i)
    {
    const MemberDescription * member = classDescription->GetMemberDescription( i );

    if ( member->GetNumberOfElements() == 1 )
      {
      if ( classDescription->IsEnumerationType( member->GetTypeName() ) )
        {
        os << "  int " << member->GetMemberName() << ";\n\n";
        }
      else
        {
        os << "  " << this->GetVTKTypeName( member->GetTypeName() ) << " " << member->GetMemberName() << ";\n\n";
        }
      }
    else
      {
      os << "  " << this->GetVTKTypeName( member->GetTypeName() ) << " " << member->GetMemberName() << "["
         << member->GetNumberOfElements() << "];\n\n";
      }
    }

  os << "};\n\n";

  os << "#endif\n";

  os.flush();
  os.close();

  return true;
}

bool
ParaView3PluginGenerator
::GenerateCXXCode()
{
  // Useful variables
  const ClassDescription * classDescription = this->GetClassDescription();
  std::string itkClassName( this->GetITKClassName() );
  std::string vtkClassName( this->GetVTKClassName() );

  // Open file for writing
  std::string headerFilePath( this->GetPluginPath() );
  char lastChar = *( this->GetPluginPath().rbegin() );
  if ( lastChar != '/' || lastChar != '\\' )
    {
    // Add delimiter
#ifdef WIN32
    headerFilePath.append( "\\" );
#else
    headerFilePath.append( "/" );
#endif
    }
  headerFilePath.append( vtkClassName );
  headerFilePath.append( ".cxx" );

  std::ofstream os( headerFilePath.c_str() );

  //os << "#include \"vtkITKImageFilter.h\"\n\n";

  os << "#include <itk" << itkClassName << ".h>\n";
  os << "#include <" << vtkClassName << ".h>\n\n";

#ifdef SITK_HAS_STLTR1_TR1_FUNCTIONAL
  os << "#include <tr1/functional>\n";
#endif
#ifdef SITK_HAS_STLTR1_FUNCTIONAL
  os << "#include <functional>\n";
#endif
#ifdef SITK_HAS_STLTR1_TR1_TYPE_TRAITS
  os << "#include <tr1/type_traits>\n";
#endif
#ifdef SITK_HAS_STLTR1_TYPE_TRAITS
  os << "#include <type_traits>\n";
#endif
  os << "\n";

  // Add additional files requested in the ClassDescription
  for ( int i = 0; i < this->GetClassDescription()->GetNumberOfIncludeFiles(); ++i )
    {
    os << "#include <" << this->GetClassDescription()->GetIncludeFile( i ) << ">\n";
    }
  os << "\n";

  os << "#include <vtkDataArray.h>\n";
  os << "#include <vtkObjectFactory.h>\n";
  os << "#include <vtkImageData.h>\n";
  os << "#include <vtkInformationVector.h>\n";
  os << "#include <vtkInformation.h>\n";
  os << "#include <vtkPointData.h>\n";
  os << "#include <vtkStreamingDemandDrivenPipeline.h>\n\n";

  os << "#include <itkImageToVTKImageFilter.h>\n";
  os << "#include <itkVTKImageToImageFilter.h>\n\n";

#ifdef WIN32
  // This is probably bad, but it makes up for the fact that stdint.h
  // isn't available in Visual Studio.
  os << "typedef unsigned char uint8_t;\n";
  os << "typedef unsigned int  uint32_t;\n\n";
#endif

  this->WritePixelTypeDefinitions( os );
  os << "\n";

  os << "vtkStandardNewMacro(" << vtkClassName << ");\n\n";

  // Constructor
  os << vtkClassName << "::" << vtkClassName << "()\n";
  os << "{\n";

  // Set the number of input and output ports
  os << "  this->SetNumberOfInputPorts(" << classDescription->GetNumberOfInputs() << ");\n";
  os << "  this->SetNumberOfOutputPorts(1);\n\n";

  //os << "  this->Init();\n";

  // Initialize default values
  for ( int i = 0; i < classDescription->GetNumberOfMemberDescriptions(); ++i )
    {
    const MemberDescription * member = classDescription->GetMemberDescription( i );

    if ( classDescription->IsEnumerationType( member->GetTypeName() ) )
      {
      os << "  this->" << member->GetMemberName() << " = " << member->GetDefaultValue() << ";\n";
      }
    else
      {
      int numberOfElements = member->GetNumberOfElements();
      if ( numberOfElements == 1 )
        {
        os << "  this->" << member->GetMemberName() << " = " << member->GetDefaultValue();
        if ( member->GetTypeName() == "bool" )
          {
          os << " ? 1 : 0";
          }
        os << ";\n";
        }
      else
        {
        for (int i = 0; i < numberOfElements; ++i)
          {
          os << "  this->" << member->GetMemberName() << "[" << i << "] = " << member->GetElementDefaultValue();
          if ( member->GetElementTypeName() == "bool" )
            {
            os << " ? 1 : 0";
            }
          os << ";\n";
          }
        }
      }
    }

  os << "}\n\n";

  // Destructor
  os << vtkClassName << "::~" << vtkClassName << "()\n";
  os << "{\n";
  os << "}\n\n";

  // Set active scalar method
  os << "void " << vtkClassName << "::SetActiveScalar( char * scalar )\n";
  os << "{\n";
  os << "  std::cout << \"Setting active scalar to '\" << scalar << \"'\\n\";\n";
  os << "}\n\n";

  // RequestUpdateExtent() method
  os << "int " << vtkClassName << "::RequestUpdateExtent(\n";
  os << "  vtkInformation * vtkNotUsed(request),\n";
  os << "  vtkInformationVector **inputVector,\n";
  os << "  vtkInformationVector * vtkNotUsed(outputVector))\n";
  os << "{\n";

  os << "  int inWExt[6];\n";

  for (int i = 0; i < classDescription->GetNumberOfInputs(); ++i)
    {
    os << "  vtkInformation *inInfo" << i << " = inputVector[" << i << "]->GetInformationObject(0);\n";
    os << "  inInfo" << i << "->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), inWExt);\n";
    os << "  inInfo" << i << "->Set(vtkStreamingDemandDrivenPipeline::UPDATE_EXTENT(), inWExt, 6);\n\n";
    }

  os << "  return 1;\n";

  os << "}\n\n";

  // FillInputPortInformation() method
  os << "int " << vtkClassName << "::FillInputPortInformation(int port, vtkInformation * info)\n";
  os << "{\n";
  os << "  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), \"vtkImageData\");\n";
  os << "  info->Set(vtkAlgorithm::INPUT_IS_REPEATABLE(), 0);\n";
  os << "  info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 0);\n\n";

  os << "  if (port == 0";
  for (int i = 1; i < classDescription->GetNumberOfInputs(); ++i)
    {
    os << " || port == " << i;
    }
  os << ")\n";
  os << "    {\n";
  os << "    return 1;\n";
  os << "    }\n\n";

  os << "  return 0;\n";
  os << "}\n\n";

  os << "int " << vtkClassName << "::RequestInformation(vtkInformation *vtkNotUsed(request), vtkInformationVector **inputVector, vtkInformationVector *outputVector)\n";
  os << "{\n";
  os << "  double spacing[3];\n";
  os << "  double origin[3];\n";
  os << "  int wholeExt[6];\n\n";

  os << "  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);\n";
  os << "  vtkInformation *outInfo = outputVector->GetInformationObject(0);\n\n";

  os << "  inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), wholeExt);\n";
  os << "  inInfo->Get(vtkDataObject::SPACING(), spacing);\n";
  os << "  inInfo->Get(vtkDataObject::ORIGIN(), origin);\n\n";

  os << "  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), wholeExt, 6);\n";
  os << "  outInfo->Set(vtkDataObject::SPACING(), spacing, 3);\n";
  os << "  outInfo->Set(vtkDataObject::ORIGIN(), origin, 3);\n\n";

  os << "  return 1;\n";
  os << "}\n\n";

  // RequestData() method
  os << "int " << vtkClassName << "::RequestData(vtkInformation * vtkNotUsed(info), vtkInformationVector **inputVector, vtkInformationVector *outputVector)\n";
  os << "{\n";

  if ( classDescription->GetNumberOfInputs() == 1 )
    {
    if ( !this->InsertFile( os, ITKPlugins_SOURCE_DIR "/Generators/ParaView3/RunOneInput.h" ) )
      {
      std::cerr << "Could not find file RunOneInput.h" << std::endl;
      }
    }
  else
    {
    if ( !this->InsertFile( os, ITKPlugins_SOURCE_DIR "/Generators/ParaView3/RunTwoInputs.h" ) )
      {
      std::cerr << "Could not find file RunTwoInputs.h" << std::endl;
      }
    }

  os << "  return (success ? 1 : 0);\n";

  os << "}\n\n";



  // Run method
  if ( classDescription->GetNumberOfInputs() == 1 )
    {
    os << "template< class TInputPixel >\n";
    os << "bool " << vtkClassName << "::Run(TInputPixel *, vtkImageData * vtkInput0, vtkImageData * output)\n";
    }
  else
    {
    os << "template< class TInputPixel, class TInputPixel1 >\n";
    os << "bool " << vtkClassName << "::Run(TInputPixel *, TInputPixel1 *, vtkImageData * vtkInput0, vtkImageData * vtkInput1, vtkImageData * output)\n";
    }

  os << "{\n";

  // Instantiate the ITK filter
  os << "  typedef itk::Image< TInputPixel, 3 > InputImageType;\n";

  for (int i = 1; i < classDescription->GetNumberOfInputs(); ++i)
    {
    os << "  typedef InputImageType InputImageType" << i+1 << ";\n";
    }

  if ( this->GetClassDescription()->GetOutputPixelType() != "" )
    {
    // Custom pixel type
    std::string outputPixelType = this->GetClassDescription()->GetOutputPixelType();
    os << "  typedef itk::Image< " << outputPixelType << ", 3 > OutputImageType;\n";
    }
  else
    {
    // Standard pixel type
    os << "  typedef InputImageType OutputImageType;\n\n";
    }

  if ( this->GetClassDescription()->GetFilterType() != "" )
    {
    // Custom filter type declaration from JSON file
    os << "  typedef " << this->GetClassDescription()->GetFilterType() << " FilterType;\n";
    }
  else if ( this->GetClassDescription()->GetTemplateCodeFileName() == "KernelImageFilter" )
    {
    // Kernel filtering class of filters
    os << "  typedef itk::" << itkClassName << "< InputImageType, InputImageType, InputImageType > FilterType;\n";
    }
  else
    {
    // Standard filter type declaration
    os << "  typedef itk::" << itkClassName << "< InputImageType, ";
    for ( int i = 1; i < this->GetNumberOfInputs(); ++i )
      {
      os << "InputImageType" << i+1 << ", ";
      }
    os << " OutputImageType > FilterType;\n";
    }

  os << "  typename FilterType::Pointer filter = FilterType::New();\n\n";

  // Set up the VTK-to-ITK connections
  for (int i = 0; i < classDescription->GetNumberOfInputs(); ++i)
    {
    //os << "  typedef itk::VTKImageToImageFilter< InputImageType" << i+1 << " > VTKToITKConnectorType" << i << ";\n";
    //os << "  typename VTKToITKConnectorType" << i << "::Pointer vtkToITKConnector" << i << " = VTKToITKConnectorType" << i << "::New();\n";
    //os << "  vtkToITKConnector" << i << "->SetInput( vtkInput" << i << " );\n\n";
    os << "  typedef itk::VTKImageToImageFilter< InputImageType";
    if ( i > 0 )
      {
      os << i+1;
      }
    os << " > VTKToITKConnectorType" << i << ";\n";
    os << "  typename VTKToITKConnectorType" << i << "::Pointer vtkToITKConnector" << i << " = VTKToITKConnectorType" << i << "::New();\n";
    os << "  vtkToITKConnector" << i << "->SetInput( vtkInput" << i << " );\n";
    os << "  try\n";
    os << "    {\n";
    os << "    vtkToITKConnector" << i << "->Update();\n";
    os << "    }\n";
    os << "  catch ( itk::ExceptionObject & except )\n";
    os << "    {\n";
    os << "    vtkErrorMacro( << except );\n";
    os << "    return false;\n";
    os << "    }\n\n";
    }

  // Set the input
  this->WriteSetInputCode( os );
  os << "\n";

  // Pass the member variable values from VTK to ITK
  for (int i = 0; i < classDescription->GetNumberOfMemberDescriptions(); ++i)
    {
    const MemberDescription * member = classDescription->GetMemberDescription( i );

    if ( member->GetCustomITKCast() != "<undefined>" &&
         !this->GetClassDescription()->IsEnumerationType( member->GetTypeName() ) )
      {
      std::string code( member->GetCustomITKCast() );
      code = this->SubstituteString( "this->m_", "", code );
      code = this->SubstituteString( "m_", "", code );

      os << "  " << code << "\n";
      }
    else if ( classDescription->IsEnumerationType( member->GetTypeName() ) )
      {
      if ( member->GetCustomITKCast() == "<undefined>" )
        {
        os << "  filter->Set" << member->GetMemberName() << "(static_cast<typename FilterType::"
           << member->GetTypeName() << ">(this->Get"
           << member->GetMemberName() << "()));\n";
        }
      else
        {
        std::string code( member->GetCustomITKCast() );
        code = this->SubstituteString( "this->m_", "", code );
        code = this->SubstituteString( "m_", "", code );
        code = this->SubstituteString( "ImageBoundaryCondition", "itk::ImageBoundaryCondition", code );
        code = this->SubstituteString( "CreateNewBoundaryConditionInstance", "itk::simple::CreateNewBoundaryConditionInstance", code );

        os << "  " << code << "\n";
        }
      }
    else if ( member->GetTypeName().find( "vector" ) != std::string::npos )
      {
      os << "  " << member->GetITKType() << " tmp" << member->GetMemberName() << ";\n";

      for ( int i = 0; i < 3; ++i )
        {
        os << "  tmp" << member->GetMemberName() << "[" << i << "] = this->Get" << member->GetMemberName() << "()[" << i << "];\n";
        }
      os << "  filter->Set" << member->GetMemberName() << "( tmp" << member->GetMemberName()
         << " );\n";
      }
    else
      {
      // Turn bools into ints
      if ( member->GetElementTypeName() == "bool" )
        {
        int numberOfElements = member->GetNumberOfElements();
        if ( numberOfElements == 1 )
          {
          os << "  filter->Set" << member->GetMemberName() << "(this->Get" << member->GetMemberName() << "() != 0);\n\n";
          }
        else
          {
          os << "  int tmp" << member->GetMemberName() << "[" << numberOfElements << "];\n";
          os << "  this->Get" << member->GetMemberName() << "(tmp" << member->GetMemberName() << ");\n\n";

          os << "  bool b" << member->GetMemberName() << "[" << numberOfElements << "];\n";
          for (int i = 0; i < numberOfElements; ++i)
            {
            os << "  b" << member->GetMemberName() << "[" << i << "] = (tmp" << member->GetMemberName() << "[" << i << "] != 0);\n";
            }
          os << "  filter->Set(b" << member->GetMemberName() << ");\n";
          }
        }
      else
        {
        os << "  filter->Set" << member->GetMemberName() << "(this->Get"
           << member->GetMemberName() << "());\n";
        }
      }
    }

  os << "  try\n";
  os << "    {\n";
  os << "    filter->Update();\n";
  os << "    }\n";
  os << "  catch ( itk::ExceptionObject & except )\n";
  os << "    {\n";
  os << "    vtkErrorMacro( << except );\n";
  os << "    return false;\n";
  os << "    }\n\n";

  // Set up the glue between the ITK image and the VTK image.
  os << "  typedef itk::ImageToVTKImageFilter< OutputImageType > ITKToVTKConnectorType;\n";
  os << "  typename ITKToVTKConnectorType::Pointer itkToVTKConnector = ITKToVTKConnectorType::New();\n";
  os << "  itkToVTKConnector->SetInput( filter->GetOutput() );\n";
  os << "  itkToVTKConnector->Update();\n\n";

  os << "  output->DeepCopy( itkToVTKConnector->GetOutput() );\n\n";

  // End the Run method
  os << "  return true;\n";
  os << "}\n\n";

  // PrintSelf method
  os << "void " << vtkClassName << "::PrintSelf(ostream& os, vtkIndent indent)\n";
  os << "{\n";
  os << "  this->Superclass::PrintSelf(os, indent);\n\n";

  for (int i = 0; i < classDescription->GetNumberOfMemberDescriptions(); ++i)
    {
    const MemberDescription * member = classDescription->GetMemberDescription( i );
    os << "  os << indent << " << "\"" << member->GetMemberName() << ":\" << this->"
       << member->GetMemberName() << " << \"\\n\";\n";
    }

  os << "}\n";

  os.flush();
  os.close();

  return true;
}

std::string
ParaView3PluginGenerator
::GetITKClassName()
{
  std::string filterName( this->GetClassDescription()->GetPluginName() );

  // Override the filter name if it is explicit in the JSON file
  if ( this->GetClassDescription()->GetITKClassName() != "" )
    {
    filterName = this->GetClassDescription()->GetITKClassName();
    }

  return filterName;
}


std::string
ParaView3PluginGenerator
::GetVTKClassName()
{
  const ClassDescription * classDescription = this->GetClassDescription();
  std::string vtkClassName( "vtkITK" );
  vtkClassName.append( classDescription->GetPluginName() );

  return vtkClassName;
}

std::string
ParaView3PluginGenerator
::GetVTKTypeName( const std::string & typeName )
{
  if ( this->GetClassDescription()->IsEnumerationType( typeName ) )
    {
    return typeName;
    }
  else if ( this->IsBoolVectorType( typeName ) )
    {
    return std::string( "int" );
    }
  else if ( this->IsIntVectorType( typeName ) )
    {
    return std::string( "int" );
    }
  else if ( this->IsDoubleVectorType( typeName ) )
    {
    return std::string( "double" );
    }

  std::cerr << "Unknown input typeName in GetVTKTypeName" << std::endl;
  return std::string();
}

void
ParaView3PluginGenerator
::WriteSetInputCode( std::ostream & os)
{
  for ( int i = 0; i < this->GetClassDescription()->GetNumberOfInputs(); ++i )
    {
    os << "  InputImageType";
    if ( i > 0 )
      {
      os << i+1;
      }
    os<< " * image" << i+1 << " = vtkToITKConnector" << i << "->GetOutput();\n";
    }

  if ( this->GetClassDescription()->GetCustomSetInput() == "<undefined>" )
    {
    if ( this->GetClassDescription()->GetNumberOfInputs() == 1 )
      {
      os << "  filter->SetInput( image1 );\n";
      }
    else
      {
      // Assume at most two inputs
      os << "  filter->SetInput1( image1 );\n";
      os << "  filter->SetInput2( image2 );\n";
      }
    }
  else
    {
    // Custom input code from the JSON file

    std::string code( this->GetClassDescription()->GetCustomSetInput() );
    code = this->SubstituteString( "this->m_", "plugins", code );
    code = this->SubstituteString( "m_", "plugins", code );
    os << "  " << code << "\n";
    }
}
