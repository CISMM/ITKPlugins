vtkInformation *inInfo0 = inputVector[0]->GetInformationObject(0);
vtkImageData *input0 = vtkImageData::SafeDownCast
  (inInfo0->Get(vtkDataObject::DATA_OBJECT()));
if (!input0)
  {
  vtkErrorMacro("Input 0 is not of type vtkImageData");
  return 0;
  }

vtkInformation *inInfo1 = inputVector[1]->GetInformationObject(0);
vtkImageData *input1 = vtkImageData::SafeDownCast
  (inInfo1->Get(vtkDataObject::DATA_OBJECT()));
if (!input1)
  {
  vtkErrorMacro("Input 1 is not of type vtkImageData");
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

switch ( input0->GetScalarType() )
  {
  vtkReducedTypeTemplateMacro( DispatchTwoInputs( static_cast< VTK_TT *>( 0 ), input0, input1, output ) );

  default:
    vtkErrorMacro( "Unknown pixel type encountered in input0" );
    break;
  }
