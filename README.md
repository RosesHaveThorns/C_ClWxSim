# Climate Weather Sim
A very simplistic weather and climate simulator for worldbuilding. Currently in the process of converting the project from python (ClWxSim-py) to C (C_ClWxSim).

Note, conversion is still incomplete at this time, although the software is now runnable.

## Features
### Current Features
- Basic Win32API ui
- Graph showing air pressure and wind velocity
- Threaded simulation calculations

### In-progress Features
- Pressure Advection and Diffusion

### Planned Features
- Wind Simulation
- Coriolis Effect
- Pressure Gradient Force Simulation
- Temperature Calculation, using ideal gas laws and the Pressure Simulation
- Humidity Simulation, including Precipitation and Cloud tracking
- Advanced UI with front tracking and views for humidity, temperatures, pressure, wind etc
- Ability to input any Planet map into the simulation
- Simulation state storage and continuation

## Credit
Code by Rose Awen Brindle.

Pressure advection and diffusion based on code from Jos Stam: \
`Jos Stam (2001), 'A Simple Fluid Solver based on the FFT'`

Uses Win32Api and FFTW3.
