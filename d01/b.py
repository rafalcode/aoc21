#!/usr/bin/env python3
# this script does what?
import sys 

if __name__ == "__main__":
    argquan=len(sys.argv)
    if argquan != 2:
       print("This script one arg")
       sys.exit(2)

    with open(sys.argv[1]) as f: fl=f.read().splitlines()
    aal=len(fl)
    bb=list(map(int, fl))
    count=0
    for i in range(1, aal):
        if bb[i]>bb[i-1]:
            count+=1
    print(count)
