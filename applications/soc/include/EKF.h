/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-06     sippa       the first version
 */
#ifndef APPLICATIONS_SOC_INCLUDE_EKF_H_
#define APPLICATIONS_SOC_INCLUDE_EKF_H_

#include "ocv.h"
#include "ulapack.h"
#include <stdbool.h>

/**
* @brief    Update part of Extended Kalman Filter.
* @note     part of ekf where we 'update' state of charge based on  state from 'predict' part and the voltage on the cell
*
* @param[in] voltage  voltage reading
* @param[in] R  return the measurement noise
* @param[in] Q_capacity,Wh_Energy capacity or energy of the investigated cell in Ah, Wh respectively,
* @param[out] x_pack  return the array of state of charge
* @param[out] P       return the updated state covariance matrix
*
*
** Battery model
* X = [SOC
*      Vrc]
*
* U = [I] Current
*
* A = [1  0
*      0  e^-(t/r1c1)]
*
* B = [-t/Q
*       r1(1-e^-(t/r1c1))]
*
* H = [d(ocv)/d(soc), -1]
*
* Z = [V] Terminal voltage
*
* Proof
* H matrix = partial derivative of measurement respect with state Z(x)/dx
* Z = Vt = FnOCV(soc) - Vrc - RI
* H[1][2] = [d(OCV(soc) - Vrc - RI)/d(soc), d(FnOCV(soc) - Vrc - RI)/d(Vrc)]
* H[1][2] = [d(OCV)/d(soc) - 0 - 0, 0 -1 - 0]
* H[1][2] = [d(OCV)/d(soc), -1]
*
* HX[1]   = [d(OCV)/d(soc), -1] [SOC
*                                Vrc]
* HX[1]   = SOCd(OCV)/d(soc) - Vrc
* ASSUME for easy calculation
* HX[1]   = FnOCV(soc) - Vrc
*
* Calculate kalman gain
* K = P*Ht/[H*P*Ht+R]
* K [2][1]
* H [1][2]
* Ht[2][1]
* P [2][2]
* R [1]
*
* K[2][1] = (P[2][2]Ht[2][1])/(H[1][2]P[2][2]Ht[2][1] + R[1])
* STEP
* 1.FINE H[2][1]   H[2][1]  = [d(OCV)/d(soc), -1] ->H[1][2]
* 2.MUL  X1        X1[1][2] = H[1][2]P[2][2] -> X1[1][2]
* 3.TRAN H         Ht[2][1] = trans[H] -> Ht[2][1] = trans(H[1][2])
* 4.MUL  X2        X2[1][1] = X1[1][2]Ht[2][1] -> X2[1][1]
* 5.ADD  S1        S[1][1]  = X2[0][0] + R
* 6.MUL  X3        X3[2][1] = P[2][2]Ht[2][1] -> X3[2][1]
* 7.INV  SI        SI[1][1] = inv[S] -> SI[1][1] = inv(S[1][1])
* 8.MUL  K         K[2][1]  = X3[2][1]SI[1][1] -> K[2][1]
*
* Estimate correction
* Xk = Xk-1 + K(Z -HXk-1)
* STEP
* 1.MEL  HX[1]       HX = FnOCV(soc) - Vrc
* 2.SUB  E           E  = MEASURE - HX[1]
* 3.MUL  X4[2][1]    X4[2][1] = K[2][1]E[1][1] -> X4[2][1]
* 4.ADD  SOC[2][1]   SOC[2][1] = SOC[2][1] + X4[2][1] -> SOC[2,1]
*
* Error covariance update P
* P = (I-KH)P
* STEP
* 1.MUL KH[2][2]     KH[2][2] = K[2][1]H[1][2] -> KH[2][2]
* 2.INT I[2][2]      I[2][2]  = Identity Matrix
* 3.SUB I_KH[2][2]   I_KH[2][2] = I[2][2]- KH[2][2] -> I_KH[2][2]
* 4.MUL X5[2][2]     X5[2][2] = I_KH[2][2]P[2][2] -> X5[2][2]
*
*/
void ekf_update(float voltage,  float R, float Q_capacity, Matrix_t *x_pack, Matrix_t *P, float current);

