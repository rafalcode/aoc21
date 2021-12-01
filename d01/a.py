#!/usr/bin/env python3
# this script does what?
import sys 

if __name__ == "__main__":
    argquan=len(sys.argv)
    if argquan != 1:
       print("This script no args")
       sys.exit(2)
    aa = [201, 208, 214,202,240, 250, 233, 288, 291]
    aal = len(aa)
    count=0
    for i in range(1, aal):
        if aa[i]>aa[i-1]:
            count+=1
        
    print("%i, %i" % (aal, count))

