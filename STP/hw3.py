#-*- encoding: utf-8 -*-
"""
SDE simulation

Copyright (c) 2015 @myuuuuun
Released under the MIT license.
"""
import math
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.cm as cm

plt.rcParams['font.size'] = 14
# 日本語対応
mpl.rcParams['font.family'] = 'Osaka'


def simulate(x_0, t_step, t_size, rs):
    # setting
    path = np.zeros((2, t_size), dtype=float)
    t_0 = 0
    path[0][0] = t_0
    path[1][0] = x_0
    x = x_0

    # loop
    for i in range(1, t_size):
        t = t_0 + i * t_step
        rand = -1 if rs.binomial(1, 0.5) == 0 else 1
        x = x + x * (2 - x) * t_step + rand * math.sqrt(t_step) * x
        #rand = rs.normal(0, math.sqrt(t_step))
        #x = x + x * (2 - x) * t_step + x * rand 
        path[0][i] = t
        path[1][i] = x

    return path


def euler(x_0, t_step, t_size):
    # setting
    path = np.zeros((2, t_size), dtype=float)
    t_0 = 0
    path[0][0] = t_0
    path[1][0] = x_0
    x = x_0

    # loop
    for i in range(1, t_size):
        t = t_0 + i * t_step
        x = x + x * (2 - x) * t_step
        path[0][i] = t
        path[1][i] = x

    return path


if __name__ == '__main__':
    # Mersenne Twister pseudo-random number generator
    seed = 20160108
    rs = np.random.RandomState(seed)

    x_0 = 1
    t_size = 10000
    t_step = 0.01
    repeats = 100
    array_de = euler(x_0, t_step, t_size)

    fig, ax = plt.subplots(figsize=(20, 10))
    plt.title("ノイズ有・無の比較")
    plt.xlabel("time")
    plt.ylabel("size")

    plt.plot(0, 0, color='#cccccc', linewidth=1, label="ノイズ入り(100回分)")
    
    array_sde = np.zeros((2, t_size), dtype=float)
    for i in range(repeats):
        array = simulate(x_0, t_step, t_size, rs)
        array_sde += array
        plt.plot(array[0], array[1], color='#cccccc', linewidth=1)
        
    array_sde /= repeats

    plt.plot(array_sde[0], array_sde[1], color='red', linewidth=2, label="100回分の平均")
    plt.plot(array_de[0], array_de[1], color='blue', linewidth=2, label="通常の微分方程式")
        
    plt.legend()
    plt.show()

   



