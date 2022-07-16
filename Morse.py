import numpy as np
from scipy.io import wavfile

SAMPLE_RATE = 44100
FREQUENCY = 440
LENGTH_DOT = 0.1
LENGTH_DASH = 0.3
LENGTH_DIGIT = 4410
LENGTH_LETTER = LENGTH_DIGIT * 3
LENGTH_WORD = LENGTH_DIGIT * 7

codes_dict = {
    "A": [0, 1],
    "B": [1, 0, 0, 0],
    "C": [1, 0, 1, 0],
    "D": [1, 0, 0],
    "E": [0],
    "F": [0, 0, 1, 0],
    "G": [1, 1, 0],
    "H": [0, 0, 0, 0],
    "I": [0, 0],
    "J": [0, 1, 1, 1],
    "K": [1, 0, 1],
    "L": [0, 1, 0, 0],
    "M": [1, 1],
    "N": [1, 0],
    "O": [1, 1, 1],
    "P": [0, 1, 1, 0],
    "Q": [1, 1, 0, 1],
    "R": [0, 1, 0],
    "S": [0, 0, 0],
    "T": [1],
    "U": [0, 0, 1],
    "V": [0, 0, 0, 1],
    "W": [0, 1, 1],
    "X": [1, 0, 0, 1],
    "Y": [1, 0, 1, 1],
    "Z": [1, 1, 0, 0],
}

sound_data = np.empty(1)

t = np.linspace(0, LENGTH_DOT, round(SAMPLE_RATE * LENGTH_DOT))
dot_sound = np.sin(FREQUENCY * 2 * np.pi * t)

t = np.linspace(0, LENGTH_DASH, round(SAMPLE_RATE * LENGTH_DASH))
dash_sound = np.sin(FREQUENCY * 2 * np.pi * t)

digit_wait = np.zeros(LENGTH_DIGIT)
letter_wait = np.zeros(LENGTH_LETTER)
word_wait = np.zeros(LENGTH_WORD)


def generate_letter(code, character_data):
    for character in codes_dict[code]:
        if character == 0:
            character_data = np.concatenate([character_data, dot_sound])
        else:
            character_data = np.concatenate([character_data, dash_sound])
        character_data = np.concatenate([character_data, digit_wait])
    character_data = np.concatenate([character_data, letter_wait])
    return character_data


message = input("Enter message to be encoded: ").upper()
for character in message:
    if character.isalpha():
        sound_data = generate_letter(character, sound_data)
    elif character.isspace():
        sound_data = np.concatenate([sound_data, word_wait])

wavfile.write('Sine.wav', SAMPLE_RATE, sound_data)
