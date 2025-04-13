import numpy as np
import matplotlib.pyplot as plt
import control
import math
from scipy import signal
from scipy import integrate
from scipy import linalg
from numpy.linalg import matrix_rank
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
kt=.275
kgr=1/46
ke=kt
R=4.8
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
    [0, 0, 0, 0, -(k*ke) / (2 * R * I_psi), (k*ke) / (2 * R * I_psi)],
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
print("eigenVals: ",eigVals)
T=.01
discrete_sys = signal.cont2discrete((A,B,C,D), T, method='zoh')
A_d, B_d, C_d, D_d = discrete_sys[0], discrete_sys[1], discrete_sys[2], discrete_sys[3]

print(np.shape(B_d))
import numpy as np
from numpy.linalg import matrix_rank

# Assuming A_d, B_d, C_d are already defined and A_d is 6×6
n = 6  # Your system dimension

# Build controllability matrix with all 6 terms
Co = np.hstack([B_d, 
                A_d @ B_d, 
                A_d @ A_d @ B_d,
                A_d @ A_d @ A_d @ B_d,
                A_d @ A_d @ A_d @ A_d @ B_d,
                A_d @ A_d @ A_d @ A_d @ A_d @ B_d])

print("Controllability Matrix Rank:", matrix_rank(Co))
print("System is controllable:", matrix_rank(Co) == n)

# Build observability matrix with all 6 terms
Ob = np.vstack([C_d, 
                C_d @ A_d, 
                C_d @ A_d @ A_d,
                C_d @ A_d @ A_d @ A_d,
                C_d @ A_d @ A_d @ A_d @ A_d,
                C_d @ A_d @ A_d @ A_d @ A_d @ A_d])

print("Observability Matrix Rank:", matrix_rank(Ob))
print("System is observable:", matrix_rank(Ob) == n)
discreteEigVals = np.linalg.eigvals(A_d)
print("Eigenvalues of A_d:",discreteEigVals)
kp=40

ki=0

X0=np.array([math.radians(10),0,math.radians(0),0,0,0])
tSteps=500
time=np.arange(tSteps)*T
X = np.zeros((tSteps, 6))
Y = np.zeros((tSteps, 2))
X[0]=X0


Uk=np.zeros((tSteps,2))
for k in range(1,tSteps):
    Y[k-1]=C_d@X[k-1]
    if(k==1):
        Uk[k-1]=kp*Y[k-1]
        print(Y[k-1])
        print(Uk[k-1])
        
    else:
        Uk[k-1]=kp*Y[k-1]+ki*Y[k-2]
    X[k]=A_d@X[k-1]+B_d@Uk[k-1]
X=np.array(X)
Y=np.array(Y)
Uk=np.array(Uk)

plt.figure(figsize=(10, 5))
plt.plot(time,np.rad2deg(Y[:, 0]) , label=r'Pitch $\theta$ (degrees)')
plt.plot(time,np.rad2deg(Y[:, 1] ), label=r'Yaw $\psi$ (degrees)')
plt.xlabel("TStep. convert to ms or s for poster")
plt.ylabel("Angle (degrees)")
plt.legend()
plt.title("time sim of LQR control")
plt.grid()


plt.figure()
plt.plot(time,Uk[:,0],label=r'Motor 1 effort (V)')
plt.plot(time,Uk[:,1],label=r'Motor 2 effort (V)')
plt.xlabel("time")
plt.ylabel("voltage")
plt.legend()
plt.title("Motor efforts")
plt.grid()
plt.show()

