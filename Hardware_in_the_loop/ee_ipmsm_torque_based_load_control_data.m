%% Parameters for IPMSM Torque-Based Load Control Example

% This example shows how to control the torque in an interior permanent
% magnet synchronous motor (IPMSM) based drive. A high-voltage battery
% feeds the IPMSM through a controlled three-phase inverter. A ramp of
% torque request is provided to the motor controller. The load torque is
% quadratically dependent on the rotor speed. The Control subsystem uses 
% an open-loop approach to control the IPMSM torque and a closed-loop 
% approach to control the current. At each sample instant, the torque 
% request is converted to relevant current references. The current control 
% is PI-based and uses a sample rate that is faster than the rate that is 
% used for torque control. The task scheduling is designed in 
% Stateflow(R). The Scopes subsystem contains scopes that allow you to 
% see the simulation results.

% Copyright 2017-2018 The MathWorks, Inc.

%% Machine Parameters
Pmax = 100e3;      % Maximum power                      [W]
Tmax = 160;        % Maximum torque                     [N*m]
Ld   = 0.00024368; % Stator d-axis inductance           [H]
Lq   = 0.00029758; % Stator q-axis inductance           [H]
L0   = 0.00012184; % Stator zero-sequence inductance    [H]
Rs   = 0.010087;   % Stator resistance per phase        [Ohm]
psim = 0.04366;    % Permanent magnet flux linkage      [Wb]
p    = 5;          % Number of pole pairs
Jm   = 0.067;        % Rotor inertia                      [Kg*m^2]

%% High-Voltage System Parameters
Cdc  = 0.001;      % DC-link capacitor [F]
Vnom = 540;        % Nominal DC voltage[V] 
V1   = 539;        % Voltage V1(< Vnom)[V]

%% Control Parameters
Ts   = 5e-6;       % Fundamental sample time               [s]
fsw  = 10e3;       % PMSM drive switching frequency        [Hz]
Tsi  = 1e-4;       % Sample time for current control loops [s]

Kp_id = 0.8779;     % Proportional gain id controller
Ki_id = 710.3004;   % Integrator gain id controller
Kp_iq = 1.0744;     % Proportional gain iq controller
Ki_iq = 1.0615e+03; % Integrator gain iq controller

Kp_omega = 0.5;     % Proportional gain velocity controller
Ki_omega = 10;     % Integrator gain velocity controller

%%Discrete PI Controller
K_p = 1; % Proportional gain to PI controller
K_v = 1;   %Proportional gain to P controller
K_i = 200;   %Integral gain to PI controller
K_aw = 100; %Anti-windup gain

%% Zero-Cancellation Transfer Functions
numd_id = Tsi/(Kp_id/Ki_id);
dend_id = [1 (Tsi-(Kp_id/Ki_id))/(Kp_id/Ki_id)];
numd_iq = Tsi/(Kp_iq/Ki_iq);
dend_iq = [1 (Tsi-(Kp_iq/Ki_iq))/(Kp_iq/Ki_iq)];

%% Current references
load ee_ipmsm_35kW_ref_idq;
%% CAN parameters
msg_period = 10e-3;
%% Message CAN file
LOG_CAN = canMessageTimetable(msg_can)
writetimetable(LOG_CAN,'log_can.txt')
