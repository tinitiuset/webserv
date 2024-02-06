import csv
import sys


"""
def get_code():
    url = "https://www.dropbox.com/scl/fi/u18ocfuobwxt7lrh18s2d/MORSE.csv?rlkey=5z9j8fga9uejsvndi8pq8ggh8&dl=1"
    response = requests.get(url)
    
    if response.status_code == 200:
        raw = response.text.replace('_', '-')
        morse_code = {}
        lines = raw.split('\n')
        for par in lines:
            if ',' in par:
                leter, code = par.split(',')
                morse_code[leter] = code
        del morse_code[next(iter(morse_code))]
        return morse_code
    else:
        print("Error al descargar el archivo")
        return None
"""

def get_code():
    file = open('./MORSE.csv',mode='r', encoding='latin-1')
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

morse_code = get_code()


if len(sys.argv) < 2:
	print("Error")
	exit()
else:
     i = 1
     argument = ""
     while i < len(sys.argv):
          argument += sys.argv[i].strip() + " "
          i += 1
input = argument.strip()
output = ""
i = 0
while i < len(input):
    if input[i].upper() in morse_code.keys():
        output += morse_code[input[i].upper()] + ' '
    elif input[i] == ' ':
        output += '/' + ' '
    i += 1
   


print(output)
