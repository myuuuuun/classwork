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
    seed = 2016
    eps = np.random.RandomState(seed).normal(size=201, scale=5)
    x_list = np.linspace(-10, 10, 201)
    f_list = np.zeros(201, dtype=float)
    f_list = 3 * x_list + 2 + eps

    data = np.zeros((201, 2), dtype=float)
    data[:, 0] = x_list
    data[:, 1] = f_list

    df = pd.DataFrame(data)
    df = df.reindex(np.random.permutation(df.index))
    df.to_csv("data.csv", index=False, header=False)

    plt.plot(x_list, 3*x_list+2, color='orange', linewidth=2, label="元の直線, 3x+2")
    plt.plot(x_list, f_list, '.', color='gray', linewidth=1, label="観測点, 3x+2+ε, ε~N(0, 5^2)")
    plt.plot(x_list, 2.99175*x_list+1.67285, color='blue', linewidth=2, label="回帰直線, 2.99175x+1.67285")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.legend()
    plt.show()