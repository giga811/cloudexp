# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt

# s = np.random.poisson(5, 10000)
#
# count, bins, ignored = plt.hist(s, 14, normed=True)
# plt.show()

def generate(mu, sigma):
    s = sigma * np.random.randn(20000) + mu
    del_index = []
    for i in range(len(s)):
        if s[i] <= 0 or s[i] > 100:
            del_index.append(i)
    s = np.delete(s, del_index)
    return s


def show(s):
    count, bins, ignored = plt.hist(s, 14, normed=True)
    plt.show()
