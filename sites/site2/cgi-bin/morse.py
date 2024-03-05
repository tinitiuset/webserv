#!/usr/local/bin/python3

import csv
import sys

def get_code():
    morse_code = {}
    try:
        with open('sites/site2/cgi-bin/morse.csv', mode='r', encoding='latin-1') as file:
            fd = file.read()
            raw = fd.replace('_', '-')
            lines = raw.split('\n')
            for par in lines:
                if ',' in par:
                    leter, code = par.split(',')
                    morse_code[leter] = code
            del morse_code[next(iter(morse_code))]
    except Exception as e:
        print(f"Error al abrir el archivo: {e}")
        sys.exit(1)
    return morse_code

argument = input()

if argument is None or argument == "":
    print("Input error")
    exit()
else:
    if "=" in argument:
        argument = argument[(argument.find("=") + 1):]
        argument = argument.replace("+", " ")
    else:
        print("Unexpected input")
        exit()
input_txt = argument.strip()

morse_code = get_code()

output = ""
i = 0
while i < len(input_txt):
    if input_txt[i].upper() in morse_code.keys():
        output += morse_code[input_txt[i].upper()] + ' '
    elif input_txt[i] == ' ':
        output += "  "
    i += 1

print(output)
