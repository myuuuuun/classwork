# !/usr/bin/python
# -*- encoding: utf-8 -*-

from __future__ import division, print_function
import math
import numpy as np
import matplotlib.pyplot as plt
from functools import reduce
np.set_printoptions(threshold=np.nan)


# 問題1(1) 効用可能性フロンティアを描く
def utility_possibility_frontier():
    fig, ax = plt.subplots(figsize=(10, 8))
    plt.title("Utility Possibility Frontier")
    plt.xlabel("U1")
    plt.ylabel("U2")

    # 45度線をプロット
    plt.plot(np.arange(-1.0, 3.05, 0.05), np.arange(-1.0, 3.05, 0.05), color='#88ffff', linewidth=1, label='45°Line')

    # tをパラメタとして、tがある値を取る時の、u1, u2の値を求め、プロットする
    t = np.arange(0.0, 1.05, 0.05)
    u1 = (lambda t: -2 * pow(t, 2) + 3/2 * t + 1/2)(t)
    u2 = (lambda t: -1/2 * pow(t, 2) - 3/2 * t + 2)(t)
    plt.plot(u1, u2, color='b', linewidth=2, label='Frontier Curve (0<=t<=1)')

    # t=? の時のu1, u2の値を出力
    for z in zip(t, u1, u2):
        print("t = {0:.3f} のとき u1 = {1:.3f}, u2 = {2:.3f}".format(z[0], z[1], z[2]))

    plt.legend()
    ax.set_xlim(-0.1, 3.0)
    ax.set_ylim(-0.1, 3.0)
    plt.plot([0, 0.75, 0.78, 0.78, 0.5], [0, 1.125, 1.32, 1.414, 2.0], 'o', color='b')
    plt.text(0.1, 0.05, 'u1=0, u2=0 (t=1.0)', ha = 'left', va = 'top')
    plt.text(0.8, 1.125, 'u1=0.75, u2=1.125 (t=0.5)', ha = 'left', va = 'center')
    plt.text(0.83, 1.32, 'u1=0.78, u2=1.32 (t=0.4)', ha = 'left', va = 'center')
    plt.text(0.83, 1.414, 'u1=0.78, u2=1.414 (t=0.35)', ha = 'left', va = 'bottom')
    plt.text(0.5, 2.1, 'u1=0.5, u2=2.0 (t=0.0)', ha = 'center', va = 'bottom')
    plt.show()


#utility_possibility_frontier()


# 問題1(2) ラッファー・カーブを描く
def laffer_curve():
    fig, ax = plt.subplots(figsize=(10, 8))
    plt.title("Laffer Curve")
    plt.xlabel("t: tax rate")
    plt.ylabel("A: subsidy")


    # t（税率）とA（補助金額）の関係を描く
    t = np.arange(0.0, 1.05, 0.05)
    A = (lambda t: 5.0/2 * t * (1-t))(t)
    plt.plot(t, A, color='b', linewidth=2, label='Laffer Curve (0<=t<=1)')

    # t=? の時のAの値を出力
    for z in zip(t, A):
        print("t = {0:.3f} のとき A = {1:.3f}".format(z[0], z[1]))

    plt.legend()
    ax.set_xlim(0.0, 1.0)
    ax.set_ylim(0.0, 1.0)
    plt.plot([0.5], [0.625], 'o', color='b')
    plt.text(0.5, 0.675, 't=0.5, A=0.625', ha = 'center', va = 'bottom')
    plt.show()


#laffer_curve()


# 問題1(3)-1 ベンサム型の社会厚生関数を想定する時、最適な税率を求める
def bentham_optimum_tax():
    fig, ax = plt.subplots(figsize=(10, 8))
    plt.title("Bentham Social Welfare Curves")
    plt.xlabel("U1")
    plt.ylabel("U2")

    # 45度線をプロット
    plt.plot(np.arange(-1.0, 3.05, 0.05), np.arange(-1.0, 3.05, 0.05), color='#88ffff', linewidth=1, label='45°Line')

    # 効用可能性フロンティアをプロット
    t = np.arange(0.0, 1.05, 0.05)
    u1 = (lambda t: -2 * pow(t, 2) + 3/2 * t + 1/2)(t)
    u2 = (lambda t: -1/2 * pow(t, 2) - 3/2 * t + 2)(t)
    plt.plot(u1, u2, color='b', linewidth=2, label='Frontier Curve (0<=t<=1)')

    # ベンサム型社会厚生関数の無差別曲線をプロット
    plt.plot(0, 0, color='pink', linewidth=1, label='Bentham Social Welfare (U1 + U2 = W)')
    u_list = [2.0, 2.5, 3.0]
    u1 = np.arange(-1.0, 3.05, 0.05)
    for u in u_list: 
        u2 = u - u1
        plt.plot(u1, u2, color='pink', linewidth=1)

    plt.legend()
    ax.set_xlim(-0.1, 3.0)
    ax.set_ylim(-0.1, 3.0)
    plt.plot([0, 0.5], [0, 2.0], 'o', color='b')
    plt.text(0.1, 0.05, 'u1=0, u2=0 (t=1.0)', ha = 'left', va = 'top')
    plt.text(0.5, 2.1, 'u1=0.5, u2=2.0 (t=0.0)', ha = 'center', va = 'bottom')

    plt.text(1.6, 0.5, 'w=2.0', ha = 'left', va = 'top')
    plt.text(1.85, 0.75, 'w=2.5', ha = 'left', va = 'top')
    plt.text(2.1, 1.0, 'w=3.0', ha = 'left', va = 'top')

    plt.show()


