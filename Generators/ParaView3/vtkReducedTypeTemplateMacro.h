#define vtkReducedTypeTemplateMacro(call)                                   \
  vtkTemplateMacroCase(VTK_DOUBLE, double, call);                           \
  vtkTemplateMacroCase(VTK_FLOAT, float, call);                       \
  vtkTemplateMacroCase_si64(VTK___INT64, __int64, call)               \
  vtkTemplateMacroCase_ui64(VTK_UNSIGNED___INT64, unsigned __int64, call) \
  vtkTemplateMacroCase(VTK_LONG, long, call);                         \
  vtkTemplateMacroCase(VTK_UNSIGNED_LONG, unsigned long, call);       \
  vtkTemplateMacroCase(VTK_INT, int, call);                           \
  vtkTemplateMacroCase(VTK_UNSIGNED_INT, unsigned int, call);         \
  vtkTemplateMacroCase(VTK_SHORT, short, call);                       \
  vtkTemplateMacroCase(VTK_UNSIGNED_SHORT, unsigned short, call);     \
  vtkTemplateMacroCase(VTK_CHAR, char, call);                         \
  vtkTemplateMacroCase(VTK_SIGNED_CHAR, signed char, call);           \
  vtkTemplateMacroCase(VTK_UNSIGNED_CHAR, unsigned char, call)
