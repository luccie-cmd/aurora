#
# Copyright (c) - All Rights Reserved.
# 
# See the LICENCE file for more information.
#

import re
from decimal import Decimal

def parseSize(size: str):
    size_match = re.match(r'([0-9\.]+)([kmg]?)', size, re.IGNORECASE)
    if size_match is None:
        raise ValueError(f'Error: Invalid size {size}')

    result = Decimal(size_match.group(1))
    multiplier = size_match.group(2).lower()

    if multiplier == 'k':
        result *= 1024
    if multiplier == 'm':
        result *= 1024 ** 2
    if multiplier == 'g':
        result *= 1024 ** 3

    return int(result)

def compareFiles(file1, file2):
    try:
        with open(file1) as f1:
            with open(file2) as f2:
                return f1.read() == f2.read()
    except FileNotFoundError:
        return False