#bentham_optimum_tax()


# 問題1(3)-2 ロールズ型の社会厚生関数を想定する時、最適な税率を求める
def rawls_optimum_tax():
    fig, ax = plt.subplots(figsize=(10, 8))
    plt.title("Rawls Social Welfare Curves")
    plt.xlabel("U1")
    plt.ylabel("U2")

    # 45度線をプロット
    plt.plot(np.arange(-1.0, 3.05, 0.05), np.arange(-1.0, 3.05, 0.05), color='#88ffff', linewidth=1, label='45°Line')

    # 効用可能性フロンティアをプロット
    t = np.arange(0.0, 1.05, 0.05)
    u1 = (lambda t: -2 * pow(t, 2) + 3/2 * t + 1/2)(t)
    u2 = (lambda t: -1/2 * pow(t, 2) - 3/2 * t + 2)(t)
    plt.plot(u1, u2, color='b', linewidth=2, label='Frontier Curve (0<=t<=1)')

    # ベンサム型社会厚生関数の無差別曲線をプロット
    plt.plot(0, 0, color='pink', linewidth=1, label='Rawls Social Welfare (min(U1, U2) = W)')
    u_list = [0.6, 0.78, 1.0]
    for u in u_list:
        u1 = [u, u, 3.0]
        u2 = [3.0, u, u]

        print(u1)
        print(u2)
        plt.plot(u1, u2, color='pink', linewidth=1)

    plt.legend()
    ax.set_xlim(-0.1, 3.0)
    ax.set_ylim(-0.1, 3.0)
    plt.plot([0, 0.78, 0.78, 0.5], [0, 1.32, 1.414, 2.0], 'o', color='b')
    plt.text(0.83, 1.32, 'u1=0.78, u2=1.32 (t=0.4)', ha = 'left', va = 'center')
    plt.text(0.83, 1.414, 'u1=0.78, u2=1.414 (t=0.35)', ha = 'left', va = 'bottom')

    plt.text(2.1, 0.63, 'w=0.6', ha = 'left', va = 'bottom')
    plt.text(2.1, 0.82, 'w=0.78', ha = 'left', va = 'bottom')
    plt.text(2.1, 1.05, 'w=1.0', ha = 'left', va = 'bottom')

    plt.show()


#rawls_optimum_tax()


income_list = np.array([50, 50, 200, 300, 400])


# 問題2(1) ジニ係数を求める
def gini(income_list):
    size = len(income_list)
    average = np.average(income_list)
    total = 0

    for i in income_list:
        for j in income_list:
            total += math.fabs(i-j)

    gini = total / (2.0 * math.pow(size, 2) * average)

    return gini


#print(gini(income_list))


def fgt(income_list, gamma):
    size = len(income_list)
    income_list = np.asarray(income_list, dtype=np.float)
    income_list = np.sort(income_list)
    median = np.median(income_list)
    poverty_line = median / 2.0
    poors = income_list[income_list < poverty_line]
    fgt = 0

    for i in poors:
        fgt += pow((1 - i/poverty_line), gamma)

    fgt /= size

    return fgt


# 問題2(2) 相対的貧困率を求める
def relative_poverty_rate(income_list):
    return fgt(income_list, 0)


#print(relative_poverty_rate(income_list))


# 問題2(3) 貧困ギャップ率を求める
def poverty_gap_rate(income_list):
    return fgt(income_list, 1)


#print(poverty_gap_rate(income_list))


# 問題2(4) σ=0, 1, 2のときのアトキンソン指数を求める
def atkinson(income_list, risk_aversion):
    size = len(income_list)
    income_list = np.asarray(income_list, dtype=np.float)
    average = np.average(income_list)

    if risk_aversion == 1.0:
        equipartition_income = np.power(reduce(lambda a, b: a * b, income_list), 1.0/size)

    else:
        equipartition_income = np.power( 1.0/size * np.sum(np.power(income_list, 1-risk_aversion)), 1.0/(1.0-risk_aversion) )

    atkinson = 1 - equipartition_income/average

    return atkinson


#print(atkinson(income_list, 0))
#print(atkinson(income_list, 1))
#print(atkinson(income_list, 2))




