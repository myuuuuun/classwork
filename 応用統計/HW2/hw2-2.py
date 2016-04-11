#!/usr/bin/python
#-*- encoding: utf-8 -*-
"""
Copyright (c) 2015 @myuuuuun
Released under the MIT license.
"""
import math
import numpy as np
import pandas as pd
import functools
from scipy.optimize import minimize
import matplotlib.pyplot as plt
from scipy.optimize import approx_fprime
EPS = 1.0e-8
np.set_printoptions(precision=3)
np.set_printoptions(linewidth=400)
np.set_printoptions(threshold=np.nan)
pd.set_option('display.max_columns', 30)
pd.set_option('display.width', 400)


def loglikelihood(a, b, o, ab):
    def func(x):
        inner_a = x[0]**2 + 2*x[0]*(1-x[0]-x[1])
        inner_b = x[1]**2 + 2*x[1]*(1-x[0]-x[1])
        inner_o = (1-x[0]-x[1])**2
        inner_ab = 2*x[0]*x[1]
        return -1 * (a*np.log(inner_a) + b*np.log(inner_b) + o*np.log(inner_o) + ab*np.log(inner_ab))
    return func 


def hessian(x0, func):
    f1 = approx_fprime(x0, func, EPS) 
    n = x0.shape[0]
    hessian = np.zeros((n, n))
    xx = x0
    for j in range(n):
        xx0 = xx[j]
        xx[j] = xx0 + EPS
        f2 = approx_fprime( x0, func, EPS) 
        hessian[:, j] = (f2 - f1)/EPS
        xx[j] = xx0      
    return hessian


def chi_squared(a, b, o, ab):
    size = a+b+o+ab
    a_prob = 0.4
    o_prob = 0.3
    b_prob = 0.2
    ab_prob = 0.1
    return pow(a-size*a_prob, 2)/size*a_prob + pow(o-size*o_prob, 2)/size*o_prob \
        + pow(b-size*b_prob, 2)/size*b_prob + pow(ab-size*ab_prob, 2)/size*ab_prob


def likelihood_ratio(a, b, o, ab, seed=282):
    size = a+b+o+ab
    a_prob = 0.4
    o_prob = 0.3
    b_prob = 0.2
    ab_prob = 0.1
    x0 = np.array([0, 0])
    res1 = minimize(loglikelihood(a, b, o, ab), x0, method='nelder-mead', options={'xtol': 1e-8, 'disp': False})
    h_likelihood = res1.fun
    res2 = minimize(loglikelihood(a, b, o, ab), x0, method='nelder-mead', options={'xtol': 1e-8, 'disp': False})
    h0_likelihood = loglikelihood(a, b, o, ab)([0.253, 0.193])
    return -2 * (h_likelihood - h0_likelihood)


if __name__ == '__main__':
    """
    x0 = np.array([0, 0])
    res = minimize(loglikelihood(14, 7, 5, 8), x0, method='nelder-mead', options={'xtol': 1e-8, 'disp': False})
    print(res.x)
    """
    size = 34
    # 初期ベクトル
    x0 = np.array([0, 0])
    # 乱数種
    seed = 282

    # 帰無仮説でシミュレーション
    types = [0.4, 0.3, 0.2, 0.1]
    type_sample = np.random.RandomState(seed).multinomial(34, types, size=1000)
    print(type_sample[0].mean())

    # カイ二乗
    chi_stat = chi_squared(14, 7, 5, 8)
    """
    print(chi_stat)
    p = 0
    for i, t in enumerate(type_sample):
        print(t)
        if chi_squared(t[0], t[1], t[2], t[3]) > chi_stat:
            p+=1
    print(p / 1000)
    """

    # 尤度比
    like_stat = likelihood_ratio(14, 7, 5, 8)

    print(like_stat)



    """
    residual = np.array(residual);
    size = residual.shape[0]
    residual_norm = np.zeros(size, dtype=float)
    for i in range(size):
      residual_norm[i] = np.linalg.norm(residual[i])

    plt.title("Jacobi法, 反復毎の残差の推移")
    x = np.arange(size)
    plt.plot(x, residual_norm, color='green', label="√(残差2乗和)の推移")

    plt.xlabel("repeats")
    plt.ylabel("residual(2-norm)")
    plt.yscale('log')
    plt.legend()
    plt.show()
    """