#!/usr/bin/env python3

'Translation of https://cs.nyu.edu/~perlin/noise/'

import json
import math
from pathlib import Path

OUR_ROOT = Path(__file__).resolve().parent

def perlin_noise(x, y, z):
    X = math.floor(x) & 255                            # FIND UNIT CUBE THAT
    Y = math.floor(y) & 255                            # CONTAINS POINT.
    Z = math.floor(z) & 255
    x -= math.floor(x)                                 # FIND RELATIVE X,Y,Z
    y -= math.floor(y)                                 # OF POINT IN CUBE.
    z -= math.floor(z)
    u = fade(x)                                        # COMPUTE FADE CURVES
    v = fade(y)                                        # FOR EACH OF X,Y,Z.
    w = fade(z)
    A = p[X  ]+Y; AA = p[A]+Z; AB = p[A+1]+Z           # HASH COORDINATES OF
    B = p[X+1]+Y; BA = p[B]+Z; BB = p[B+1]+Z           # THE 8 CUBE CORNERS,

    return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),  # AND ADD
                                   grad(p[BA  ], x-1, y  , z   )), # BLENDED
                           lerp(u, grad(p[AB  ], x  , y-1, z   ),  # RESULTS
                                   grad(p[BB  ], x-1, y-1, z   ))),# FROM  8
                   lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),  # CORNERS
                                   grad(p[BA+1], x-1, y  , z-1 )), # OF CUBE
                           lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
                                   grad(p[BB+1], x-1, y-1, z-1 ))))

def fade(t):
    return t ** 3 * (t * (t * 6 - 15) + 10)

def lerp(t, a, b):
    return a * (1 - t) + b * t

def grad(hash, x, y, z):
    h = hash & 15                           # CONVERT LO 4 BITS OF HASH CODE
    u = x if h<8 else y                     # INTO 12 GRADIENT DIRECTIONS.
    v = y if h<4 else (x if h in (12, 14) else z)
    return (u if (h&1) == 0 else -u) + (v if (h&2) == 0 else -v)

p = [0] * 512
permutation = json.load(open(OUR_ROOT / 'permutation.json'))
for i in range(256):
    p[256+i] = p[i] = permutation[i]

if __name__ == '__main__':
    import sys

    if len(sys.argv) != 4:
        print('Usage: {} X Y Z'.format(sys.argv[0]))
        sys.exit(1)

    result = perlin_noise(
        float(sys.argv[1]),
        float(sys.argv[2]),
        float(sys.argv[3]),
    )
    print(repr(result))
