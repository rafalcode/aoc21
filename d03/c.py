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
    # check first line
    ll=len(fl[0])
    bb=fl
    bbl=len(bb)
    for k in range(ll):
        c01=[0,0];
        if bbl==1:
            break
        for j in range(bbl):
            if bb[j][k]=='1':
                c01[1] +=1
            elif bb[j][k]=='0':
                c01[0] +=1
        print("%i 0s vs. %i 1s" %(c01[0], c01[1]))
        dd=[]
        for j in range(bbl):
            if c01[1]>=c01[0]:
                if bb[j][k]=='1':
                    dd.append(j)
            else:
                if bb[j][k]=='0':
                    dd.append(j)
        cc=[bb[i] for i in dd]
        bb=cc
        bbl=len(bb)
    if bbl != 1:
        raise Exception("bbl not 1")
    print(bb)
    ogen=int(''.join(bb), 2)
    print(ogen)
    # scru
    bb=fl
    bbl=len(bb)
    for k in range(ll):
        c01=[0,0];
        if bbl==1:
            break
        for j in range(bbl):
            if bb[j][k]=='1':
                c01[1] +=1
            elif bb[j][k]=='0':
                c01[0] +=1
        print("%i 0s vs. %i 1s" %(c01[0], c01[1]))
        dd=[]
        for j in range(bbl):
            if c01[1]>=c01[0]:
                if bb[j][k]=='0':
                    dd.append(j)
            else:
                if bb[j][k]=='1':
                    dd.append(j)
        cc=[bb[i] for i in dd]
        bb=cc
        bbl=len(bb)
    if bbl != 1:
        raise Exception("2nd bbl not 1")
    print(bb)
    scru=int(''.join(bb), 2)
    print(scru)
    print("lifesupportrating ogen*scru (%i*%i)= %i" % (ogen, scru, ogen*scru))
