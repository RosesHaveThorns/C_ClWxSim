#ifndef H_PRESSURE
#define H_PRESSURE

#include "world.h"
#include <fftw3.h>
#include "maths_utils.h"
#include <math.h>

#define VISCOSITY 1

void tickPressure(World *wld, float dt);

#endif