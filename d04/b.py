#!/usr/bin/env python3
# this script does what?
import sys 

def sumem(l, nc, nr):
    ra=[]
    ca=[]
    for i in range(nr):
        if sum(l[i]) == 5:
            ra.append(i)
    for j in range(nc):
        su=0
        for i in range(nr):
            su+=l[i][j]
        if su == 5:
            ca.append(j)
    return ra, ca

if __name__ == "__main__":
    argquan=len(sys.argv)
    if argquan != 2:
       print("This script one arg")
       sys.exit(2)

    with open(sys.argv[1]) as f: fl=f.read().splitlines()
    aal=len(fl)
    bb= list(map(int, fl[0].split(',')))
    # bb= fl[0].split(',')
    print(bb)
    alc=[] # all cards
    for i in range(2,aal,6):
        alc.append([ list(map(int,fl[j].split())) for j in range(i,i+5)])
    alcl=len(alc)

    # blc our mark with 1-to-1 corresp
    blc=[[[0 for i in range(5)] for j in range(5)] for k in range(alcl)]
    
    seen=0
    for b in bb:
        for k in range(alcl):
            for i in range(5):
                for j in range(5):
                    if b==alc[k][i][j]:
                        blc[k][i][j] =1
            r1, c1 = sumem(blc[k], 5, 5)
            if len(r1)==1:
                usu=0 # sum of unmarked numbers
                for i in range(5):
                    for j in range(5):
                        if blc[k][i][j]==0:
                            usu += alc[k][i][j]
                print("fullrow@%i: card%i, row%i, score%i" % (b, k, r1[0], usu*b))
                seen=1
            if len(c1)==1:
                usu=0
                for i in range(5):
                    for j in range(5):
                        if blc[k][i][j]==0:
                            usu += alc[k][i][j]
                print("fullcol@%i: card%i, col%i, score%i" % (b, k, c1[0], usu*b))
                seen=1
            if seen==1:
                break
        if seen==1:
            break
