/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-06     sippa       the first version
 */

#include "EKF.h"

//Prediction
Matrix_t *P1;
Matrix_t *P2;
Matrix_t *P3;
Matrix_t *P4;
Matrix_t *AT;

//Update
Matrix_t *U1;
Matrix_t *U2;
Matrix_t *U3;
Matrix_t *U4;
Matrix_t *U5;
Matrix_t *U6;
Matrix_t *U7;
Matrix_t *U8;
Matrix_t *K;
Matrix_t *KH;
Matrix_t *K_T;
Matrix_t *HJacobian;
Matrix_t *HJacobian_T;
Matrix_t *Err;
Matrix_t *S;
Matrix_t *S_I;
Matrix_t *I;
Matrix_t *I_KH;
Matrix_t *I_KH_T;
bool is_init = false;

void ekf_init_local_matrix()
{
    //predict
    ulapack_init(&P1, 2, 1);
    ulapack_init(&P2, 2, 1);
    ulapack_init(&P3, 2, 2);
    ulapack_init(&P4, 2, 2);
    ulapack_init(&AT, 2, 2);

    //update
    ulapack_init(&HJacobian,  1, 2);
    ulapack_init(&HJacobian_T, 2, 1);
    ulapack_init(&Err, 1, 1);
    ulapack_init(&K, 2, 1);
    ulapack_init(&KH, 2, 2);
    ulapack_init(&K_T, 1, 2);
    ulapack_init(&S, 1, 1);
    ulapack_init(&S_I, 1, 1);
    ulapack_init(&I,  2, 2);
    ulapack_init(&I_KH,  2, 2);
    ulapack_init(&I_KH_T,  2, 2);
    ulapack_init(&U1, 1, 2);
    ulapack_init(&U2, 1, 1);
    ulapack_init(&U3, 2, 1);
    ulapack_init(&U4, 2, 1);
    ulapack_init(&U5, 2, 2);
    ulapack_init(&U6, 2, 2);
    ulapack_init(&U7, 2, 1);
    ulapack_init(&U8, 2, 2);

    is_init = true;
}

void ekf_predict(Matrix_t *A, Matrix_t *B, Matrix_t *Q,float current, float voltage, Matrix_t *x_pack, Matrix_t *P)
{

    if(is_init == false)
    {
        ekf_init_local_matrix();
    }

    //Prediction state X=AX+BU
    //SOC = SOC + -tI/Q or -tVI/Wh
    //Vrc = Vrc*e-(t/r1c1) + I*r1(1-e^-(t/r1c1))

    //P1 = A*x_pack = AX
    ulapack_product(A,x_pack,P1);
    //P2 = B*(current*voltage) = BU
    ulapack_scale(B,(current*voltage),P2); //if we pass only current no need to divide B[1][0] and change Wh to Ah in B matrix
    //divide B[1][0] with voltage
    P2->entry[1][0] = P2->entry[1][0]/voltage;

    //Add x_pack = P1+P2 = AX+BU
    ulapack_add(P1, P2, x_pack);

    /////////////////////////////////////////////////////////////////////////
    //Prediction convarince P=APAT+Q

    //P3 = A*P
    ulapack_product(A,P,P1);
    //AT = tran(A)
    ulapack_transpose(A, AT);
    //P4 = P3*AT = APAT
    ulapack_product(P3, AT, P4);

    //P = X4+Q = APAT + Q
    ulapack_add(P4, Q, P);

}

void ekf_update(float voltage,  float R, float Q_capacity, Matrix_t *x_pack, Matrix_t *P, float current)
{
    if(is_init == false)
    {
        ekf_init_local_matrix();
    }

    /**
    Estimate correction
    Xk = Xk-1 + K(Z -HXk-1)
    **/

    //find jacobian H
    //H[1][2] = [d(ocv)/soc, -1]
    HJacobian->entry[0][0] = ocv_deriv_sys(x_pack->entry[0][0], current);
    HJacobian->entry[0][1] = -1;

    //find Hx
    //Hx = [d(ocv)/soc, -1] * [soc,Vrc] = [ocv - Vrc]
    float HX = ocv_sys(x_pack->entry[0][0],current) - x_pack->entry[1][0];

    //Error
    float Error = voltage - HX;
    Err->entry[0][0] = Error;

    //Calculate K (Kalman gain)
    //K = P*Ht/[H*P*Ht+R]

    //find transpose HT
    ulapack_transpose(HJacobian, HJacobian_T);

    //U1[1][2] = HP
    ulapack_product(HJacobian, P, U1);

    //U2[1][1] = U1HT = HPHT
    ulapack_product(U1, HJacobian_T, U2);

    //S = U2 + R = HPHT + R
    S->entry[0][0] = U2->entry[0][0] + R;

    //Inverse S = S_I
    ulapack_inverse(S, S_I);

    //U3 = P*HT
    ulapack_product(P, HJacobian_T, U3);

    //K = U3*S_I = P*HT * [HPHT + R]^-1
    ulapack_product(U3, S_I, K);

    //U4 = KErr
    ulapack_product(K, Err, U4);

    //X = X + U4 = X + KErr
    ulapack_add(x_pack, U4,x_pack);

    /**
    Error covariance update P
    P = (I-KH)P or (I-KH)P(I-KH)t + KRKt
    **/

    //Identity matrix
    ulapack_eye(I);

    //KH = K*HJacobian
    ulapack_product(K, HJacobian, KH);

    //I_KH = I - KH
    ulapack_add(I, KH, I_KH);

    //U5 = (I_KH)P if we not use (I-KH)P(I-KH)t + KRKt P = U5 is done
    ulapack_product(I_KH, P, U5);

    //I_KH_T = trans(I_KH)
    ulapack_transpose(I_KH, I_KH_T);

    //U6 = U5*I_KH_T = (I_KH)P(I_KH_T)
    ulapack_product(U5,I_KH_T,U6);

    //U7 = KR
    ulapack_scale(K, R, U7);

    //K_T = trans(K)
    ulapack_transpose(K, K_T);

    //U8 = U7*K_T = (K)R(K_T)
    ulapack_product(U7, K_T, U8);

    //P = U6 +U8 = (I_KH)P(I_KH_T) + (K)R(K_T)
    ulapack_add(U6, U8, P);
}
