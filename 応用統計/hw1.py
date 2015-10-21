#-*- encoding: utf-8 -*-
'''
Ouyoutoukei HW1
'''
# for python2
from __future__ import division, print_function
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import statsmodels.api as sm
np.set_printoptions(precision=3)
pd.set_option('display.max_columns', 20)
pd.set_option('display.width', 200)
pd.set_option('display.max_rows', 200)
np.set_printoptions(threshold=np.nan)


df = pd.read_csv( 'odakyu-mansion.csv' )
#print(df)
print(df.describe())
#df.set_index(['year', 'month']).plot()
data_len = df.shape[0]



# 家の向きはdummyに
df['d_N'] = np.zeros(data_len, dtype=float)
df['d_E'] = np.zeros(data_len, dtype=float)
df['d_W'] = np.zeros(data_len, dtype=float)
df['d_S'] = np.zeros(data_len, dtype=float)
for i, row in df.iterrows():
    for direction in ["N", "W", "S", "E"]:
        if direction in str(row.muki):
            df.loc[i, 'd_{0}'.format(direction)] = 1
    
# 欠損値は平均で置き換え
df = df.fillna(df.mean())

# 定数項も加える
X = sm.add_constant(df[['time', 'bus', 'walk', 'area', 'bal', 'kosuu', 'floor', 'tf', 'd_N', 'd_E', 'd_S', 'd_W', 'year']])

# 普通の最小二乗法
model = sm.OLS(df.price, X)
results = model.fit()

# 結果を表示
print(results.summary())

# 戸数の外れ値を外す
print(df.loc[161])
df = df.drop(161)

X = sm.add_constant(df[['time', 'bus', 'walk', 'area', 'bal', 'kosuu', 'floor', 'tf', 'd_N', 'd_E', 'd_S', 'd_W', 'year']])
model = sm.OLS(df.price, X)
results = model.fit()
print(results.summary())


X = sm.add_constant(df[['time', 'bus', 'walk', 'area', 'bal', 'tf', 'd_N', 'd_E', 'd_S', 'd_W', 'year']])
model = sm.OLS(df.price, X)
results = model.fit()
print(results.summary())

X = sm.add_constant(df[['time', 'bus', 'walk', 'area', 'tf', 'year']])
model = sm.OLS(df.price, X)
results = model.fit()
print(results.summary())
"""

f = lambda row: model.predict( np.concatenate( ([1], row[columns].values) ) )[0]
df['p'] = df.apply( lambda row: f( row ), axis=1 )
df[ ['population', 'p'] ].plot()

plt.show()
"""

"""
# データを読み込む
data = np.loadtxt("test.txt")
x = data.T[0]
y = data.T[1]

# サンプルの数
nsample = x.size

# おまじない (後で解説)
X = np.column_stack((np.repeat(1, nsample), x))

# 回帰実行
model = sm.OLS(y, X)
results = model.fit()

# 結果の概要を表示
print(results.summary())

# パラメータの推定値を取得
a, b = results.params

# プロットを表示
plt.plot(x, y, 'o')
plt.plot(x, a+b*x)
plt.text(0, 0, "a={:8.3f}, b={:8.3f}".format(a,b))
plt.show()
"""