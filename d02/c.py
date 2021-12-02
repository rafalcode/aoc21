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
    bb= [(l.split()[0],int(l.split()[1])) for l in fl]
    p=[0,0,0]
    for i in bb:
        if(i[0][0] == 'f'): 
            p[0] += i[1]
            p[1] += p[2]*i[1]
        elif(i[0][0] == 'u'): 
            p[2] -= i[1]
        elif(i[0][0] == 'd'): 
            p[2] += i[1]

    print(p)
    print(p[0]*p[1])
