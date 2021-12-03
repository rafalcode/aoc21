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
    # if ll != 6:
    #     raise Exception("Line is not six chars: %s", % l) # bit strict for AOC
    # g="%*.s" % (ll, '0')
    g=['0' for i in range(ll)]
    e=['0' for i in range(ll)]
    for k in range(ll):
        c01=[0,0];
        for j in range(aal):
            if fl[j][k]=='1':
                c01[1] +=1
            elif fl[j][k]=='0':
                c01[0] +=1
        if c01[1]>c01[0]:
            g[k]='1'
        elif c01[0]>c01[1]:
            e[k]='1'
        # No else needed, default is '0' anyway.
    # print(''.join(g))
    gr=int(''.join(g), 2)
    er=int(''.join(e), 2)
    print("powerconsumption g*e (%i*%i)= %i" % (gr, er, gr*er))
