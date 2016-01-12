#-*- encoding: utf-8 -*-
"""
solve stochastic differential equations

Copyright (c) 2016 @myuuuuun
Released under the MIT license.
"""
import math
import numpy as np
import pandas as pd
import ode
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.cm as cm
np.set_printoptions(precision=3)
np.set_printoptions(linewidth=400)
pd.set_option('display.max_columns', 130)
pd.set_option('display.width', 800)
plt.rcParams['font.size'] = 13

# 日本語対応
mpl.rcParams['font.family'] = 'Osaka'


# Explicit Euler Method
# 陽的（前進）オイラー法
# 現状1階の問題のみ対応
def sde_euler(func, init, t_start, step, repeat, random_coef, random_process):
    dim = 1
    path = np.zeros((dim+1, repeat), dtype=float)
    path[0, 0] = t_start
    path[1, 0] = init

    for i in range(1, repeat):
        current = path[1, i-1]
        path[0, i] = t_start + i * step
        path[1, i] = current + func(current) * step + random_coef(current) * random_process(current, step)
 
    return path


# Modified Euler Method
# 修正オイラー法
# 現状1階の問題のみ対応
def sde_modified_euler(func, init, t_start, step, repeat, random_coef, random_process):
    dim = 1
    path = np.zeros((dim+1, repeat), dtype=float)
    path[0, 0] = t_start
    path[1, 0] = init
    k1 = 0
    k2 = 0

    for i in range(1, repeat):
        current = path[1, i-1]
        path[0, i] = t_start + i * step

        rp = random_process(current, step)
        # k1
        k1 = func(current) * step + random_coef(current) * rp
        # k2
        k2 = func(current + k1*step) * step + random_coef(current + k1*step) * rp
        
        path[1, i] = current + (k1 + k2) / 2

    return path


# Explicit RK4 Method
# 4段4次ルンゲ・クッタ
# 現状1階の問題のみ対応
def sde_runge_kutta(func, init, t_start, step, repeat, random_coef, random_process):
    dim = 1
    path = np.zeros((dim+1, repeat), dtype=float)
    path[0, 0] = t_start
    path[1, 0] = init
    k1, k2, k3, k4 = 0, 0, 0, 0

    for i in range(1, repeat):
        current = path[1, i-1]
        path[0, i] = t_start + i * step

        rp = random_process(current, step)
        # k1
        k1 = func(current) * step + random_coef(current) * rp
        # k2
        k2 = func(current + k1*step*0.5) * step + random_coef(current + k1*step*0.5) * rp
        # k3
        k3 = func(current + k2*step*0.5) * step + random_coef(current + k2*step*0.5) * rp
        # k4
        k4 = func(current + k3*step) * step + random_coef(current + k3*step) * rp
        
        path[1:, i] = current + (k1 + 2*k2 + 2*k3 + k4) / 6

    return path


if __name__ == '__main__':
    """
    Sample: solve x'(t) = x(2-x) dt + x dW(t) where W(t) is Wiener process

    analytic solution of
        x'(t) = x(2-x) dt, x(0) = 1
    is x(t) = 2*exp(2t) / (1 + exp(2t))
    """

    func = lambda x: 2*x - x**2
    init = 1
    t_start = 0
    step = 0.01
    repeat = 200
    seed = 198

    ts = np.arange(t_start, step*repeat, step)
    analytic_path = 2*np.exp(2*ts) / (1 + np.exp(2*ts))
    approx_path = ode.euler(func, init, t_start, step, repeat)

    sample_size = 100

    # Euler Method
    rs = np.random.RandomState(seed)
    random_coef = lambda x: x
    random_process = lambda x, step: rs.normal(loc=0, scale=math.sqrt(step))

    euler_path = np.zeros((sample_size, repeat))
    for i in range(sample_size):
        euler_path[i] = sde_euler(func, init, t_start, step, repeat, random_coef, random_process)[1]
    
    euler_average = np.zeros(repeat)
    for i in range(repeat):
        euler_average[i] = euler_path[:, i].mean()

    # Modified Euler Method
    rs = np.random.RandomState(seed)
    random_coef = lambda x: x
    random_process = lambda x, step: rs.normal(loc=0, scale=math.sqrt(step))

    modified_euler_path = np.zeros((sample_size, repeat))
    for i in range(sample_size):
        modified_euler_path[i] = sde_modified_euler(func, init, t_start, step, repeat, random_coef, random_process)[1]
    
    modified_euler_average = np.zeros(repeat)
    for i in range(repeat):
        modified_euler_average[i] = modified_euler_path[:, i].mean()

    # RK4 Method
    rs = np.random.RandomState(seed)
    random_coef = lambda x: x
    random_process = lambda x, step: rs.normal(loc=0, scale=math.sqrt(step))

    rk4_path = np.zeros((sample_size, repeat))
    for i in range(sample_size):
        rk4_path[i] = sde_runge_kutta(func, init, t_start, step, repeat, random_coef, random_process)[1]
    
    rk4_average = np.zeros(repeat)
    for i in range(repeat):
        rk4_average[i] = rk4_path[:, i].mean()

    fig, ax = plt.subplots(figsize=(16, 8))
    plt.title(r"SDE: $x'(t) = x(2-x) dt + x dW(t),\ x(0)=1,\ \Delta t = {0}$".format(step))
    plt.xlabel("t")
    plt.ylabel("x")

    for i in range(sample_size):
        plt.plot(ts, rk4_path[i], color='#cccccc', linewidth=1)

    plt.plot(approx_path[0], approx_path[1], color='green', linewidth=3, label=r"Euler approx($x'(t) = x(2-x) dt$)")
    plt.plot(ts, analytic_path, color='orange', linewidth=1, label=r"Analytic sol($x(t) = 2e^{2t} / (1+e^{2t}})$)")
    plt.plot(ts, euler_average, color='red', linewidth=2, label="Euler mean({0}times)".format(sample_size))
    plt.plot(ts, modified_euler_average, color='purple', linewidth=2, label=r"Modified Euler mean({0}times)".format(sample_size))
    plt.plot(ts, rk4_average, color='black', linewidth=2, label=r"RK4 mean({0}times)".format(sample_size))

    plt.ylim(0, 8)
    plt.legend()
    plt.show()




