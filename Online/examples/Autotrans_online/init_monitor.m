mdl = 'Autotrans_online';

% Parameters
max_rob = 500;              % initial robustness is between -max_rob, +max_rob
rob_up_lim = -max_rob;  % stops when rob_up is below 
rob_low_lim = max_rob; 
rob_diff_lim = -inf;

d = 0;
times = 10;


%Throttle: [0, 100] Brake: [0,325]
%input = [37.6272   19.0924   42.8253   48.2022   39.1988  191.5899   73.5110  125.0012];
%input = [58.2986   25.1806   29.0441   61.7091   86.2163  267.9223  319.3656  237.3309];%
%input = [61.0959   77.8802   42.3453    9.0823   86.6032   49.9384   91.3267  143.0277];%
%input = [67.6122   28.9065   67.1808   69.5140   22.0976   82.8068   72.8130  217.0456];
%input = [84.4392   34.4462   78.0520   67.5332    2.1825  195.7054  125.7006  297.6972];
input = [98.9872   51.4423   88.4281   58.8026   50.2945   64.9554  132.2603  243.3294];
%input = [94.5174   20.8935   70.9282   23.6231   38.8038  197.3738  146.2948  149.0858];
%input = [66.1945   77.0286   35.0218   66.2010  135.2515  273.6270  270.6980   83.3433];
%input = [99.3705   21.8677   10.5798   10.9697   20.6672  131.4885  145.7212  118.8903];
%input = [94.9304   32.7565   67.1264   43.8645  270.8877  249.8776   54.3574  280.1437];

phi_autotrans = 'alw_[0,30](not(speed[t] < 50) or ev_[1,3](gear[t] > 3))';
%phi_autotrans = 'alw_[0,30](not(speed[t] <10) or ev_[0,3](speed[t] > 50))';

AT = BreachSimulinkSystem(mdl);

input_gen.type = 'UniStep';
input_gen.cp = 4;
AT.SetInputGen(input_gen);

AT.SetParam({'Throttle_u0', 'Throttle_u1', 'Throttle_u2', 'Throttle_u3', 'Brake_u0', 'Brake_u1', 'Brake_u2', 'Brake_u3'}, input);
AT.SetParam({'max_rob', 'diagnoser'}, [max_rob, d]);


tic
for i = 1:times
    AT.ResetSimulations();
    AT.Sim(0:.01:30);
end

simTime = toc;
one_sim_time = simTime/times

%AT.PlotSignals();

%AT.CheckSpec(phi_autotrans)



