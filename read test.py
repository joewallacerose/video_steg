from os.path import dirname, join as pjoin
from scipy.io import wavfile
import scipy.io

CHARECTER_PER_SECCOND = 45

def decodefunction(pos, data, timePeriod):
    return (data[pos] / 100) + (255/2)


samplerate, data = wavfile.read('Sine2.wav')
print(f"samplerate = {samplerate}")
length = data.shape[0] / samplerate
print(f"length = {length}s")

print(f"{length * CHARECTER_PER_SECCOND} vs {len(data)}")

timePeriod = 1/CHARECTER_PER_SECCOND

noCharecters = length/timePeriod

print(f"{noCharecters} vs {len(data)}")


string = ''

for i in range(len(data)):
    char = chr(int(decodefunction(i, data, timePeriod)))
    string = string + char

print(string)

import matplotlib.pyplot as plt
import numpy as np
time = np.linspace(0., length, data.shape[0])
plt.plot(time, data, label="Left channel")
plt.legend()
plt.xlabel("Time [s]")
plt.ylabel("Amplitude")
plt.show()