#!/usr/bin/env python3
# this script does what?
import numpy as np
import sys 

if __name__ == "__main__":
    argquan=len(sys.argv)
    if argquan != 2:
       print("This script one arg")
       sys.exit(2)

    ma=np.loadtxt(sys.argv[1], dtype=int, usecols=range(10))
    print(ma)
