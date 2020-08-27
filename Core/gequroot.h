#ifndef GEQU_ROOT_H
#define GEQU_ROOT_H

#include "core_global.h"
/*************************************************
Function: solve_quadratic_equation
Description: ��һԪ���η���(a*x^2 + b*x + c = 0)������ʵ����
Input: ���̵�ϵ�� p = {a, b, c}
Output: ���̵�����ʵ����x
Return: ʵ�����ĸ���
*************************************************/
int CORE_EXPORT solve_quadratic_equation(double p[], double x[]);

/*************************************************
Function: solve_cubic_equation
Description: ʢ��ʽ��һԪ���η���(a*x^3 + b*x^2 + c*x + d = 0)������ʵ����
             A = b * b - 3.0 * a * c;
             B = b * c - 9.0 * a * d;
             C = c * c - 3.0 * b * d;
             (1)��A = B = 0ʱ��������һ������ʵ��
             (2)���� = B^2��4AC>0ʱ��������һ��ʵ����һ�Թ������
             (3)���� = B^2��4AC = 0ʱ������������ʵ����������һ�����ظ�
             (4)���� = B^2��4AC<0ʱ����������������ȵ�ʵ��
Input: ���̵�ϵ�� p = {a, b, c, d}
Output: ���̵�����ʵ����x
Return: ʵ�����ĸ���
*************************************************/
int CORE_EXPORT solve_cubic_equation(double p[], double x[]);

// ţ�ٵ��������һԪ���η���
double CORE_EXPORT newton_solve_cubic_equation(double p[4], double r, double errLimit = 1e-3, int itCount = 32);

/*************************************************
Function: solve_quartic_equation
Description: �����﷨��һԪ�Ĵη���(a*x^4 + b*x^3 + c*x^2 + d*x + e = 0)������ʵ����
Input: ���̵�ϵ�� p = {a, b, c, d, e}
Output: ���̵�����ʵ����x
Return: ʵ�����ĸ���
*************************************************/
int CORE_EXPORT solve_quartic_equation(double p[], double x[]);

// ţ�ٵ��������һԪ�Ĵη���
double CORE_EXPORT newton_solve_quartic_equation(double p[5], double r, double errLimit = 1e-3, int itCount = 32);

#endif
