# Online Reset for STL Monitoring

Online monitoring is a popular validation approach in which the temporal behavior of a system is checked to assess whether it satisfies a given specification expressed, e.g., in Signal Temporal Logic (STL). This is done by employing a monitor that, at each time point, states the specification validity: satisfied, violated, or unknown. In some settings, monitoring should continue even after a violation episode is detected, to detect possible future violation episodes. However, for a monitor just relying on STL semantics, this is not possible, as, once the specification is violated by an input signal, any continuation of the signal still violates the specification. To tackle this problem, we here propose an optimal reset technique that, at runtime, detects the end of a violation episode and shifts the evaluation of the monitor to skip such episode. In this way, the monitoring can continue to detect possible other future violation episodes. We propose a framework that integrates the reset technique with an existing monitoring approach. Experiments on two Simulink models show that the technique can effectively reset the monitor and report all the violation episodes, with a negligible overhead on the monitoring cost.

## System requirement

- Operating system: Linux or MacOS;

- Matlab (Simulink/Stateflow) version

- Breach

## Installation of [Breach](https://github.com/decyphir/breach)

 1. start matlab, set up a C/C++ compiler using the command `mex -setup`. (Refer to [here](https://www.mathworks.com/help/matlab/matlab_external/changing-default-compiler.html) for more details.)
  
 2. navigate to home in Matlab commandline, and run `InstallBreach`

## Instructions on reproduction of experiment results
 
 The online monitoring feature is in the folder `Online/`. Scripts for AFC are in `Online/examples/AFC_online/plot_examples.m`; scripts for AT are in `Online/examples/Autotrans_online/init_monitor`
 
### Configuration of mode

- the variable `d`:
  - `d = 0`: Mode 1--monitor only
  - `d = 1`: Mode 2--monitor with online diagnostics
  - `d = 2`: Mode 3--monitor with reset

- For AFC: Line 23
- For AT: Line 9

### input data

- For AFC: Line 12-21
- For AT: Line 14-23

## Extended experimental results

![experiment in paper!](/Online/experiment/effect.png)
