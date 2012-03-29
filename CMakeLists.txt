project( ITKPlugins )

cmake_minimum_required( VERSION 2.8 )

# Choose the target applications for which the plugins should be built
option( BUILD_SLICER4_PLUGINS "Build plugins for Slicer 4.0" OFF )
option( BUILD_PARAVIEW3_PLUGINS "Build plugins for ParaView 3.14" OFF )

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ITKPlugins_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ITKPlugins_BINARY_DIR}/bin )

#######################################
# Quiet warnings from the JSON library.
#######################################
if ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  add_definitions( -DJSON_DEBUG )
endif()
if ( CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
  add_definitions( -DNDEBUG )
endif()

add_subdirectory( Common )
add_subdirectory( Generators )
add_subdirectory( Parser )
add_subdirectory( Utilities )

# Generate plugin code
# Code: #+ - Tested, compiles, #- Tested, fails

set( Plugin_JSON
  # These compile
  #Abs
  #AbsoluteValueDifference
  #Acos
  #Add
  AddConstantTo
  #And
  #ApproximateSignedDistanceMap
  #Asin
  #Atan
  #Atan2
  #Bilateral
  #BinaryContour
  #BinaryMagnitude
  #BinaryMinMaxCurvatureFlow
  #BinaryProjection
  #BinaryThinning
  #BinaryThreshold
  #BinomialBlur
  #BoundedReciprocal
  #CannyEdgeDetection
  #CheckerBoard
  #ComplexToImaginary
  #ComplexToModulus
  #ComplexToPhase
  #ComplexToReal
  #ConnectedComponent
  #ConnectedThreshold
  #Convolution
  ConstantPad
  #Cos
  #Crop
  CurvatureAnisotropicDiffusion
  CurvatureFlow
  #DanielssonDistanceMap
  #Derivative
  #DiscreteGaussian
  #Divide
  #DivideByConstant
  #DivideConstantBy
  #DoubleThreshold
  #Exp
  #ExpNegative
  FFTConvolution
  #FFTShift
  Flip
  #GradientAnisotropicDiffusion
  #GradientMagnitudeRecursiveGaussian
  #HausdorffDistance
  #HConcave
  #HConvex
  ##-HistogramMatching
  #HMaxima
  #HMinima
  #HuangThreshold
  #IntensityWindowing
  #IntermodesThreshold
  InverseDeconvolution
  #InverseFFT
  #InvertIntensity
  #IsoContourDistance
  #IsoDataThreshold
  #IsolatedWatershed
  #KittlerIllingworthThreshold
  #LabelContour
  #LandweberDeconvolution
  #Laplacian
  #LaplacianRecursiveGaussian
  #LaplacianSharpening
  #LiThreshold
  #Log
  #Log10
  #MagnitudeAndPhaseToComplex
  #Mask
  #MaskNegated
  #Maximum
  #MaximumEntropyThreshold
  #MaximumProjection
  #Mean
  #MeanProjection
  #MedianProjection
  #Median
  #MinMaxCurvatureFlow
  #Minimum
  #MinimumProjection
  #MirrorPad
  #Modulus
  #MomentsThreshold
  #Multiply
  #MultiplyByConstant
  #NeighborhoodConnected
  #Noise
  #Normalize
  #NormalizeToConstant
  #Not
  #Or
  #OtsuThreshold
  #Pow
  #PowToConstant
  #ProjectedLandweberDeconvolution
  #RealAndImaginaryToComplex
  #RelabelComponent
  #RenyiEntropyThreshold
  #RescaleIntensity
  #RichardsonLucyDeconvolution
  #ScalarConnectedComponent
  #ShanbhagThreshold
  #ShiftScale
  #Shrink
  #Sigmoid
  #SignedDanielssonDistanceMap
  #SignedMaurerDistanceMap
  #SimpleContourExtractor
  #Sin
  #SobelEdgeDetection
  #Sqrt
  #Square
  #SquaredDifference
  #StandardDeviationProjection
  #Subtract
  #SubtractConstantBy
  #SumProjection
  #TikhonovDeconvolution
  #Tan
  #Threshold
  #TriangleThreshold
  #VotingBinary
  #VotingBinaryHoleFilling
  #VotingBinaryIterativeHoleFilling
  #WienerDeconvolution
  #WrapPad
  #Xor
  #YenThreshold
  #ZeroCrossingBasedEdgeDetection

  # These don't compile yet
  #-BinaryClosingByReconstruction
  #-BinaryDilate
  #-BinaryErode
  #-BinaryFillhole
  #-BinaryGrindPeak
  #-BinaryMedian
  #-BinaryMorphologicalClosing
  #-BinaryMorphologicalOpening
  #-BinaryOpeningByReconstruction
  #-BlackTopHat
  #-BoxMean
  #-BoxSigma
  #-ClosingByReconstruction - kernel image filter type
  #-DilateObjectMorphology
  #-EdgePotential - vector pixels
  #-ErodeObjectMorphology
  #-Extract - names
  #-FastMarching
  #-ForwardFFT
  #-GeodesicActiveContourLevelSet
  #-GrayscaleDilate
  #-GrayscaleErode
  #-GrayscaleFillhole
  #-GrayscaleGeodesicDilate
  #-GrayscaleGeodesicErode
  #-GrayscaleGrindPeak
  #-GrayscaleMorphologicalClosing
  #-GrayscaleMorphologicalOpening
  #-LabelMapMask
  #-LabelMapOverlay
  #-LabelMapToBinary
  #-LabelMapToLabel
  #-LabelMapToRGB
  #-LabelOverlay
  #-LabelToRGB
  #-MorphologicalGradient - kernel image
  #-N4BiasFieldCorrection
  #-OpeningByReconstruction
  #-Paste
  #-PermuteAxes - CLP generation fails
  #-RecursiveGaussian
  #-RegionOfInterest
  #-Resample - malformed XML
  #-ScalarToRGBColormap
  #-ShapeDetectionLevelSet - default pixel type is float, JSON doesn't change it
  #-SmoothingRecursiveGaussian
  #-ThresholdMaximumConnectedComponents
  #-VectorConnectedComponent
  #-VectorIndexSelectionCast
  #-VectorMagnitude
  #-WhiteTopHat - kernel filter
  #-ZeroCrossing - pixel type list
)

if ( BUILD_SLICER4_PLUGINS )

  set( EXTENSION_NAME ITKPluginsExtension )
  find_package( Slicer REQUIRED )
  include( ${Slicer_USE_FILE} )

endif()

if ( BUILD_PARAVIEW3_PLUGINS )

  find_package( ParaView REQUIRED )
  include( ${PARAVIEW_USE_FILE} )

  # Need to include ITK separately because ParaView doesn't know anything about it
  find_package( ITK REQUIRED )
  include( ${ITK_USE_FILE} )

endif()

add_subdirectory( GeneratedCode )
