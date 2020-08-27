#ifndef GEQU_ROOT_H
#define GEQU_ROOT_H

#include "core_global.h"
/*************************************************
Function: solve_quadratic_equation
Description: 求一元二次方程(a*x^2 + b*x + c = 0)的所有实数根
Input: 方程的系数 p = {a, b, c}
Output: 方程的所有实数根x
Return: 实数根的个数
*************************************************/
int CORE_EXPORT solve_quadratic_equation(double p[], double x[]);

/*************************************************
Function: solve_cubic_equation
Description: 盛金公式求一元三次方程(a*x^3 + b*x^2 + c*x + d = 0)的所有实数根
             A = b * b - 3.0 * a * c;
             B = b * c - 9.0 * a * d;
             C = c * c - 3.0 * b * d;
             (1)当A = B = 0时，方程有一个三重实根
             (2)当Δ = B^2－4AC>0时，方程有一个实根和一对共轭虚根
             (3)当Δ = B^2－4AC = 0时，方程有三个实根，其中有一个两重根
             (4)当Δ = B^2－4AC<0时，方程有三个不相等的实根
Input: 方程的系数 p = {a, b, c, d}
Output: 方程的所有实数根x
Return: 实数根的个数
*************************************************/
int CORE_EXPORT solve_cubic_equation(double p[], double x[]);

// 牛顿迭代法求解一元三次方程
double CORE_EXPORT newton_solve_cubic_equation(double p[4], double r, double errLimit = 1e-3, int itCount = 32);

/*************************************************
Function: solve_quartic_equation
Description: 费拉里法求一元四次方程(a*x^4 + b*x^3 + c*x^2 + d*x + e = 0)的所有实数根
Input: 方程的系数 p = {a, b, c, d, e}
Output: 方程的所有实数根x
Return: 实数根的个数
*************************************************/
int CORE_EXPORT solve_quartic_equation(double p[], double x[]);

// 牛顿迭代法求解一元四次方程
double CORE_EXPORT newton_solve_quartic_equation(double p[5], double r, double errLimit = 1e-3, int itCount = 32);

#endif
