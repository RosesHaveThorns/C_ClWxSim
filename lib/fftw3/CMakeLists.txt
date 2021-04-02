message("-- Linking FFTW")

add_library(FFTW3 fftw3.h)

set_target_properties(FFTW3 PROPERTIES LINKER_LANGUAGE C)
