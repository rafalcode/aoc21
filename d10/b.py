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
    for fl in fll:
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
                    print("Corr line " +str(ii)+ ": " +c+ " vs. " +fl[k])
                    if(fl[k]) == ')':
                        su += 3
                    elif(fl[k]) == ']':
                        su += 57
                    elif(fl[k]) == '}':
                        su += 1197
                    elif(fl[k]) == '>':
                        su += 25137
        ii+=1
    print("su=" +str(su))
