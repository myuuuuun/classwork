#-*- encoding: utf-8 -*-
"""
solve ordinary differential equations

Copyright (c) 2016 @myuuuuun
Released under the MIT license.
"""
import math
import numpy as np
import pandas as pd
import functools
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.cm as cm
EPSIRON = 1.0e-8
np.set_printoptions(precision=3)
np.set_printoptions(linewidth=400)
np.set_printoptions(threshold=np.nan)
pd.set_option('display.max_columns', 130)
pd.set_option('display.width', 1400)
plt.rcParams['font.size'] = 14

# 日本語対応
mpl.rcParams['font.family'] = 'Osaka'


# Explicit Euler Method
# 陽的（前進）オイラー法
def euler(func, init, t_start, step, repeat):
    if not isinstance(func, list):
        func = [func]

    if not isinstance(init, list):
        init = [init]

    if len(init) != len(func):
        raise ValueError("微分係数の数と初期値の数が一致しません")

    dim = len(func)
    path = np.zeros((dim+1, repeat), dtype=float)
    path[:, 0] = [t_start] + init

    for i in range(1, repeat):
        current = path[1:, i-1]
        path[0, i] = t_start + i * step
        for s in range(dim):
            path[s+1, i] = current[s] + func[s](current) * step
 
    return path


# Modified Euler Method
# 修正オイラー法
def modified_euler(func, init, t_start, step, repeat):
    if not isinstance(func, list):
        func = [func]

    if not isinstance(init, list):
        init = [init]

    if len(init) != len(func):
        raise ValueError("微分係数の数と初期値の数が一致しません")

    dim = len(func)
    path = np.zeros((dim+1, repeat), dtype=float)
    path[:, 0] = [t_start] + init
    k1 = np.zeros(dim, dtype=float)
    k2 = np.zeros(dim, dtype=float)

    for i in range(1, repeat):
        current = path[1:, i-1]
        path[0, i] = t_start + i * step

        # k1
        for s in range(dim):
            k1[s] = func[s](current)

        # k2
        for s in range(dim):
            k2[s] = func[s](current + step * k1)
        
        path[1:, i] = current + step * (k1 + k2) / 2

    return path


# Explicit RK4 Method
# 4段4次ルンゲ・クッタ
def runge_kutta(func, init, t_start, step, repeat):
    if not isinstance(func, list):
        func = [func]

    if not isinstance(init, list):
        init = [init]

    if len(init) != len(func):
        raise ValueError("微分係数の数と初期値の数が一致しません")

    dim = len(func)
    path = np.zeros((dim+1, repeat), dtype=float)
    path[:, 0] = [t_start] + init
    k1 = np.zeros(dim, dtype=float)
    k2 = np.zeros(dim, dtype=float)
    k3 = np.zeros(dim, dtype=float)
    k4 = np.zeros(dim, dtype=float)

    for i in range(1, repeat):
        current = path[1:, i-1]
        path[0, i] = t_start + i * step

        # k1
        for s in range(dim):
            k1[s] = func[s](current)

        # k2
        for s in range(dim):
            k2[s] = func[s](current + step * 0.5 * k1)

        # k3
        for s in range(dim):
            k3[s] = func[s](current + step * 0.5 * k2)

        # k4
        for s in range(dim):
            k4[s] = func[s](current + step * k3)
        
        path[1:, i] = current + step * (k1 + 2*k2 + 2*k3 + k4) / 6

    return path


if __name__ == '__main__':
    """
    Sample: solve x''(t) = -x, x(0) = 1, x'(0) = 0

    analytic solution is x(t) = cos(t)
    """

    x = lambda array: array[1]
    dx = lambda array: -1 * array[0]
    func = [x, dx]
    init = [1, 0]
    t_start = 0
    step = 0.01
    repeat = 10000

    ts = np.arange(t_start, step*repeat, step)
    true_path = np.cos(ts)
    euler_path = euler(func, init, t_start, step, repeat)
    modified_euler_path = modified_euler(func, init, t_start, step, repeat)
    rk4_path = runge_kutta(func, init, t_start, step, repeat)

    fig, ax = plt.subplots(figsize=(16, 8))
    plt.title(r'Initial value problem $\"x = -x(t)$')
    plt.xlabel("t")
    plt.ylabel("x")

    plt.plot(ts, true_path, color='orange', linewidth=3, label="true_path(x=cos(t))")
    plt.plot(euler_path[0], euler_path[1], color='blue', linewidth=1, label="Euler approx")
    plt.plot(modified_euler_path[0], modified_euler_path[1], color='green', linewidth=2, label="Modified Euler approx")
    plt.plot(rk4_path[0], rk4_path[1], color='red', linewidth=1, label="RK4 approx")

    plt.legend()
    plt.show()




