#!/usr/bin/python
#-*- encoding: utf-8 -*-
"""
Copyright (c) 2015 @myuuuuun
https://github.com/myuuuuun
"""
from __future__ import division
import math
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


if __name__ == "__main__":
    real = np.linspace(-1, 1, 1001)
    imag = np.linspace(-1, 1, 1001)

    data = pd.read_csv('rst.csv', names=("real", "imag", "val", "times"))

    val0 = data[data.val == 0]
    val1 = data[data.val == 1].as_matrix()
    val2 = data[data.val == 2]
    val3 = data[data.val == 3]

    fig, ax = plt.subplots()
    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    #print(val1)

    for row in val1:
        ax.plot(row[0], row[1], ',', color='red', alpha=min(row[2] * 0.1, 1.0))

    
    #ax.plot(val2.real, val2.imag, ',', color='blue', alpha=a)
    #ax.plot(val3.real, val3.imag, ',', color='green', alpha=a)

    ax.plot(0, 0, 'o', color='black', label="inf")
    ax.plot(0, 0, 'o', color='red', label="(1, 0)")
    ax.plot(0, 0, 'o', color='blue', label="(1/2, √3/2)")
    ax.plot(0, 0, 'o', color='green', label="(1/2, -√3/2)")

    ax.plot(val0.real, val0.imag, 'o', color='black')

    plt.title("x^3-1=0 ニュートン法の収束先")
    plt.xlabel("実部")
    plt.ylabel("虚部")
    plt.legend()
    plt.show()



