  // Switch on the second pixel type
  switch ( vtkInput1->GetScalarType() )
    {
    vtkReducedTypeTemplateMacro( Run( static_cast< TInputPixel0 * >( 0 ),
                                      static_cast< VTK_TT *>( 0 ),
                                      vtkInput0, vtkInput1, output ); );

    default:
      vtkErrorMacro( "Unknown pixel type encountered in input1" );
      break;
    }
