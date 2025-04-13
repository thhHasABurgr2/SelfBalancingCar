pkg load control
pkg load signal

function eigenvalues = compute_eigenvalues(A)
  eigenvalues = eig(A);
endfunction
function radS = rpmToradS(rpm)
    radS=rpm*(2*pi/60);
endfunction
function [Aaug,Baug, Qaug, R]= augment(A,B,C,Q,R)
    n=6;%6 states
    m=2;% 2 inputs
    p=2 % 2 outputs
    Aaug=[A,zeros(n,p); C, zeros(p,p)];
    Baug=[B;zeros(p,m)];
    Qint=10*eye(p);
    Qaug=blkdiag(Q,Qint);
    return;
endfunction


% Check controllability of individual states
function checkStateControllability(A, B)
    n = size(A, 1);
    disp('Checking controllability of individual states:');
    
    for i = 1:n
        % Create a vector that selects the i-th state
        Ci = zeros(1, n);
        Ci(i) = 1;
        
        % Check PBH test for controllability of this state
        uncontrollable = true;
        for eigenval = transpose(eig(A))
            % For each eigenvalue, check if [λI-A, B] has full row rank
            test_matrix = [eigenval*eye(n) - A, B];
            row_vector = Ci * test_matrix;
            
            % If row is zero, state is uncontrollable for this eigenvalue
            if norm(row_vector) > 1e-10
                uncontrollable = false;
                break;
            endif
        endfor
        
        if uncontrollable
            printf('State %d is UNCONTROLLABLE\n', i);
        else
            printf('State %d is controllable\n', i);
        endif
    endfor
endfunction


% Motor constants
kt = 0.275;
kgr = 1/46;
ke = kt;
R = 4.8;
k = kt * kgr;
m_w = 0.06;
r = 0.026;
I_w = (m_w * r^2) / 2;

% Physical constants
h = 0.11;
w = 0.120;
l = 0.205;
m = 0.725748;
g = 9.81;
b = 0.05;
I_psi = m * (l^2 + w^2);
I_theta = m * (h^2 + w^2);

A = [
    0, 1, 0, 0, 0, 0;
    (h*m*g)/(2*I_theta), -b/I_theta, 0, 0, -(k*ke)/(2*R*I_theta), -(k*ke)/(2*R*I_theta);
    0, 0, 0, 1, 0, 0;
    0, 0, 0, 0, -(k*ke)/(2*R*I_psi), (k*ke)/(2*R*I_psi);
    0, 0, 0, 0, -(k*ke)/(2*R*I_w), 0;
    0, 0, 0, 0, 0, -(k*ke)/(2*R*I_w)
];

B = [
    0, 0;
    k/(2*R*I_theta), k/(2*R*I_theta);
    0, 0;
    k/(2*R*I_psi), -k/(2*R*I_psi);
    k/(2*R*I_w), 0;
    0, k/(2*R*I_w)
];

C = [
    1, 0, 0, 0, 0, 0;
    0, 0, 1, 0, 0, 0
];

D = zeros(2,2);

eigVals = compute_eigenvalues(A);
printf("eigenvals:%f\n",eigVals)
omega = 0;
zeta = 0;

for i = 1:length(eigVals)
  if imag(eigVals(i)) != 0
    omega = abs(eigVals(i));
    zeta = -real(eigVals(i)) / omega;
  endif
endfor

omegaF = omega / (2 * pi);
nyquistF = omegaF / 2;

printf("Damping ratio: %f\n", zeta);
printf("Nyquist frequency: %f Hz\n", nyquistF);
printf("Nyquist period (ms): %f\n", 1000 / nyquistF);

