#include "controller.h"

int tick_counter = 0;

int sim_tick(World *wld) {
    tick_counter++;
    tickPressure(wld, 1);

    return tick_counter;
}