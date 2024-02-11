import csv
import sys

def get_code():
    file = open('cgi-bin/morse.csv',mode='r', encoding='latin-1')
    fd = file.read()
    file.close()
    raw = fd.replace('_', '-')
    morse_code = {}
    lines = raw.split('\n')
    for par in lines:
        if ',' in par:
            leter, code = par.split(',')
            morse_code[leter] = code
    del morse_code[next(iter(morse_code))]
    return(morse_code)

argument = input()

if argument == None or argument == "":
	print("Input error")
	exit()
else:
    if ("=") in argument:
        argument = argument[(argument.find("=") + 1):]
        argument = argument.replace("+", " ")
    else:
        print("Unexpected input")
        exit()
input = argument.strip()

morse_code = get_code()

output = ""
i = 0
while i < len(input):
    if input[i].upper() in morse_code.keys():
        output += morse_code[input[i].upper()] + ' '
    elif input[i] == ' ':
        output += "  " + ' '
    i += 1
   
print(output)
