#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cfloat>
#include "intersecto.h"
#include <time.h>

using namespace std;

int main( ) {

    clock_t start,end;
    start = clock();
    ofstream outU("data1pi2.txt");
    outU.setf(ios::scientific);
    outU.setf(ios::right);
    outU.precision(10);
    outU.fill(' ');

    const int n = 100;
    const int O = 1;
    const int O1 = 1000;
    const int O2 = 5000;
    const int n_area = 1e5;
    int B, n1, n2, nc2, nn;
    n1 = 0;
    n2 = 0;
    nc2 = 0;
    double KK, S, a, q, c, m, Area, U, (*XX)[n_area], h, hmin, hmax, alpha1, alpha2, hd, ds, ex, pi, K1[n], K2[n], K3[n], K4[n], (*Y1)[n], (*Y2)[n], (*Y3)[n], (*Y4)[n],
            (*xx)[n], (*yy)[n], (*x)[n], (*y)[n], Dxx, Dyy, (*Dx)[n], (*Dy)[n], *H, DHH, HH, *DH, xr1[2][4], xr2[2][4], xr3[2][4], xr4[2][4], xr5[2][4], xr6[2][4],
            xm1, xM1, xm2, xM2, (*Banana)[O1], *A, l1, l2, L1, L2, L3, L4, L5, L6, b1, b2, b3, b4, b5, b6, Fx[n], Fy[n], Fxx[n], Fyy[n],
            FH, ds1, t, R, RR;
    /*
        S: 沙发夹角
        n: 刚架个数
        n1: 时间
        nn: 帧数
        q: 压强大小
        c: 阻尼大小
        m: 刚架质量
        h: 步长
        hd: 输出步长
        ds: 位移
        ex: 位移的限制
        pi: π
        K1234: 刚架边斜率
        Y1234: 刚架边截距
        x,y: 刚架基准点坐标
        s: 刚架位移
        Ds: 刚架速度
        H: 直刚架基准点坐标
        DH: 直刚架速度
        xr1234: 四种边的单向范围
        xr56: 直边的单向范围
        Banana: 存放待相交集合
        A: 存放相交后集合
        L123456: x范围
        b123456: 暴露边长
        R: 力的范数
    */
    XX = new double[5][n_area] ();
    Y1 = new double[O][n] ();
    Y2 = new double[O][n] ();
    Y3 = new double[O][n] ();
    Y4 = new double[O][n] ();
    x = new double[O][n] ();
    y = new double[O][n] ();
    xx = new double[O][n] ();
    yy = new double[O][n] ();
    Dx = new double[O][n] ();
    Dy = new double[O][n] ();
    H = new double[O] ();
    DH = new double[O] ();
    Banana = new double[5][O1] ();
    A = new double[O2] ();

    pi = 4*atan(1);
    S = 1*pi/2;
    ex = 0.01;
    ds = 1;
    ds1 = 1;
    h = 0.001;
    hmin = 1e-4;
    hmax = 0.1;
    alpha1 = 0.5;
    alpha2 = 1.1;
    q = 100;
    c = 400;
    m = 0;
    t = 0;
    Area = 0;
    RR = DBL_MAX;
    R = 0;

    for(int i = 0; i < n; i++) {
        x[0][i] = 0; y[0][i] = 0;
        Dx[0][i] = 0; Dy[0][i] = 0;
    }
    H[0] = 0; DH[0] = 0;

/*
    ifstream Initial_data;
    Initial_data.open("data-1pi2-100.txt", ios::in);
    if (Initial_data.is_open() == false) cout << "cannot open the file" << endl;
    //打开初始数据
    const int n_Initial = 200;
    double *x_Initial, *y_Initial, *s_Initial;
    x_Initial = new double[n_Initial+2] ();
    y_Initial = new double[n_Initial+2] ();
    s_Initial = new double[n_Initial+2] ();

    Initial_data >> S >> nn >> nn;

    for (int i = 0; i < nn; i++) {
        for (int j = 1; j <= n_Initial; j++) {
            Initial_data >> x_Initial[j] >> y_Initial[j] >> n1;
        }
        Initial_data >> H[0] >> KK >> n1;
    }

    y_Initial[0] = H[0]; x_Initial[0] = x_Initial[1]-(y_Initial[1]-y_Initial[0])*(x_Initial[1]-x_Initial[2])/(y_Initial[1]-y_Initial[2]);
    x_Initial[n_Initial+1] = -x_Initial[0]; y_Initial[n_Initial+1] = y_Initial[0];

    for (int i = 0; i < n_Initial+2; i++) s_Initial[i] = S/2+i*(pi-S)/(n_Initial+1);

    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n_Initial+2; j++) {
            if ((S/2+(i+1)*(pi-S)/(n+1) >= s_Initial[j])&&(S/2+(i+1)*(pi-S)/(n+1) < s_Initial[j+1])) {
                x[0][i] = x_Initial[j] + (x_Initial[j+1]-x_Initial[j])*(S/2+(i+1)*(pi-S)/(n+1)-s_Initial[j])/(s_Initial[j+1]-s_Initial[j]);
                y[0][i] = y_Initial[j] + (y_Initial[j+1]-y_Initial[j])*(S/2+(i+1)*(pi-S)/(n+1)-s_Initial[j])/(s_Initial[j+1]-s_Initial[j]);
            }
        }
        Dx[0][i] = 0; Dy[0][i] = 0;
    }
    DH[0] = 0;
*/
    for(int i = 1; i <= n; i++) {
        K1[i-1] = tan(i*(pi-S)/(n+1));
        K2[i-1] = tan(i*(pi-S)/(n+1));
        K3[i-1] = tan(S+i*(pi-S)/(n+1));
        K4[i-1] = tan(S+i*(pi-S)/(n+1));
    } //生成斜率

    while(RR >= ex) {

        U = 0;
        if (((RR > R) && (n2 != 0)) || (h == hmin)) RR = R;
        R = 0;

        for (int k = 0; k < n; k++) {
            Y1[n1][k] = y[n1][k] - x[n1][k] * K1[k];
            Y2[n1][k] = y[n1][k] - x[n1][k] * K2[k] + (sin(S / 2 + (k + 1) * (pi - S) / (n + 1)) -
                                                       K2[k] * cos(S / 2 + (k + 1) * (pi - S) / (n + 1))) /
                                                      sin(S / 2);
            Y3[n1][k] = y[n1][k] - x[n1][k] * K3[k] + (sin(S / 2 + (k + 1) * (pi - S) / (n + 1)) -
                                                       K3[k] * cos(S / 2 + (k + 1) * (pi - S) / (n + 1))) /
                                                      sin(S / 2);
            Y4[n1][k] = y[n1][k] - x[n1][k] * K4[k];
        } //生成截距

        if (1 == 1) {
            for (int k1 = 0; k1 < n / 2; k1++) {

                for (int k2 = 0; k2 < n; k2++) {

                    if (k1 == k2) {
                        Banana[0][k2] = 1;
                        Banana[1][k2] = -DBL_MAX;
                        Banana[2][k2] = DBL_MAX;
                        continue;
                    }

                    if ((K1[k1] - K1[k2]) * K1[k2] > 0) {
                        xr1[0][0] = 0;
                        xr1[1][0] = (Y1[n1][k2] - Y1[n1][k1]) / (K1[k1] - K1[k2]);
                    } else {
                        xr1[0][0] = 1;
                        xr1[1][0] = (Y1[n1][k2] - Y1[n1][k1]) / (K1[k1] - K1[k2]);
                    }

                    if ((K1[k1] - K2[k2]) * K2[k2] > 0) {
                        xr1[0][1] = 1;
                        xr1[1][1] = (Y2[n1][k2] - Y1[n1][k1]) / (K1[k1] - K2[k2]);
                    } else {
                        xr1[0][1] = 0;
                        xr1[1][1] = (Y2[n1][k2] - Y1[n1][k1]) / (K1[k1] - K2[k2]);
                    }

                    if ((K1[k1] - K3[k2]) * K3[k2] > 0) {
                        xr1[0][2] = 0;
                        xr1[1][2] = (Y3[n1][k2] - Y1[n1][k1]) / (K1[k1] - K3[k2]);
                    } else {
                        xr1[0][2] = 1;
                        xr1[1][2] = (Y3[n1][k2] - Y1[n1][k1]) / (K1[k1] - K3[k2]);
                    }

                    if ((K1[k1] - K4[k2]) * K4[k2] > 0) {
                        xr1[0][3] = 1;
                        xr1[1][3] = (Y4[n1][k2] - Y1[n1][k1]) / (K1[k1] - K4[k2]);
                    } else {
                        xr1[0][3] = 0;
                        xr1[1][3] = (Y4[n1][k2] - Y1[n1][k1]) / (K1[k1] - K4[k2]);
                    } //计算相对四种边的单向范围

                    xm1 = -DBL_MAX;
                    xM1 = DBL_MAX;
                    xm2 = -DBL_MAX;
                    xM2 = DBL_MAX;

                    for (int i = 0; i < 3; i++) {
                        if ((xm1 < xr1[1][i]) && (xr1[0][i] == 0)) { xm1 = xr1[1][i]; }
                        if ((xM1 > xr1[1][i]) && (xr1[0][i] == 1)) { xM1 = xr1[1][i]; }
                        if ((xm2 < xr1[1][i + 1]) && (xr1[0][i + 1] == 0)) { xm2 = xr1[1][i + 1]; }
                        if ((xM2 > xr1[1][i + 1]) && (xr1[0][i + 1] == 1)) { xM2 = xr1[1][i + 1]; }
                    }

                    if (K1[k1] > 0) {
                        if (xM1 > x[n1][k1]) {
                            xM1 = x[n1][k1];
                        }
                        if (xM2 > x[n1][k1]) {
                            xM2 = x[n1][k1];
                        }
                    }
                    if (K1[k1] < 0) {
                        if (xm1 < x[n1][k1]) {
                            xm1 = x[n1][k1];
                        }
                        if (xm2 < x[n1][k1]) {
                            xm2 = x[n1][k1];
                        }
                    } //计算两个范围

                    bana(k2, xm1, xM1, xm2, xM2, Banana);

                }

                Banana[0][n] = 1;
                if (K1[k1] > 0) {
                    Banana[1][n] = (H[n1] - Y1[n1][k1]) / K1[k1];
                    Banana[2][n] = (H[n1] + 1 - Y1[n1][k1]) / K1[k1];
                } else {
                    Banana[1][n] = (H[n1] + 1 - Y1[n1][k1]) / K1[k1];
                    Banana[2][n] = (H[n1] - Y1[n1][k1]) / K1[k1];
                } //生成相交表

                Intersect(n, Banana, A); //相交

                if (A[0] == 0) L1 = 0;
                else {
                    L1 = 0;
                    for (int i = 1; i <= lround(A[0]); i++) L1 = L1 + A[2 * i] - A[2 * i - 1];
                }
                b1 = L1 * sqrt(1 + K1[k1] * K1[k1]); //计算内部边长

                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////第一根

                for (int k2 = 0; k2 < n; k2++) {

                    if (k1 == k2) {
                        Banana[0][k2] = 1;
                        Banana[1][k2] = -DBL_MAX;
                        Banana[2][k2] = DBL_MAX;
                        continue;
                    }

                    if ((K2[k1] - K1[k2]) * K1[k2] > 0) {
                        xr2[0][0] = 0;
                        xr2[1][0] = (Y1[n1][k2] - Y2[n1][k1]) / (K2[k1] - K1[k2]);
                    } else {
                        xr2[0][0] = 1;
                        xr2[1][0] = (Y1[n1][k2] - Y2[n1][k1]) / (K2[k1] - K1[k2]);
                    }

                    if ((K2[k1] - K2[k2]) * K2[k2] > 0) {
                        xr2[0][1] = 1;
                        xr2[1][1] = (Y2[n1][k2] - Y2[n1][k1]) / (K2[k1] - K2[k2]);
                    } else {
                        xr2[0][1] = 0;
                        xr2[1][1] = (Y2[n1][k2] - Y2[n1][k1]) / (K2[k1] - K2[k2]);
                    }

                    if ((K2[k1] - K3[k2]) * K3[k2] > 0) {
                        xr2[0][2] = 0;
                        xr2[1][2] = (Y3[n1][k2] - Y2[n1][k1]) / (K2[k1] - K3[k2]);
                    } else {
                        xr2[0][2] = 1;
                        xr2[1][2] = (Y3[n1][k2] - Y2[n1][k1]) / (K2[k1] - K3[k2]);
                    }

                    if ((K2[k1] - K4[k2]) * K4[k2] > 0) {
                        xr2[0][3] = 1;
                        xr2[1][3] = (Y4[n1][k2] - Y2[n1][k1]) / (K2[k1] - K4[k2]);
                    } else {
                        xr2[0][3] = 0;
                        xr2[1][3] = (Y4[n1][k2] - Y2[n1][k1]) / (K2[k1] - K4[k2]);
                    } //计算相对四种边的单向范围

                    xm1 = -DBL_MAX;
                    xM1 = DBL_MAX;
                    xm2 = -DBL_MAX;
                    xM2 = DBL_MAX;

                    for (int i = 0; i < 3; i++) {
                        if ((xm1 < xr2[1][i]) && (xr2[0][i] == 0)) { xm1 = xr2[1][i]; }
                        if ((xM1 > xr2[1][i]) && (xr2[0][i] == 1)) { xM1 = xr2[1][i]; }
                        if ((xm2 < xr2[1][i + 1]) && (xr2[0][i + 1] == 0)) { xm2 = xr2[1][i + 1]; }
                        if ((xM2 > xr2[1][i + 1]) && (xr2[0][i + 1] == 1)) { xM2 = xr2[1][i + 1]; }
                    }

                    if (K2[k1] > 0) {
                        if (xM1 > x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2)) {
                            xM1 = x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2);
                        }
                        if (xM2 > x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2)) {
                            xM2 = x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2);
                        }
                    }
                    if (K2[k1] < 0) {
                        if (xm1 < x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2)) {
                            xm1 = x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2);
                        }
                        if (xm2 < x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2)) {
                            xm2 = x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2);
                        }
                    } //计算两个范围

                    bana(k2, xm1, xM1, xm2, xM2, Banana);
                }

                Banana[0][n] = 1;
                if (K2[k1] > 0) {
                    Banana[1][n] = (H[n1] - Y2[n1][k1]) / K2[k1];
                    Banana[2][n] = (H[n1] + 1 - Y2[n1][k1]) / K2[k1];
                } else {
                    Banana[1][n] = (H[n1] + 1 - Y2[n1][k1]) / K2[k1];
                    Banana[2][n] = (H[n1] - Y2[n1][k1]) / K2[k1];
                } //生成相交表

                Intersect(n, Banana, A); //相交

                if (A[0] == 0) L2 = 0;
                else {
                    L2 = 0;
                    for (int i = 1; i <= lround(A[0]); i++) L2 = L2 + A[2 * i] - A[2 * i - 1];
                }
                b2 = L2 * sqrt(1 + K2[k1] * K2[k1]); //计算内部边长

                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////第二根

                for (int k2 = 0; k2 < n; k2++) {

                    if (k1 == k2) {
                        Banana[0][k2] = 1;
                        Banana[1][k2] = -DBL_MAX;
                        Banana[2][k2] = DBL_MAX;
                        continue;
                    }

                    if ((K3[k1] - K1[k2]) * K1[k2] > 0) {
                        xr3[0][0] = 0;
                        xr3[1][0] = (Y1[n1][k2] - Y3[n1][k1]) / (K3[k1] - K1[k2]);
                    } else {
                        xr3[0][0] = 1;
                        xr3[1][0] = (Y1[n1][k2] - Y3[n1][k1]) / (K3[k1] - K1[k2]);
                    }

                    if ((K3[k1] - K2[k2]) * K2[k2] > 0) {
                        xr3[0][1] = 1;
                        xr3[1][1] = (Y2[n1][k2] - Y3[n1][k1]) / (K3[k1] - K2[k2]);
                    } else {
                        xr3[0][1] = 0;
                        xr3[1][1] = (Y2[n1][k2] - Y3[n1][k1]) / (K3[k1] - K2[k2]);
                    }

                    if ((K3[k1] - K3[k2]) * K3[k2] > 0) {
                        xr3[0][2] = 0;
                        xr3[1][2] = (Y3[n1][k2] - Y3[n1][k1]) / (K3[k1] - K3[k2]);
                    } else {
                        xr3[0][2] = 1;
                        xr3[1][2] = (Y3[n1][k2] - Y3[n1][k1]) / (K3[k1] - K3[k2]);
                    }

                    if ((K3[k1] - K4[k2]) * K4[k2] > 0) {
                        xr3[0][3] = 1;
                        xr3[1][3] = (Y4[n1][k2] - Y3[n1][k1]) / (K3[k1] - K4[k2]);
                    } else {
                        xr3[0][3] = 0;
                        xr3[1][3] = (Y4[n1][k2] - Y3[n1][k1]) / (K3[k1] - K4[k2]);
                    } //计算相对四种边的单向范围

                    xm1 = -DBL_MAX;
                    xM1 = DBL_MAX;
                    xm2 = -DBL_MAX;
                    xM2 = DBL_MAX;

                    for (int i = 0; i < 3; i++) {
                        if ((xm1 < xr3[1][i]) && (xr3[0][i] == 0)) { xm1 = xr3[1][i]; }
                        if ((xM1 > xr3[1][i]) && (xr3[0][i] == 1)) { xM1 = xr3[1][i]; }
                        if ((xm2 < xr3[1][i + 1]) && (xr3[0][i + 1] == 0)) { xm2 = xr3[1][i + 1]; }
                        if ((xM2 > xr3[1][i + 1]) && (xr3[0][i + 1] == 1)) { xM2 = xr3[1][i + 1]; }
                    }

                    if (K3[k1] < 0) {
                        if (xm1 < x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2)) {
                            xm1 = x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2);
                        }
                        if (xm2 < x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2)) {
                            xm2 = x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2);
                        }
                    }
                    if (K3[k1] > 0) {
                        if (xM1 > x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2)) {
                            xM1 = x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2);
                        }
                        if (xM2 > x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2)) {
                            xM2 = x[n1][k1] + (cos(S / 2 + (k1 + 1) * (pi - S) / (n + 1))) / sin(S / 2);
                        }
                    } //计算两个范围

                    bana(k2, xm1, xM1, xm2, xM2, Banana);
                }

                Banana[0][n] = 1;
                if (K3[k1] > 0) {
                    Banana[1][n] = (H[n1] - Y3[n1][k1]) / K3[k1];
                    Banana[2][n] = (H[n1] + 1 - Y3[n1][k1]) / K3[k1];
                } else {
                    Banana[1][n] = (H[n1] + 1 - Y3[n1][k1]) / K3[k1];
                    Banana[2][n] = (H[n1] - Y3[n1][k1]) / K3[k1];
                } //生成相交表

                Intersect(n, Banana, A); //相交

                if (A[0] == 0) L3 = 0;
                else {
                    L3 = 0;
                    for (int i = 1; i <= lround(A[0]); i++) L3 = L3 + A[2 * i] - A[2 * i - 1];
                }
                b3 = L3 * sqrt(1 + K3[k1] * K3[k1]); //计算内部边长

                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////第三根

                for (int k2 = 0; k2 < n; k2++) {

                    if (k1 == k2) {
                        Banana[0][k2] = 1;
                        Banana[1][k2] = -DBL_MAX;
                        Banana[2][k2] = DBL_MAX;
                        continue;
                    }

                    if ((K4[k1] - K1[k2]) * K1[k2] > 0) {
                        xr4[0][0] = 0;
                        xr4[1][0] = (Y1[n1][k2] - Y4[n1][k1]) / (K4[k1] - K1[k2]);
                    } else {
                        xr4[0][0] = 1;
                        xr4[1][0] = (Y1[n1][k2] - Y4[n1][k1]) / (K4[k1] - K1[k2]);
                    }

                    if ((K4[k1] - K2[k2]) * K2[k2] > 0) {
                        xr4[0][1] = 1;
                        xr4[1][1] = (Y2[n1][k2] - Y4[n1][k1]) / (K4[k1] - K2[k2]);
                    } else {
                        xr4[0][1] = 0;
                        xr4[1][1] = (Y2[n1][k2] - Y4[n1][k1]) / (K4[k1] - K2[k2]);
                    }

                    if ((K4[k1] - K3[k2]) * K3[k2] > 0) {
                        xr4[0][2] = 0;
                        xr4[1][2] = (Y3[n1][k2] - Y4[n1][k1]) / (K4[k1] - K3[k2]);
                    } else {
                        xr4[0][2] = 1;
                        xr4[1][2] = (Y3[n1][k2] - Y4[n1][k1]) / (K4[k1] - K3[k2]);
                    }

                    if ((K4[k1] - K4[k2]) * K4[k2] > 0) {
                        xr4[0][3] = 1;
                        xr4[1][3] = (Y4[n1][k2] - Y4[n1][k1]) / (K4[k1] - K4[k2]);
                    } else {
                        xr4[0][3] = 0;
                        xr4[1][3] = (Y4[n1][k2] - Y4[n1][k1]) / (K4[k1] - K4[k2]);
                    } //计算相对四种边的单向范围

                    xm1 = -DBL_MAX;
                    xM1 = DBL_MAX;
                    xm2 = -DBL_MAX;
                    xM2 = DBL_MAX;

                    for (int i = 0; i < 3; i++) {
                        if ((xm1 < xr4[1][i]) && (xr4[0][i] == 0)) { xm1 = xr4[1][i]; }
                        if ((xM1 > xr4[1][i]) && (xr4[0][i] == 1)) { xM1 = xr4[1][i]; }
                        if ((xm2 < xr4[1][i + 1]) && (xr4[0][i + 1] == 0)) { xm2 = xr4[1][i + 1]; }
                        if ((xM2 > xr4[1][i + 1]) && (xr4[0][i + 1] == 1)) { xM2 = xr4[1][i + 1]; }
                    }

                    if (K4[k1] < 0) {
                        if (xm1 < x[n1][k1]) {
                            xm1 = x[n1][k1];
                        }
                        if (xm2 < x[n1][k1]) {
                            xm2 = x[n1][k1];
                        }
                    }
                    if (K4[k1] > 0) {
                        if (xM1 > x[n1][k1]) {
                            xM1 = x[n1][k1];
                        }
                        if (xM2 > x[n1][k1]) {
                            xM2 = x[n1][k1];
                        }
                    } //计算两个范围

                    bana(k2, xm1, xM1, xm2, xM2, Banana);
                }

                Banana[0][n] = 1;
                if (K4[k1] > 0) {
                    Banana[1][n] = (H[n1] - Y4[n1][k1]) / K4[k1];
                    Banana[2][n] = (H[n1] + 1 - Y4[n1][k1]) / K4[k1];
                } else {
                    Banana[1][n] = (H[n1] + 1 - Y4[n1][k1]) / K4[k1];
                    Banana[2][n] = (H[n1] - Y4[n1][k1]) / K4[k1];
                } //生成相交表

                Intersect(n, Banana, A); //相交

                if (A[0] == 0) L4 = 0;
                else {
                    L4 = 0;
                    for (int i = 1; i <= lround(A[0]); i++) L4 = L4 + A[2 * i] - A[2 * i - 1];
                }
                b4 = L4 * sqrt(1 + K4[k1] * K4[k1]); //计算内部边长

                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////第四根

                Fxx[k1] = Fx[k1];
                Fyy[k1] = Fy[k1];

                Fx[k1] = q * (b1 - b2 + b3 - b4) * cos(S / 2) * cos((S - pi) / 2 + (k1 + 1) * (pi - S) / (n + 1))
                     - q * (-b1 + b2 + b3 - b4) * sin(S / 2) * sin((S - pi) / 2 + (k1 + 1) * (pi - S) / (n + 1));
                Fy[k1] = q * (b1 - b2 + b3 - b4) * cos(S / 2) * sin((S - pi) / 2 + (k1 + 1) * (pi - S) / (n + 1))
                     + q * (-b1 + b2 + b3 - b4) * sin(S / 2) * cos((S - pi) / 2 + (k1 + 1) * (pi - S) / (n + 1));

                R += Fx[k1]*Fx[k1]+Fy[k1]*Fy[k1];
            }

            R = sqrt(2*R);

            if (RR > R) {
                n2 = n2 + 1;
                t = t + h;
                ds = ds1;
                ds1 = 0;
                if (n2 - nc2 == 50) {
                    nc2 = n2;
                    cout << n2 << ' ' << ds << ' ' << RR << endl;
                }
                outU << setw(20) << n2 << setw(20) << h << setw(20) << R << "\n";
                h = min(h * alpha2, hmax);
                //cout << 22222222 << endl;
            } else if ((h == hmin) && (RR <= R)) {
                n2 = n2 + 1;
                t = t + h;
                ds = ds1;
                ds1 = 0;
                if (n2 - nc2 == 50) {
                    nc2 = n2;
                    cout << n2 << ' ' << ds << ' ' << RR << endl;
                }
                outU << setw(20) << n2 << setw(20) << h << setw(20) << R << "\n";
                //cout << 333333333 << endl;
            } else {
                h = max(h*alpha1,hmin);
                for (int k1 = 0; k1 < n; k1++) {
                    x[n1][k1] = xx[n1][k1];
                    y[n1][k1] = yy[n1][k1];
                    Fx[k1] = Fxx[k1];
                    Fy[k1] = Fyy[k1];
                }
                //cout << 111111111 <<endl;
            }

            for (int k1 = 0; k1 < n / 2; k1++) {

                Dx[n1][k1] = Fx[k1] / c;
                Dy[n1][k1] = Fy[k1] / c;

                xx[n1][k1] = x[n1][k1];
                yy[n1][k1] = y[n1][k1];

                x[n1][k1] = x[n1][k1] + h * Dx[n1][k1];
                y[n1][k1] = y[n1][k1] + h * Dy[n1][k1];

                if (ds1 < abs(xx[n1][k1] - x[n1][k1]) + abs(yy[n1][k1] - y[n1][k1]))
                    ds1 = abs(xx[n1][k1] - x[n1][k1]) + abs(yy[n1][k1] - y[n1][k1]);

            }

            for (int k1 = n / 2; k1 < n; k1++) {
                x[n1][k1] = -x[n1][n - k1 - 1];
                y[n1][k1] = y[n1][n - k1 - 1];
                xx[n1][k1] = -xx[n1][n - k1 - 1];
                yy[n1][k1] = yy[n1][n - k1 - 1];
            }

            H[n1] = H[n1];

        }

        //cout << RR << ' ' << R << ' ' << h << endl;

        //if(n2 >= 20000) break;

    }

    if(1 == 1) {
        for (int k = 0; k < n; k++) {
            Y1[n1][k] = y[n1][k] - x[n1][k] * K1[k];
            Y2[n1][k] = y[n1][k] - x[n1][k] * K2[k] +
                        (sin(S / 2 + (k + 1) * (pi - S) / (n + 1)) - K2[k] * cos(S / 2 + (k + 1) * (pi - S) / (n + 1))) /
                        sin(S / 2);
            Y3[n1][k] = y[n1][k] - x[n1][k] * K3[k] +
                        (sin(S / 2 + (k + 1) * (pi - S) / (n + 1)) - K3[k] * cos(S / 2 + (k + 1) * (pi - S) / (n + 1))) /
                        sin(S / 2);
            Y4[n1][k] = y[n1][k] - x[n1][k] * K4[k];
        } //生成截距

        for (int ii = 0; ii <= n_area; ii++) {

            for (int k2 = 0; k2 < n; k2++) {

                if ((0 - K1[k2]) * K1[k2] > 0) {
                    xr5[0][0] = 0;
                    xr5[1][0] = (Y1[n1][k2] - H[n1] - ii * 1.0 / n_area) / (0 - K1[k2]);
                } else {
                    xr5[0][0] = 1;
                    xr5[1][0] = (Y1[n1][k2] - H[n1] - ii * 1.0 / n_area) / (0 - K1[k2]);
                }

                if ((0 - K2[k2]) * K2[k2] > 0) {
                    xr5[0][1] = 1;
                    xr5[1][1] = (Y2[n1][k2] - H[n1] - ii * 1.0 / n_area) / (0 - K2[k2]);
                } else {
                    xr5[0][1] = 0;
                    xr5[1][1] = (Y2[n1][k2] - H[n1] - ii * 1.0 / n_area) / (0 - K2[k2]);
                }

                if ((0 - K3[k2]) * K3[k2] > 0) {
                    xr5[0][2] = 0;
                    xr5[1][2] = (Y3[n1][k2] - H[n1] - ii * 1.0 / n_area) / (0 - K3[k2]);
                } else {
                    xr5[0][2] = 1;
                    xr5[1][2] = (Y3[n1][k2] - H[n1] - ii * 1.0 / n_area) / (0 - K3[k2]);
                }

                if ((0 - K4[k2]) * K4[k2] > 0) {
                    xr5[0][3] = 1;
                    xr5[1][3] = (Y4[n1][k2] - H[n1] - ii * 1.0 / n_area) / (0 - K4[k2]);
                } else {
                    xr5[0][3] = 0;
                    xr5[1][3] = (Y4[n1][k2] - H[n1] - ii * 1.0 / n_area) / (0 - K4[k2]);
                } //计算相对四种边的单向范围

                xm1 = -DBL_MAX;
                xM1 = DBL_MAX;
                xm2 = -DBL_MAX;
                xM2 = DBL_MAX;

                for (int i = 0; i < 3; i++) {
                    if ((xm1 < xr5[1][i]) && (xr5[0][i] == 0)) { xm1 = xr5[1][i]; }
                    if ((xM1 > xr5[1][i]) && (xr5[0][i] == 1)) { xM1 = xr5[1][i]; }
                    if ((xm2 < xr5[1][i + 1]) && (xr5[0][i + 1] == 0)) { xm2 = xr5[1][i + 1]; }
                    if ((xM2 > xr5[1][i + 1]) && (xr5[0][i + 1] == 1)) { xM2 = xr5[1][i + 1]; }
                }

                bana(k2, xm1, xM1, xm2, xM2, Banana);

            } //生成相交表

            Banana[0][n] = 1;
            Banana[1][n] = -DBL_MAX;
            Banana[2][n] = DBL_MAX;

            for (int i = 0; i < n/2; i++) {
                if ((H[n1] + ii * 1.0 / n_area >= y[n1][i])&&(H[n1] + ii * 1.0 / n_area < y[n1][i+1])) {

                    a = x[n1][i]+(x[n1][i+1]-x[n1][i])*((H[n1] + ii * 1.0 / n_area)-y[n1][i])/(y[n1][i+1]-y[n1][i]);

                    if (a > 0) {
                        Banana[0][n] = 2;
                        Banana[1][n] = -DBL_MAX;
                        Banana[2][n] = -a;
                        Banana[3][n] = a;
                        Banana[4][n] = DBL_MAX;
                    }
                }
            }

            Intersect(n, Banana, A); //相交

            if (A[0] == 0) l2 = 0;
            else {
                l2 = 0;
                for (int i = 1; i <= lround(A[0]); i++) l2 = l2 + A[2 * i] - A[2 * i - 1];
            }
            if (ii != 0) Area += (l1 + l2) / (2 * n_area);
            l1 = l2;
        }
    } // 计算面积

    end = clock();

    ofstream out1("data-1pi2-100.txt");
    out1.setf(ios::scientific);
    out1.setf(ios::right);
    out1.precision(10);
    out1.fill(' ');
    out1<<setw(20)<<S<<setw(20)<<n<<setw(20)<<1<<"\n";

    for (int i = 0; i < n; i++) out1 << setw(20) << x[n1][i] << setw(20) << y[n1][i] << setw(20) << 0 << "\n";
    out1 << setw(20) << H[n1] << setw(20) << H[n1] + 1 << setw(20) << 0 << "\n";

    cout << n2 << endl;

    for(int j = 0; j < n; j++) cout << x[n1][j] << ' ' << y[n1][j] << endl; cout << endl;

    cout << setprecision(30) << Area << endl;

    cout << pi << endl;

    cout<<"time = "<<double(end-start)/CLOCKS_PER_SEC<<"s"<<endl;

    delete[] XX;
    delete[] Y1;
    delete[] Y2;
    delete[] Y3;
    delete[] Y4;
    delete[] x;
    delete[] y;
    delete[] xx;
    delete[] yy;
    delete[] Dx;
    delete[] Dy;
    delete[] H;
    delete[] DH;
    delete[] Banana;
    delete[] A;
/*
    delete[] x_Initial;
    delete[] y_Initial;
    delete[] s_Initial;
*/
}