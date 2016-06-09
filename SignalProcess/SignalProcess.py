from scipy.signal import butter, lfilter, freqz, medfilt
import numpy as np
import pandas as pd
from sys import argv
import time


def butter_lowpass(cutoff, fs, order=5):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    return b, a


def butter_lowpass_filter(data, cutoff, fs, order=5):
    b, a = butter_lowpass(cutoff, fs, order=order)
    y = lfilter(b, a, data)
    return y


def MedianMovingSmoothing(x, windowSize):
    y = medfilt(x, windowSize)
    return y


def MeanMovingSmoothing(x, windowSize):
    y = np.zeros(x.size)
    for i in range(0, windowSize // 2):
        y[i] = x[i]
    for i in range((x.size - windowSize // 2), x.size):
        y[i] = x[i]
    summation = x[:windowSize].sum()
    y[windowSize // 2] = summation / windowSize
    for i in range(windowSize // 2 + 1, x.size - windowSize // 2):
        summation = summation - x[i - windowSize // 2 - 1] + x[i + windowSize // 2]
        y[i] = summation / windowSize
    return y


if __name__ == "__main__":
    time.clock()
    order = 6
    fs = 1000.0       # sample rate, Hz
    cutoff = 100.0  # desired cutoff frequency of the filter, Hz

    pg, filename, targetname = argv
    data = pd.read_csv(filename)
    y = pd.DataFrame()
    for i in range(1, 31):
        if(i < 10):
            tmp = 'csi0' + str(i)
        else:
            tmp = 'csi' + str(i)
        tmpVal = butter_lowpass_filter(data[tmp].values, cutoff, fs, order)
        y[i] = MeanMovingSmoothing(tmpVal, 301)
    y.to_csv(targetname)
    print(time.clock())
