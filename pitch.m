pkg load control
pkg load signal

function eigenvalues = compute_eigenvalues(A)
  eigenvalues = eig(A);
endfunction
function radS = rpmToradS(rpm)
    radS=rpm*(2*pi/60);
endfunction
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
kt = 0.1604;% in Nm/A
kgr = 1/46;
ke = kt;% in V \cdot s/rad
R = 4.8;% in Ohms
k = kt * kgr;%simplification
m_w = 0.06;%kg
r = 0.026;%m
I_w = (m_w * r^2) / 2;%kg\cdot m^2

% Physical constants
h = 0.11;%m
w = 0.135;%m
m = 0.725748;%kg
g = 9.81;%m/s^2
b = 0.0488;%Nm\cdot s/rad

I_theta = m * (h^2 + w^2)/36;%kg\cdot m^2

A = [
    0, 1, 0, 0;
    (h*m*g)/(2*I_theta), -b/I_theta, -(k*ke)/(2*R*I_theta), -(k*ke)/(2*R*I_theta);
    0, 0, -(k*ke)/(2*R*I_w), 0;
    0, 0, 0, -(k*ke)/(2*R*I_w)
];

B = [
    0, 0;
    k/(2*R*I_theta), k/(2*R*I_theta);
    k/(2*R*I_w), 0;
    0, k/(2*R*I_w)
];

C = [
    1, 0, 0, 0;
];

D = zeros(1,2);

eigVals = compute_eigenvalues(A);
printf("eigenvals:%f\n",eigVals)

T=.005;
sys_c = ss(A, B, C, D);
sys_d = c2d(sys_c, T, 'zoh');
A_d = sys_d.A;
B_d = sys_d.B;
C_d = sys_d.C;
D_d = sys_d.D;

checkStateControllability(A_d,B_d);

Co = [B_d, A_d*B_d, A_d^2*B_d, A_d^3*B_d];
printf("Controllability Matrix Rank: %d\n", rank(Co));

Ob = [C_d; C_d * A_d; C_d * A_d^2;C_d*A_d^3];
printf("Observability Matrix Rank: %d\n", rank(Ob));

eigvals_A_d = eig(A_d);
printf("Eigenvalues of A_d without control: \n");
disp(eigvals_A_d);
%beginning steady state time response graph
initX=[deg2rad(-1);0;0;0];
t=1;% seconds
tSteps=t/T;
Xhist = zeros(4, tSteps);
Xhist(:,1)=initX;
Yhist=zeros(1,tSteps);
Yhist(:,1)=C_d*initX;
Uhist=zeros(2,tSteps);
Uhist(:,1)=[0;0];
setpoint=[0;0;0;0];
U=zeros(2,1);
for k=2:tSteps

    
    
    X=A_d*Xhist(:,k-1)+B_d*U;
    Y=C_d*X;
    if Y > deg2rad(15)
        Y=deg2rad(15);
    endif
    if Y < deg2rad(-15)
        Y=deg2rad(-15);
    endif

    Xhist(:,k)=X;
    Yhist(:,k)=Y;
    Uhist(:,k)=U;
endfor
tGraph = (0:tSteps-1) * T;
disp(transpose(setpoint));
figure;
plot(tGraph, rad2deg(Yhist(1,:)), 'b', 'DisplayName', 'Pitch \theta (degrees)');
xlabel('Time (s)');
ylabel('Angle (degrees)');
title('time simulation without control');
legend show;
grid on;