overSampleGain = 250;
samplingFrequency = nyquistF * overSampleGain;
T = 1 / samplingFrequency;
printf("%dx oversampling constant\nSampling frequency: %f Hz\nSampling period: %f ms\n", overSampleGain, samplingFrequency, T * 1000);
T=.01;
sys_c = ss(A, B, C, D);
sys_d = c2d(sys_c, T, 'zoh');
A_d = sys_d.A;
B_d = sys_d.B;
C_d = sys_d.C;
D_d = sys_d.D;
checkStateControllability(A_d,B_d);
% Proper controllability matrix calculation
Co = [B_d, A_d*B_d, A_d^2*B_d, A_d^3*B_d, A_d^4*B_d, A_d^5*B_d];
printf("Controllability Matrix Rank: %d\n", rank(Co));

Ob = [C_d; C_d * A_d; C_d * A_d^2];
printf("Observability Matrix Rank: %d\n", rank(Ob));

eigvals_A_d = eig(A_d);
printf("Eigenvalues of A_d without control: \n");
disp(eigvals_A_d);

%max values.
% pitch cant exceed 15 degrees due to linearizations
% idk limitations on pitchRate. ig pi rad/s as the max is decent
%yaw can be inf too, but imma say 2pi radians
%idk for yawRate 
%Rated speed is 135 rpm. convert to rad/s for max wheel vvelocity
brysonX=[deg2rad(15),pi/8,2*pi,pi, rpmToradS(135),rpmToradS(135)]; 

brysonU=[12,12];% max voltage for both motors
Qdiag=1./(brysonX.^2);
Rdiag=1./(brysonU.^2)
Q = diag(1./(brysonX.^2));
R = diag([1./(brysonU.^2)]);
[Aaug,Baug,Qaug,R]=augment(A_d,B_d,C_d,Q,R);
printf("Q matrix\n");
disp(Q)
printf("R matrix\n");
disp(R)
P = dare(Aaug, Baug, Qaug, R);
K = inverse(transpose(Baug)*P*Baug+R)*(transpose(Baug)*P*Aaug);
closed_loop = Aaug - Baug * K;

printf("Closed-loop eigenvalues:\n");
disp(eig(closed_loop));
printf("K matrix:\n");
disp(K);

initX=[deg2rad(10);0;deg2rad(3);0;0;0];


function sim(tSteps,A,B,C,K,initX,T,setpoint,disturbance)

Xhist = zeros(6, tSteps);
Xhist(:,1)=initX;
Yhist=zeros(2,tSteps);
Yhist(:,1)=[0;0];
Uhist=zeros(2,tSteps);
Uhist(:,1)=[0;0];

sum=zeros(6,1);
n=6;
m=2;
for k=2:tSteps
    if(mod(k,50)==0)
        sum=0;
    endif;
    e=-Xhist(:,k-1)+setpoint;
    sum+=e;
    U=-Ke;
    X=A*e+B*U;
    Y=C*X;
    Xhist(:,k)=X;
    Yhist(:,k)=Y;
    Uhist(:,k)=U;
    


    
endfor

t = (0:tSteps-1) * T;

disp(transpose(setpoint));
printf("disturbance vector: ");
disp(disturbance);
figure;
plot(t, rad2deg(Yhist(1,:)), 'b', 'DisplayName', 'Pitch \theta (degrees)');
hold on;
plot(t, rad2deg(Yhist(2,:)), 'r', 'DisplayName', 'Yaw \psi (degrees)');
xlabel('Time (s)');
ylabel('Angle (degrees)');
title('simulation with disturbance testing and step response');
legend show;
grid on;
hold off;
figure;
plot(t,Uhist(1,:),'b','DisplayName','motor1 (u1)');
hold on;
plot(t,Uhist(2,:),'r','DisplayName','motor2 (u2)');
xlabel('Time (s)')
ylabel('Voltage (V)');
title("voltages of the motors");
legend show;
grid on;
hold off;
endfunction

setpoint=[0;0;0;0;0;0];

disturbance=[0;0;0;0;0;0];

sim(200,Aaug,Baug,Caug,K,initX,T,setpoint,disturbance); % time sim without the special stuff