/**
* @brief    Prediction part of Extended Kalman Filter.
* @note     part of ekf where we 'predict' state of charge based on current state and current consumption
*
* @param[in] A the array of state transition model 1
* @param[in] B the array of state transition model 2
* @param[in] Q the process noise covariance matrix
* @param[in] current the current consumption + charge, - discharge
* @param[in] voltage reading
* @param[out] x_pack  return the array of state of charge
* @param[out] P  return the updated state covariance matrix
*
* Battery model
* X = [SOC
*      Vrc]
*
* U = [I] Current
*
* A = [1  0
*      0  e^-(t/r1c1)]
*
* B = [-t/Q
*       r1(1-e^-(t/r1c1))]
*
* SOC = SOC + -tI/Q or -tVI/Wh
* Vrc = Vrc*e-(t/r1c1) + I*r1(1-e^-(t/r1c1))
*
* The equation of Vrc come from
* I=Cdv/dt -> dv/dt = I/C
* Current pass to r in rc = Ir = Vrc/R from v=ir
* Current pass to c in rc = Ic = CdVrc from I=Cdv/dt
* Total current = Iload = Ir + Ic
* Iload = Vrc/R + CdVrc
* Iload/C = Vrc/RC + dVrc
* dVrc = Iload/C - Vrc/RC
*
* The solution of dVrc = Iload/C - Vrc/RC is equation below
* Vrc = Vrc*e-(t/r1c1) + I*r1(1-e^-(t/r1c1))
*
* Proof
* Example slove fisrt-order linear ordinary equation
* dVrc/dt = -Vrc/RC to discrete equation
* 1.MOVE          dVrc/Vrc = (-1/RC)dt
* 2.INTEGRATE     ∫ (1/Vrc) dVrc = ∫ (-1/RC) dt
* 3.SOLVE         ln(Vrc) = (-1/RC)t + C
* 4.TAKE ln by e  e^[ln(Vrc)] = e^[(-1/RC)t + C ]
* 5.e^[ln(x)]=x   Vrc = e^[(-1/RC)t + C ]
* 6.RESULT ->     Vrc = V(0)e^[(-1/RC)t]
*
* Add ∫ Iload/C dt
* 1.CONV INTEGRATE      ∫ g(t-tr) I(tk) dtr
* 2.DEFINE              INPUT=I(tr)/C SYSRES=e^-((t-tr)/RC
* 3.INPUT AT tk         I(tr)/C
* 4.SYSRES AT tr to t   e^-((t-tr)/RC
* 5.CONV                ∫ I(tr)/C * e^-((t-tr)/RC) dtr
* 6.ASSUME CONST I(tr)  T(tr)/C  ∫e^-((t-tr)/RC) dtr
*
* Slide time point tk to tk-1
* 1.DEFINE      tk=kΔt and tk-1= (k-1)Δt
* 2.HOMO EQ     Vrc(tk) = V(tk-1)e^[(-1/RC)Δt]
* 3.I(t)        I(tk-1)/C  ∫e^-((tk-tr)/RC) dtr
* 4.INTEGRATE   ∫e^-((t-tr)/RC) dtr
* 5.CHANGE VAR  u=tk-tr du=-dtr
* 6.DEFINE      When tr=tk-1, u=tk - tk-1 When tr=tk, u=0
* 7.INTEGRATE   ∫e^-((u)/RC) du form u=0 to u=tk - tk-1
* 8.SLOVE1      -(RC)e^-((u)/RC |tk - tk-1 to 0
* 9.SLOVE2      -(RC) [e^-((tk - tk-1)/RC) - e^((0)/RC)]
* 10.SLOVE3     -(RC) [e^-((Δt)/RC) - 1]
* 11.ADD        [I(tk-1)/C][(RC) [1-e^-((Δt)/RC)]]
*
* Final
* Vrc(tk) = V(tk-1)e^[(-1/RC)Δt] + [I(tk-1)R][1-e^-((Δt)/RC)]
*
* X = AX + BU
* X[SOC      = [ 1  0            [SOC    +   [-t/Q              [I]
*   Vrc]          0  e-(t/r1c1)]  Vrc]        r1(1-e-(t/r1c1))]
*
* Calculation state estimate
* X(k) = AX(k-1) + BU(k)
* X[2][1] = A[2][2]X[2][1] + B[2][1]U -> result = X[2][1] = SOC[2][1]
*
* Calculation error covariance matrix P
* P(k)    = AP(k-1)At + Q
* P[2][2] = A[2][2]P[2][2]At[2][2] + Q[2][2] -> result = P[2][2]
*
* STEP
* 1.MUL  X1   X1 = A*P -> X1[2][2] = A[2][2]P[2][2]
* 2.TRAN A    At = trans[A] -> At[2][2] = trans(A[2][2])
* 3.MUL  X4   X4 = X1*At -> X4[2][2] = X1[2][2]At[2][2]
* 4.Add  P    P  = X4 + Q -> P[2][2] = X4[2][2] + Q[2][2]
*
*/
void ekf_predict(Matrix_t *A, Matrix_t *B, Matrix_t *Q,float current, float voltage, Matrix_t *x_pack, Matrix_t *P);

void ekf_init_local_matrix();


#endif /* APPLICATIONS_SOC_INCLUDE_EKF_H_ */
