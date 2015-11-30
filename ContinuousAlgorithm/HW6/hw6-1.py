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


if __name__ == '__main__':
    # x^2 を -1〜1まで積分した。答えは2/3。
    h_list = [2 * pow(2, -i) for i in range(9)]
    int_values = [0.832774000015436, 
                  0.41691937705316179, 
                  0.6658855800450576, 
                  0.66666666653570195, 
                  0.66666666666666663, 
                  0.66666666666666696, 
                  0.66666666666666663, 
                  0.66666666666666696, 
                  0.6666666666666663, 
                  0.66666666666666707]

    j_list = [abs(int_values[i] - int_values[i+1]) for i in range(9)]

    print(h_list)
    print(j_list)
    
    fig, ax = plt.subplots()
    plt.title("x^2を-1〜1までDE積分した際の、ステップ数による誤差変化")

    plt.xlabel("h")
    plt.ylabel("Error")
    plt.xscale('log')
    plt.yscale('log')
    plt.plot(h_list, j_list, color='c')
    plt.show()











