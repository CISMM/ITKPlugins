vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
vtkImageData *input = vtkImageData::SafeDownCast
  (inInfo->Get(vtkDataObject::DATA_OBJECT()));
if (!input)
  {
  vtkErrorMacro("Input is not of type vtkImageData");
  return 0;
  }

vtkInformation *outInfo = outputVector->GetInformationObject(0);
vtkImageData *output = vtkImageData::SafeDownCast
  (outInfo->Get(vtkDataObject::DATA_OBJECT()));
if (!output)
  {
  vtkErrorMacro("Output is not of type vtkImageData");
  return 0;
  }

#define vtkReducedTypeTemplateMacro(call)                                   \
  vtkTemplateMacroCase(VTK_DOUBLE, double, call);                           \
  vtkTemplateMacroCase(VTK_FLOAT, float, call);                             \
  //vtkTemplateMacroCase_ll(VTK_LONG_LONG, long long, call)             \
  //vtkTemplateMacroCase_ll(VTK_UNSIGNED_LONG_LONG, unsigned long long, call) \
  vtkTemplateMacroCase_si64(VTK___INT64, __int64, call)                     \
  vtkTemplateMacroCase_ui64(VTK_UNSIGNED___INT64, unsigned __int64, call)   \
  //vtkTemplateMacroCase(VTK_ID_TYPE, vtkIdType, call);                 \
  vtkTemplateMacroCase(VTK_LONG, long, call);                               \
  vtkTemplateMacroCase(VTK_UNSIGNED_LONG, unsigned long, call);             \
  vtkTemplateMacroCase(VTK_INT, int, call);                                 \
  vtkTemplateMacroCase(VTK_UNSIGNED_INT, unsigned int, call);               \
  vtkTemplateMacroCase(VTK_SHORT, short, call);                             \
  vtkTemplateMacroCase(VTK_UNSIGNED_SHORT, unsigned short, call);           \
  vtkTemplateMacroCase(VTK_CHAR, char, call);                               \
  vtkTemplateMacroCase(VTK_SIGNED_CHAR, signed char, call);                 \
  vtkTemplateMacroCase(VTK_UNSIGNED_CHAR, unsigned char, call)

switch ( input->GetScalarType() )
  {
  vtkReducedTypeTemplateMacro( Run( static_cast< VTK_TT *>( 0 ), input, output ); );

  default:
    vtkErrorMacro( "Unknown pixel type encountered" );
    break;
  }
