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


if __name__ == '__main__':
    
    x0 = np.array([0, 0])
    res = minimize(loglikelihood(14, 7, 5, 8), x0, method='nelder-mead', options={'xtol': 1e-8, 'disp': True})
    print(res.fun)
    print(res.x)

    # 初期ベクトル
    x0 = np.array([0, 0])
    # 乱数種
    seed = 282
    types = [0.09+0.6*0.55, 0.15**2+0.3*0.55, 0.55**2, 2*0.3*0.15]
    print(types)
    print(np.sum(types))
    type_sample = np.random.RandomState(seed).multinomial(34, types, size=500)

    mle = np.zeros((2, 500), dtype=float)
    hesse = np.zeros((2, 2), dtype=float)
    hesse_nan = 0

    for i, t in enumerate(type_sample):
        res = minimize(loglikelihood(t[0], t[1], t[2], t[3]), x0, method='nelder-mead', options={'xtol': 1e-8, 'disp': False})
        h = hessian(res.x, loglikelihood(t[0], t[1], t[2], t[3]))
        if np.isnan(h[0, 0]):
            hesse_nan += 1
        else:
            hesse += h
        
        mle[0, i] = res.x[0]
        mle[1, i] = res.x[1]


    alpha_mean = mle[0].mean()
    beta_mean = mle[1].mean()

    print("average of alpha:", alpha_mean, "average of beta:", beta_mean)
    print("var-cov matrix:", np.cov(mle))
    print(hesse_nan)
    print("hessian-ave:", hesse/(500-hesse_nan))
    print(np.linalg.inv(hesse/(500-hesse_nan)))

