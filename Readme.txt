[Project Overview]
This project is designed for the numerical solution of the moving sofa problem under different corridor angles. The method is based on a boundary-pressure-induced dynamical system model. By constructing the continuous evolution of rigid-body configurations, the algorithm searches for possible extremal shapes within the feasible domain. The program implements two different rotation strategies and computes the effective contact boundary length via set intersection operations, thereby obtaining generalized forces to drive the system evolution and ultimately converge to an equilibrium configuration.

[File Description]
a_sofa.cpp  
Implements the numerical computation for the first rotation mode (clockwise rotation through the supplementary corridor angle).

b_sofa.cpp  
Implements the numerical computation for the second rotation mode (counterclockwise rotation through the full corridor angle).

intersecto.h  
Header file for performing set intersection operations, mainly used to compute corridor intersection regions and boundary truncation intervals.

[Key Adjustable Parameters]
S : Corridor angle corresponding to the sofa  
n : Number of corridor discretizations (number of rigid frames)

[Output]
After execution, the program outputs two txt files:

1. Norm of Force Evolution  
   Content: The norm of the force R as a function of time (or iteration steps), used to determine whether the system converges to an equilibrium state.

2. Configuration Data  
   Content: Positions (x_k, y_k) of the reference points (origins of rigid frames), which can be used to reconstruct the sofa shape or for visualization.

[Usage]

1. Default Initialization  
   The program can start from a simple initial configuration (e.g., all rigid frames initialized near the origin), and then gradually converges to a stable solution through dynamical system evolution.

2. Interpolation-Based Initialization  
   The program supports high-resolution initialization based on existing results:  
   A result file (txt) with a smaller number of rigid frames n under the same corridor angle can be provided. The program interpolates this data and uses it as the initial condition for a larger n, improving computational efficiency and convergence stability.  
   To use this feature, it is necessary to manually enable the commented input-related sections in the code (including file reading and dynamic memory deallocation).
