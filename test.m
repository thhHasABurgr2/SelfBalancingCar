pkg load control
pkg load signal

function eigenvalues = compute_eigenvalues(A)
  eigenvalues = eig(A);
endfunction
function radS = rpmToradS(rpm)
    radS=rpm*(2*pi/60);
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

overSampleGain = 1;
samplingFrequency = nyquistF * overSampleGain;
T = 1 / samplingFrequency;
printf("%dx oversampling constant\nSampling frequency: %f Hz\nSampling period: %f ms\n", overSampleGain, samplingFrequency, T * 1000);
