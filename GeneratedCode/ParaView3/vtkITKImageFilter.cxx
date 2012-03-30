/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkITKImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef __vtkITKImageFilter_cxx
#define __vtkITKImageFilter_cxx

#include "vtkITKImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

vtkStandardNewMacro(vtkITKImageFilter);

//----------------------------------------------------------------------------
vtkITKImageFilter::vtkITKImageFilter()
{
  this->VTKCasters = NULL;
  this->VTKExporters = NULL;
  this->ITKImporters = NULL;
  this->ITKExporter = ITKImageExportType::New();
  this->VTKImporter = vtkImageImport::New();
}

//----------------------------------------------------------------------------
vtkITKImageFilter::~vtkITKImageFilter()
{
  int numPorts = this->GetNumberOfInputPorts();
  if (this->VTKCasters)
    {
    for (int i = 0; i < numPorts; i++)
      {
      if ( this->VTKCasters[i] )
        {
        this->VTKCasters[i]->Delete();
        }
      }
    }

  if (this->VTKExporters)
    {
    for (int i = 0; i < numPorts; i++)
      {
      if ( this->VTKExporters[i] )
        {
        this->VTKExporters[i]->Delete();
        }
      }
    }

  if (this->VTKImporter)
    {
    this->VTKImporter->Delete();
    }
}

//----------------------------------------------------------------------------
void vtkITKImageFilter::InitializeITKImporters()
{
  // Allocate the caster and VTK exporter filters
  int numPorts = this->GetNumberOfInputPorts();
  this->VTKCasters = new vtkImageCast*[numPorts];
  this->VTKExporters = new vtkImageExport*[numPorts];
  this->ITKImporters = new ITKImageImportType::Pointer[numPorts];
  for (int i = 0; i < numPorts; i++)
    {
    this->VTKCasters[i]  = vtkImageCast::New();
    this->VTKCasters[i]->SetOutputScalarTypeToFloat();
    this->VTKExporters[i] = vtkImageExport::New();
    this->ITKImporters[i] = ITKImageImportType::New();

    // Connect the vtkImageCast that converts the input to the
    // floating-point type we want to pass to ITK filters.
    this->VTKExporters[i]->SetInputConnection(this->VTKCasters[i]->GetOutputPort());

    // This call takes the place of the usual SetInput() method.
    this->ITKImporters[i]->SetCallbackUserData(this->VTKExporters[i]);

    // Set the rest of the callbacks
    this->ITKImporters[i]->SetBufferPointerCallback(this->VTKExporters[i]->GetBufferPointerCallback());
    this->ITKImporters[i]->SetDataExtentCallback(this->VTKExporters[i]->GetDataExtentCallback());
    this->ITKImporters[i]->SetOriginCallback(this->VTKExporters[i]->GetOriginCallback());
    this->ITKImporters[i]->SetSpacingCallback(this->VTKExporters[i]->GetSpacingCallback());
    this->ITKImporters[i]->SetNumberOfComponentsCallback(this->VTKExporters[i]->GetNumberOfComponentsCallback());
    this->ITKImporters[i]->SetPipelineModifiedCallback(this->VTKExporters[i]->GetPipelineModifiedCallback());
    this->ITKImporters[i]->SetPropagateUpdateExtentCallback(this->VTKExporters[i]->GetPropagateUpdateExtentCallback());
    this->ITKImporters[i]->SetScalarTypeCallback(this->VTKExporters[i]->GetScalarTypeCallback());
    this->ITKImporters[i]->SetUpdateDataCallback(this->VTKExporters[i]->GetUpdateDataCallback());
    this->ITKImporters[i]->SetUpdateInformationCallback(this->VTKExporters[i]->GetUpdateInformationCallback());
    this->ITKImporters[i]->SetWholeExtentCallback(this->VTKExporters[i]->GetWholeExtentCallback());
    }
}

//----------------------------------------------------------------------------
void vtkITKImageFilter::InitializeVTKImporters()
{
  // This call takes the place of the usual SetInput() method.
  this->VTKImporter->SetCallbackUserData(ITKExporter->GetCallbackUserData());

  // Set the rest of the callbacks
  this->VTKImporter->SetUpdateInformationCallback(ITKExporter->GetUpdateInformationCallback());
  this->VTKImporter->SetPipelineModifiedCallback(ITKExporter->GetPipelineModifiedCallback());
  this->VTKImporter->SetWholeExtentCallback(ITKExporter->GetWholeExtentCallback());
  this->VTKImporter->SetSpacingCallback(ITKExporter->GetSpacingCallback());
  this->VTKImporter->SetOriginCallback(ITKExporter->GetOriginCallback());
  this->VTKImporter->SetScalarTypeCallback(ITKExporter->GetScalarTypeCallback());
  this->VTKImporter->SetNumberOfComponentsCallback(ITKExporter->GetNumberOfComponentsCallback());
  this->VTKImporter->SetPropagateUpdateExtentCallback(ITKExporter->GetPropagateUpdateExtentCallback());
  this->VTKImporter->SetUpdateDataCallback(ITKExporter->GetUpdateDataCallback());
  this->VTKImporter->SetDataExtentCallback(ITKExporter->GetDataExtentCallback());
  this->VTKImporter->SetBufferPointerCallback(ITKExporter->GetBufferPointerCallback());
}

//----------------------------------------------------------------------------
void vtkITKImageFilter::Init()
{
  this->InitializeITKImporters();
  this->InitializeVTKImporters();
}

//----------------------------------------------------------------------------
int vtkITKImageFilter::BeforeUpdateInternalFilters(vtkInformationVector** inputVector)
{
  for (int i = 0; i < this->GetNumberOfInputPorts(); i++)
    {
    // Check that the inputs are of type vtkImageData
    vtkInformation *inInfo = inputVector[i]->GetInformationObject(0);
    vtkImageData *input = vtkImageData::SafeDownCast
      (inInfo->Get(vtkDataObject::DATA_OBJECT()));
    if(!input)
      {
      vtkErrorMacro("An input is not of type vtkImageData");
      return 0;
      }

    this->VTKCasters[i]->SetInputData(input);
    this->VTKCasters[i]->Update();
    }

  return 1;
}

//----------------------------------------------------------------------------
int vtkITKImageFilter::AfterUpdateInternalFilters(vtkInformationVector* outputVector)
{
  // Now connect the ITK pipeline output to the VTK output
  this->ITKExporter->Update();

  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkImageData *output = vtkImageData::SafeDownCast
    (outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!output)
    {
    vtkErrorMacro("Output is not of type vtkImageData");
    return 0;
    }

  this->VTKImporter->Update();
  this->VTKImporter->Update();

  output->ShallowCopy(this->VTKImporter->GetOutput());

  return 1;
}

//----------------------------------------------------------------------------
int vtkITKImageFilter::RequestData(vtkInformation *request,
                                   vtkInformationVector **inputVector,
                                   vtkInformationVector *outputVector)
{
  try
    {
    if (!this->BeforeUpdateInternalFilters(inputVector))
      {
      return 0;
      }

    this->UpdateInternalFilters();

    if (!this->AfterUpdateInternalFilters(outputVector))
      {
      return 0;
      }
    }
  catch(itk::ExceptionObject& error)
    {
	vtkErrorMacro(<< error.GetDescription());
    return 0;
    }

  return 1;
}

//----------------------------------------------------------------------------
void vtkITKImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

#endif // __vtkITKImageFilter_cxx
