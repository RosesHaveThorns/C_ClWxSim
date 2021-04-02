#ifndef H_WORLD
#define H_WORLD

#include "config.h"
#include "array_utils.h"


struct world {
    int width;  // amount of cells wide
    int height; // amount of cells high

    float cell_size; // size and width of a cell, measured in km a

    float angular_vel; // angular velocity of planet, measured in rad/s

    float pressure[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE];       // air pressure at sea level, measured in mbar
    float old_pressure[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE];   // copy of pressure data from last tick

    float wind_vel_x[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE];     // latitudinal wind speed, measured in m/s
    float old_wind_vel_x[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE]; // copy of latitudinal wind speed data from last tick
    float wind_vel_y[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE];     // longitudinal wind speed, measured in m/s
    float old_wind_vel_y[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE]; // copy of longitudinal wind speed data from last tick

    float air_temp[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE];       // measured in kelvin
    float precip[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE];
    float humidity[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE];

    float ground_temp[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE];    // temperature of ground, measured in kelvin
    float ground_height[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE];  // height of land in cell, measured in metres
    float ground_water[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE];   // amount of water in cell, ratio
};

typedef struct world World;

void wld_init(World *wld, int, int, float, float, float);

#endif