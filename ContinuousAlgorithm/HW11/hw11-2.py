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
import matplotlib.pyplot as plt
EPSIRON = 1.0e-8
np.set_printoptions(precision=3)
np.set_printoptions(linewidth=400)
np.set_printoptions(threshold=np.nan)


if __name__ == '__main__':
    data = np.array([[1, 0.37064, 0.135654],
                [2, 0.430141, 0.112775],
                [3, 0.205169, 0.0188287],
                [4, 0.265379, 0.0702761],
                [5, 0.29848, 0.0760213],
                [6, 0.371307, 0.0933493],
                [7, 0.329687, 0.0963437],
                [8, 0.350829, 0.0884015],
                [9, 0.308465, 0.0831644],
                [10, 0.329174, 0.0864833],
                [11, 0.332904, 0.0903685],
                [12, 0.330935, 0.0856766],
                [13, 0.33418, 0.0889967],
                [14, 0.334571, 0.0882978],
                [15, 0.335747, 0.0892373],
                [16, 0.333223, 0.0889802],
                [17, 0.333985, 0.0890121],
                [18, 0.332598, 0.0885303],
                [19, 0.332833, 0.0888661],
                [20, 0.333344, 0.0890352]])

    pop_mean = 0.2
    pop_var = 4/45

    size_list = data[:, 0]
    ave_list = np.fabs(data[:, 1] - pop_mean)
    var_list = np.fabs(data[:, 2] - pop_var)

    plt.plot(size_list, ave_list, color='orange', linewidth=2, label="母平均(0.5)と標本平均の乖離")
    plt.xlabel("sample size(2^n)")
    plt.ylabel("difference")
    plt.yscale("log")
    plt.legend()
    plt.show()

    plt.plot(size_list, var_list, color='orange', linewidth=2, label="母分散(0.083)と標本分散の乖離")
    plt.xlabel("sample size(2^n)")
    plt.ylabel("difference")
    plt.yscale("log")
    plt.legend()
    plt.show()



