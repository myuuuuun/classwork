# !/usr/bin/python
# -*- encoding: utf-8 -*-
'''
Modules for imcomplete information games.

Copyright (c) 2015 @myuuuuun
Released under the MIT license.
'''

from __future__ import division, print_function
import math
import itertools
import numpy as np
import matplotlib.pyplot as plt
np.set_printoptions(threshold=np.nan)


def is_best_response(str_pattern, player_num, players, strategies, payoff_func):
    str_pattern = list(str_pattern)
    current_payoff = payoff_func(str_pattern)[player_num]
    for new_strategy in strategies:
        new_pattern = str_pattern[:]
        new_pattern[player_num] = new_strategy
        deviated_payoff = payoff_func(new_pattern)[player_num]

        if deviated_payoff > current_payoff:
            print("strategy:", str_pattern, "deviate to:", new_pattern)
            return False

    return True


def is_nash_equilibrium(str_pattern, players, strategies, payoff_func):
    for player_num in range(len(players)):
        if not is_best_response(str_pattern, player_num, players, strategies, payoff_func):
            return False
    
    print("strategy:", str_pattern, "deviate to: nothing")
    return True


def find_nash_equilibrium(players, strategies, payoff_func):
    len_players = len(players)
    len_strategies = len(strategies)
    nash_equilibria = []

    for str_pattern in itertools.product(strategies, repeat=len_players):
        if is_nash_equilibrium(str_pattern, players, strategies, payoff_func):
            nash_equilibria.append(str_pattern)

    return nash_equilibria



"""
不完備情報ゲームは、
G = <>
"""
class Imcomplete_information_game():

    def __init__():
        a

    # 戦略はstateに関係なく同じ
    def find_dominant_strategy():
        pass


    # 戦略は自分のstateのみに依存（相手のstateには依存せず）
    def find_expost_equilibrium():
        pass


    def find_bayesian_nash_equilibrium():
        pass


"""
Samples

------------------
Prisoner's dilemma
------------------

The game G = (N, S, U) is defined as below.

    Player: N = {0, 1}
    Strategy: S = S0 × S1, Si = {'Cooperate', 'Defect'}
    Utility: 
            C      D
        |------|------|
    C   | 2, 2 | 0, 3 |
        |------|------|
    D   | 3, 0 | 1, 1 |
        |------|------|

(Pure) nash equilibrium is ['Defect', 'Defect'].

Sourcecode:

players = [0, 1]
strategies = ["Cooperate", "Defect"]

def payoff_func(str_pattern):
    if str_pattern == ["Cooperate", "Cooperate"]:
        return [2, 2]

    elif str_pattern == ["Defect", "Cooperate"]:
        return [3, 0]

    elif str_pattern == ["Cooperate", "Defect"]:
        return [0, 3]

    elif str_pattern == ["Defect", "Defect"]:
        return [1, 1]

    else:
        print("ERROR")


print("Nash equilibrium is...")
print(find_nash_equilibrium(players, strategies, payoff_func))


-------------------
Battle of the sexes
-------------------

The game G = (N, S, U) is defined as below.

    Player: N = {'Man', 'Woman'}
    Strategy: S = S0 × S1, Si = {'Opera', 'Football'}
    Utility: (Row player is woman / column player is man)
            O      F
        |------|------|
    O   | 2, 1 | 0, 0 |
        |------|------|
    F   | 0, 0 | 1, 2 |
        |------|------|

(Pure) nash equilibrium are ['Opera', 'Opera'] and ['Football', 'Football'].

Sourcecode:

players = ["Man", "Woman"]
strategies = ["Opera", "Football"]

def payoff_func(str_pattern):
    if str_pattern == ["Opera", "Opera"]:
        return [2, 1]

    elif str_pattern == ["Opera", "Football"]:
        return [0, 0]

    elif str_pattern == ["Football", "Opera"]:
        return [0, 0]

    elif str_pattern == ["Football", "Football"]:
        return [1, 2]

    else:
        print("ERROR")


print("Nash equilibrium are...")
print(find_nash_equilibrium(players, strategies, payoff_func))

"""


if __name__ == '__main__':
    """
    players = [0, 1]
    strategies = range(0, 201, 20)

    def payoff_func(str_pattern):
        return [160 - str_pattern[0], 0] if str_pattern[0] >= str_pattern[1] else [0, 100 - str_pattern[1]]

    print(find_nash_equilibrium(players, strategies, payoff_func))
    """


    """
    player_num = 3
    players = range(player_num)
    strategies = range(0, 201, 20)

    def payoff_func(str_pattern):
        # NumPyのargmaxはタイがある場合は最初の1つのindexを返すので、このままで大丈夫
        max_index = np.argmax(str_pattern)
        
        if max_index == 0:
            return [180 - str_pattern[0], 0, 0]

        elif max_index == 1:
            return [0, 140 - str_pattern[1], 0]

        elif max_index == 2:
            return [0, 0, 100 - str_pattern[2]]

        else:
            print("ERROR")

    print(find_nash_equilibrium(players, strategies, payoff_func))
    """





