#!/usr/bin/env python3
# this script does what?
import sys 

if __name__ == "__main__":
    argquan=len(sys.argv)
    if argquan != 2:
       print("This script one arg")
       sys.exit(2)

    with open(sys.argv[1]) as f: fll=f.read().splitlines()
    aal=len(fll)
    ii=0
    su=0
    sua=[]
    for fl in fll:
        out=0
        ll=len(fl)
        e=[]
        j=0;
        for k in range(ll):
            if fl[k] =='(' or fl[k]=='[' or fl[k] =='{' or fl[k]=='<':
                e.append(fl[k])
            elif fl[k] ==')' or fl[k]==']' or fl[k] =='}' or fl[k]=='>':
                c=e.pop()
                if (c =='(' and fl[k] == ')') or (c =='[' and fl[k] == ']') or (c =='{' and fl[k] == '}') or (c =='<' and fl[k] == '>'):
                    pass
                else:
                    out=1
                    break
        ii+=1
        if out:
            continue
        ell=len(e)
        su=0
        for k in range(ell-1,-1,-1):
            su*=5
            if e[k] =='(':
                su += 1
            elif e[k] =='[':
                su += 2
            elif e[k] =='{':
                su += 3
            elif e[k] =='<':
                su += 4
        sua.append(su)
    sua=sorted(sua)
    sll=len(sua)
    ii=int(sll/2.0) # no additional 1 required due to 0-indexing
    print(sua[ii])
