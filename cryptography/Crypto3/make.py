from random import shuffle
from string import (
    ascii_lowercase,
    ascii_uppercase
)

key_lower = "bulcgrxnjkstiqafhomzveydpw"
key_upper = "BULCGRXNJKSTIQAFHOMZVEYDPW"

description = '''\
Welcome to the cuctf bootcamp. The flag for this challenge are the following words all lowercase and separated by underscores and surrounded by the usual flag foramt: scrambled alphabet.\
'''

def transform(letter):
    if letter in key_lower:
        return key_lower[
            ascii_lowercase.index(letter)
        ]
    elif letter in key_upper:
        return key_upper[
            ascii_uppercase.index(letter)
        ]
    else:
        return letter

print(''.join(map(transform, description)))
