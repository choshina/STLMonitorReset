# CAV 2023:

## System requirement

- Operating system: Linux or MacOS;

- Matlab (Simulink/Stateflow) version

- Breach

## Installation of [Breach](https://github.com/decyphir/breach)

 1. start matlab, set up a C/C++ compiler using the command `mex -setup`. (Refer to [here](https://www.mathworks.com/help/matlab/matlab_external/changing-default-compiler.html) for more details.)
  
 2. navigate to home in Matlab commandline, and run `InstallBreach`
  > Note that we customized `InstallBreach`. It only compiles the online monitoring component. For the full functionality of Breach, please refer to [the original repository](https://github.com/decyphir/breach)

## Instructions on reproduction of experiment results
 
 The online monitoring feature is in the folder `Online/`. Scripts for AFC are in `Online/examples/AFC_online/plot_examples.m`; scripts for AT are in `Online/examples/Autotrans_online/init_monitor`
 
### Configuration of mode

- the variable `d`:
  - `d = 0`: Mode 1--monitor only
  - `d = 1`: Mode 2--monitor with online diagnostics
  - `d = 2`: Mode 3--monitor with reset

- For AFC: Line 23
- For AT: Line 9

### Input data

Select one input signal by uncommenting out one line, and commenting out the other lines.

- For AFC: Line 12-21
- For AT: Line 14-23

## Extended experimental results

In `Online/experiment/`

![experiment in paper](/Online/experiment/effect.png)

![another example](/Online/experiment/case2.png)
