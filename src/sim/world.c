#include "world.h"

#include <stdio.h>
#include "array_utils.h"

// setup graph struct to initial values
void wld_init(World *wld, int width, int height, float init_pressure, float ang_vel, float cell_size) {
  wld->width = width;
  wld->height = height;

  wld->angular_vel = ang_vel;
  wld->cell_size = cell_size;

  set_all(wld->pressure, init_pressure);
  set_all(wld->old_pressure, init_pressure);

  zeros(wld->wind_vel_x);
  zeros(wld->wind_vel_y);

  zeros(wld->air_temp);
  zeros(wld->precip);
  zeros(wld->humidity);

  zeros(wld->ground_temp);
  zeros(wld->ground_height);
  zeros(wld->ground_water);
}
