#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

using namespace std;

#ifndef A_2_H
#define A_2_H
void Intersect1(double *B0, double *A0);
void Intersect1(double *B0, double *A0)
{
    int a1, a2, c1, c2;
    c1 = 0;
    c2 = 0;
    a1 = 2*lround(A0[0])+1;
    a2 = 2*lround(A0[0])+1;
    for(int j = 1; j <= 2*lround(A0[0]); j++) {
        if((B0[1] <= A0[j]) && (c1 == 0)) {
            c1 = 1;
            a1 = j;
        }
        if((B0[2] <= A0[j]) && (c2 == 0)) {
            c2 = 1;
            a2 = j;
        }
    } //寻找B中集合的相对位置

    if((a1 % 2 == 1)&&(a2 % 2 == 1)&&(a1 != a2)) {
        A0[0] = (a2-a1)/2;
        for(int j = 1; j <= 2*lround(A0[0]); j++) A0[j] = A0[a1+j-1];
    }
    if((a1 % 2 == 1)&&(a2 % 2 == 1)&&(a1 == a2)) {
        A0[0] = 0;
    }
    if((a1 % 2 == 0)&&(a2 % 2 == 0)&&(a1 == a2)) {
        A0[0] = 1;
        A0[1] = B0[1];
        A0[2] = B0[2];
    }
    if((a1 % 2 == 0)&&(a2 % 2 == 0)&&(a1 != a2)) {
        A0[0] = (a2-a1)/2+1;
        for(int j = 1; j <= 2*lround(A0[0]); j++) A0[j] = A0[a1+j-2];
        A0[1] = B0[1];
        A0[2*lround(A0[0])] = B0[2];
    }
    if((a1 % 2 == 0)&&(a2 % 2 == 1)) {
        A0[0] = (a2-a1+1)/2;
        for(int j = 1; j <= 2*lround(A0[0]); j++) A0[j] = A0[a1+j-2];
        A0[1] = B0[1];
    }
    if((a1 % 2 == 1)&&(a2 % 2 == 0)) {
        A0[0] = (a2-a1+1)/2;
        for(int j = 1; j <= 2*lround(A0[0]); j++) A0[j] = A0[a1+j-1];
        A0[2*lround(A0[0])] = B0[2];
    } //一个集合的相交运算
}

void Intersect(int n, double (*B)[1000], double A[5000]);
void Intersect(int n, double (*B)[1000], double A[5000])
{
    int a;
    double B1[3], B2[3], A1[5000], A2[5000];
    a = 1;
    for(int i = 0; i <= n; i++) {
        if(B[0][i] == 0) {
            A[0] = 0;
            a = 0;
        }
    }
    if(a != 0) {
        if(B[0][0] == 1) {
            A[0] = 1;
            A[1] = B[1][0];
            A[2] = B[2][0];
        }
        else {
            A[0] = 2;
            A[1] = B[1][0];
            A[2] = B[2][0];
            A[3] = B[3][0];
            A[4] = B[4][0];
        } //将B中的第一个集合放入A

        for(int i = 1; i <= n; i++) if(lround(A[0])) {
                if(B[0][i] == 1) {
                    for(int j = 1; j <= 2; j++) B1[j] = B[j][i];
                    Intersect1(B1, A);
                }
                else {
                    for(int j = 1; j <= 2; j++) {
                        B1[j] = B[j][i];
                        B2[j] = B[j+2][i];
                    }
                    for(int j = 0; j <= 2*lround(A[0]); j++) {
                        A1[j] = A[j];
                        A2[j] = A[j];
                    }
                    Intersect1(B1, A1);
                    Intersect1(B2, A2);
                    A[0] = A1[0]+A2[0];
                    if(lround(A1[0])) {for(int j = 1; j <= 2*lround(A1[0]); j++) A[j] = A1[j];}
                    if(lround(A2[0])) {for(int j = 1; j <= 2*lround(A2[0]); j++) A[j+2*lround(A1[0])] = A2[j];}
                }
            }
    }
}

void bana(int k2, double xm1, double xM1, double xm2, double xM2, double (*Banana)[1000]);
void bana(int k2, double xm1, double xM1, double xm2, double xM2, double (*Banana)[1000]) {
    int B;

    B = 2;

    if ((xm1 >= xM1) && (xm2 >= xM2)) {
        B = 0;
        Banana[0][k2] = B;
    }
    if ((xm1 >= xM1) && (xm2 < xM2)) {
        B = 1;
        Banana[0][k2] = B;
        Banana[1][k2] = xm2;
        Banana[2][k2] = xM2;
    }
    if ((xm1 < xM1) && (xm2 >= xM2)) {
        B = 1;
        Banana[0][k2] = B;
        Banana[1][k2] = xm1;
        Banana[2][k2] = xM1;
    }
    if ((xm1 > xM2) && (B == 2)) {
        Banana[0][k2] = B;
        Banana[1][k2] = xm2;
        Banana[2][k2] = xM2;
        Banana[3][k2] = xm1;
        Banana[4][k2] = xM1;
        B = 1;
    }
    if ((xm2 > xM1) && (B == 2)) {
        Banana[0][k2] = B;
        Banana[1][k2] = xm1;
        Banana[2][k2] = xM1;
        Banana[3][k2] = xm2;
        Banana[4][k2] = xM2;
        B = 1;
    }
    if ((xm1 <= xm2) && (xM1 <= xM2) && (B == 2)) {
        B = 1;
        Banana[0][k2] = B;
        Banana[1][k2] = xm1;
        Banana[2][k2] = xM2;
    }
    if ((xm1 <= xm2) && (xM1 >= xM2) && (B == 2)) {
        B = 1;
        Banana[0][k2] = B;
        Banana[1][k2] = xm1;
        Banana[2][k2] = xM1;
    }
    if ((xm1 >= xm2) && (xM1 <= xM2) && (B == 2)) {
        B = 1;
        Banana[0][k2] = B;
        Banana[1][k2] = xm2;
        Banana[2][k2] = xM2;
    }
    if ((xm1 >= xm2) && (xM1 >= xM2) && (B == 2)) {
        B = 1;
        Banana[0][k2] = B;
        Banana[1][k2] = xm2;
        Banana[2][k2] = xM1;
    }
}

#endif //A_2_H
