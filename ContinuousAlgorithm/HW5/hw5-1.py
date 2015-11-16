#!/usr/bin/python
#-*- encoding: utf-8 -*-
"""
Copyright (c) 2015 @myuuuuun
Released under the MIT license.
"""

import math
import numpy as np
import functools
import matplotlib.pyplot as plt
import matplotlib.cm as cm
EPSIRON = 1.0e-8


# P0〜P_(length-1)までのルジャンドル多項式の, xにおける値の配列を返す
def legendre(x, length):
    values = [1, x]
    for i in range(2, length):
        v = ((2*i-1)*x*values[i-1] - (i-1) * values[i-2]) / i
        values.append(v)
    
    return values


# P0〜P_(length-1)までのチェビシェフ多項式の, xにおける値の配列を返す
def chebyshev(x, length):
    values = []
    for i in range(length):
        v = np.cos(i * np.arccos(x))
        values.append(v)
    
    return values


if __name__ == '__main__':
    # 共通設定
    length = 6
    x_list = np.arange(-0.99, 1.00, 0.01)
    f_matrix = np.zeros((length, 199), dtype=float)

    # legendre
    """
    for i, x in enumerate(x_list):
        values = legendre(x, length)
        for j in range(length):
            f_matrix[j][i] = values[j]

    fig, ax = plt.subplots()
    plt.title("Legendre多項式")

    plt.xlabel("x")
    plt.ylabel("f")
    plt.xlim(-1, 1)
    for j in range(length):
        plt.plot(x_list, f_matrix[j], color=cm.gist_earth(j/length), label='P{0}'.format(j))
    plt.legend()
    plt.show()
    """

    """
    # chebyshev
    for i, x in enumerate(x_list):
        values = chebyshev(x, length)
        for j in range(length):
            f_matrix[j][i] = values[j]

    fig, ax = plt.subplots()
    plt.title("Chebyshev多項式")

    plt.xlabel("x")
    plt.ylabel("f")
    plt.xlim(-1, 1)
    for j in range(length):
        plt.plot(x_list, f_matrix[j], color=cm.gist_earth(j/length), label='P{0}'.format(j))
    plt.legend()
    plt.show()
    """










