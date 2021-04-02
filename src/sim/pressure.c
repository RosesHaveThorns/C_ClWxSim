#include "pressure.h"

// Calculates the advection and diffusion of the pressure array over a single tick
// Based on code from 'Jos Stam (2001), A Simple Fluid Solver based on the FFT'
// That code was updated to use FFTW3 (yes that was as painful as you think it was. no i have no clue how it actually works)

void stable_solve(int, int, float[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE], float[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE],
                  float[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE], float[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE], float, float);

static fftw_plan plan_vel_y_rc, plan_vel_y_cr, plan_vel_x_rc, plan_vel_x_cr;   // a plan for real to complex and a plan for complex to real, for both the wind x and y velocities

void init_FFT(World *wld){
    plan_vel_y_rc = fftw_plan_dft_2d(wld->width, wld->height, (fftw_complex *) wld->wind_vel_y, (fftw_complex *)wld->wind_vel_y, FFTW_FORWARD, FFTW_ESTIMATE);
    plan_vel_y_cr = fftw_plan_dft_2d(wld->width, wld->height, (fftw_complex *) wld->wind_vel_y, (fftw_complex *)wld->wind_vel_y, FFTW_BACKWARD, FFTW_ESTIMATE);

    plan_vel_x_rc = fftw_plan_dft_2d(wld->width, wld->height, (fftw_complex *) wld->wind_vel_x, (fftw_complex *)wld->wind_vel_x, FFTW_FORWARD, FFTW_ESTIMATE);
    plan_vel_x_cr = fftw_plan_dft_2d(wld->width, wld->height, (fftw_complex *) wld->wind_vel_x, (fftw_complex *)wld->wind_vel_x, FFTW_BACKWARD, FFTW_ESTIMATE);
}

void destroy_FFT_plans() {
    fftw_destroy_plan(plan_vel_y_rc);
    fftw_destroy_plan(plan_vel_y_cr);
    fftw_destroy_plan(plan_vel_x_rc);
    fftw_destroy_plan(plan_vel_x_cr);
}

void tickPressure(World *wld, float dt) {
    init_FFT(wld);

    stable_solve(wld->width, wld->width, wld->wind_vel_x, wld->wind_vel_y, wld->old_wind_vel_x, wld->old_wind_vel_y, VISCOSITY, dt);

    destroy_FFT_plans();
}


void stable_solve(int n1, int n2, float u[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE],
                  float v[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE],
                  float u0[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE], float v0[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE],
                  float visc, float dt) {

    float x, y, x0, y0, f, r, U[2], V[2], s, t;
    int i, j, i0, j0, i1, j1;

    for (i = 0; i < n1; i++) {
        for (j = 0; j < n2; j++) {
            u[i][j] += dt * u0[i][j];
            u0[i][j] = u[i][j];

            v[i][j] += dt * v0[i][j];
            v0[i][j] = v[i][j];
        }
    }

    for (x = 0.5 / n1, i = 0; i < n1; i++, x += 1.0 / n1) {
        for (y = 0.5 / n2, j = 0; j < n2; j++, y += 1.0 / n2) {
            x0 = n1 * (x - dt * u0[i][j]) - 0.5;
            y0 = n2 * (y - dt * v0[i][j]) - 0.5;

            i0 = floor_custom(x0);
            s = x0 - i0;
            i0 = (n1 + (i0 % n1)) % n1;
            i1 = (i0 + 1) % n2;

            j0 = floor_custom(y0);
            t = y0 - j0;
            j0 = (n1 + (j0 % n1)) % n1;
            j1 = (j0 + 1) % n2;


            u[i][j] = (1 - s) * ((1 - t) * u0[i0][j0] + t * u0[i0][j1]) +
                           s * ((1 - t) * u0[i1][j0] + t * u0[i1][j1]);
            v[i][j] = (1 - s) * ((1 - t) * v0[i0][j0] + t * v0[i0][j1]) +
                           s * ((1 - t) * v0[i1][j0] + t * v0[i1][j1]);
        }
    }

    for (i = 0; i < n1; i++) {
        for (j = 0; j < n2; j++) {
            u0[i][j + 1] = u[i][j];
            v0[i][j + 1] = v[i][j];
        }
    }

    fftw_execute(plan_vel_x_rc);
    fftw_execute(plan_vel_y_rc);

    for (i = 0; i <= n1; i += 2) {
        x = 0.5 * i;

        for (j = 0; j < n2; j++) {
            y = j <= n2 / 2 ? j : j - n2;

            r = x * x + y * y;
            if (r == 0.0) continue;

            f = expf(-r * dt * visc);

            U[0] = u0[i][j + 1];
            V[0] = v0[i][j + 1];
            U[1] = u0[i + 1][j + 1];
            V[1] = v0[i + 1][j + 1];

            u0[i][j + 1]        =   f * ((1 - x * x / r) * U[0] - x * y / r * V[0]);
            u0[i + 1][j + 1]    =   f * ((1 - x * x / r) * U[1] - x * y / r * V[1]);
            v0[i][j + 1]        =   f * (-y * x / r * U[0] + (1 - y * y / r) * V[0]);
            v0[i + 1][j + 1]    =   f * (-y * x / r * U[1] + (1 - y * y / r) * V[1]);
        }
    }

    fftw_execute(plan_vel_x_cr);
    fftw_execute(plan_vel_y_cr);

    f = 1.0 / (n1 * n2);
    for (i = 0; i < n1; i++) {
        for (j = 0; j < n2; j++) {
            u[i][j] = f * u0[i][j + 1];
            v[i][j] = f * v0[i][j + 1];
        }
    }
}