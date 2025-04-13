import numpy as np
import matplotlib.pyplot as plt
import math
from scipy import signal
from scipy import integrate
from scipy import linalg
from numpy.linalg import matrix_rank
def compute_eigenvalues(A):
    eigenvalues = np.linalg.eigvals(A)
    return eigenvalues

#motor constants
kt=.2207
kgr=1/46
ke=kt
R=4.5
k=kt*kgr
m_w=0.06
r=0.026
I_w=(m_w*math.pow(r,2))/2
#physical constants
h=.11
w=.12
l=.205
m=0.725748
g=9.81
b=.051121
I_psi=(m*(math.pow(l,2)+math.pow(w,2)))
I_theta=(m*(math.pow(h,2)+math.pow(w,2)))

A = np.array([
    [0, 1, 0, 0, 0, 0],
    [(h*m*g)/(2*I_theta), -b/I_theta, 0, 0,-(k*ke)/ (2 * R * I_theta),-(k*ke) / (2 * R * I_theta)],
    [0, 0, 0, 1, 0, 0],
    [0, 0, 0, -b/I_psi, -(k*ke) / (2 * R * I_psi), (k*ke) / (2 * R * I_psi)],
    [0, 0, 0, 0, -(k*ke)/ (2*R * I_w), 0],
    [0, 0, 0, 0, 0, -(k*ke) / (2*R * I_w)]
])
B=np.array([
    [0,0],
    [k/(2*R*I_theta),k/(2*R*I_theta)],
    [0,0],
    [k/(2*R*I_psi),-k/(2*R*I_psi)],
    [k/(2*R*I_w),0],
    [0,k/(2*R*I_w)]
])

C= np.array([
    [1,0,0,0,0,0],
    [0,0,1,0,0,0]
])
D=np.zeros((6,2))


eigVals=compute_eigenvalues(A)
#print("eigenvalues: ",eigVals)
omega=0
zeta=0
for eig in eigVals:
    if np.imag(eig) != 0: 
        omega = np.abs(eig)  
        zeta=-np.real(eig)/omega
omegaF=omega/(2*math.pi)
nyquistF=omegaF/2

print("damping ratio: ", zeta)
#print("Natural frequency: {0} Hz".format( omegaF))
print("Nyquist frequency: {0} Hz".format(nyquistF))
#print("Natural period (ms): ", 1000/omegaF)
print("Nyquist period (ms): ",1000/nyquistF)
overSampleGain=250
samplingFrequency=nyquistF*overSampleGain
T=1/samplingFrequency
print("{0}x oversampling constant\nSampling frequency: {1} Hz\nsampling period: {2} ms".format(overSampleGain,samplingFrequency,T*1000))

discrete_sys = signal.cont2discrete((A,B,C,D), T, method='zoh')
A_d, B_d, C_d, D_d = discrete_sys[0], discrete_sys[1], discrete_sys[2], discrete_sys[3]


Co = np.hstack([B_d, A_d @ B_d, A_d @ A_d @ B_d])
print("Controllability Matrix Rank:", matrix_rank(Co))

Ob = np.vstack([C_d, C_d @ A_d, C_d @ A_d @ A_d])
print("Observability Matrix Rank:", matrix_rank(Ob))
eigvals = np.linalg.eigvals(A_d)
print("Eigenvalues of A_d:", eigvals)

Q=np.diag([
    500,100,1,0,0,0
])
R=np.diag([
    .01,.01
])
P=linalg.solve_discrete_are(A_d,B_d,Q,R)
K=np.linalg.inv(R).dot(B_d.T).dot(P)
closed_loop = A_d - B_d.dot(K)
print("Closed-loop eigenvalues:", np.linalg.eigvals(closed_loop))
print("K matrix (first row):", K[0,:])
def system_dynamics(X):
    U = -K @ X
    Xprime = A_d.dot(X) + B_d.dot(U)
    return Xprime
X0=np.array([math.radians(2),0,math.radians(6),0,0,0])
tSteps=500
Xhist=[X0]
for k in range(tSteps):
    X0=system_dynamics(X0)
    Xhist.append(X0.copy())
Xhist=np.array(Xhist)
plt.figure(figsize=(10, 5))
plt.plot(np.array(Xhist)[:, 0] *180/np.pi, label=r'Pitch $\theta$ (degrees)')
plt.plot(np.array(Xhist)[:, 2] *180/np.pi, label=r'Yaw $\psi$ (degrees)')
plt.xlabel("TStep. convert to ms or s for poster")
plt.ylabel("Angle (degrees)")
plt.legend()
plt.title("sim of LQR control")
plt.grid()
plt.show()
plt.close('all')