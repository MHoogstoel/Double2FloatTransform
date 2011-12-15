cmake_minimum_required(VERSION 2.6)

project( Double2FloatTransform )

FIND_PACKAGE ( ITK )
IF ( ITK_FOUND )
       INCLUDE( ${USE_ITK_FILE} )
ENDIF( ITK_FOUND )

add_executable( Double2FloatTransform Double2FloatTransform.cpp )

TARGET_LINK_LIBRARIES( Double2FloatTransform
  ITKIO ITKBasicFilters ITKNumerics ITKCommon
  )
