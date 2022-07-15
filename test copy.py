import numpy as np
from scipy.io import wavfile

SAMPLE_RATE = 44100
FREQUENCY = 440
BITS_PER_CHARECTER = 8

dummy_text = b"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et " \
             b"dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut " \
             b"aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum " \
             b"dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui " \
             b"officia deserunt mollit anim id est laborum. "

numberOfCharecters = len(dummy_text)

charectersPerSecond = 45

timePeriod = 1 / charectersPerSecond

length = timePeriod * numberOfCharecters

def encodingFunction(time, timePeriod, byteArray):
    #print(f"time :{time} index : {int(time/timePeriod)}")
    return (byteArray[int(time)] - (100)/2 ) * 100

t = np.linspace(0, length, round(SAMPLE_RATE * length))
y = np.array([encodingFunction(time, timePeriod, dummy_text) for time in t])

# ut = np.concatenate([y, a])
# outorig = np.concatenate([y, a])

# for i in range(100):
#    out = np.concatenate([out, outorig])

wavfile.write('Sine2.wav', SAMPLE_RATE, y.astype(np.int16))
