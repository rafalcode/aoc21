#!/usr/bin/env python3
# implement named groups in the regex module
# and use groupdict() to capture all of them in a dict
import os, re, sys

def pg(g):
    g2=[]
    for i in g:
        g2.append(list(map(str, i)))
    for i in g2:
        for j in i:
            if j=='0':
                print('.', end='')
            else:
                print(j, end='')
        print()

argquan=len(sys.argv)
if argquan != 2:
    print("This script requires one argument: the target file")
    sys.exit(2)

# RGX=re.compile(r'^(?P<a>[0-9]+),(?P<b>[0-9]+) -> (?P<c>[0-9]+),(?P<d>[0-9]+)')
RGX=re.compile(r'^([0-9]+),([0-9]+) -> ([0-9]+),([0-9]+)')
# p#x style labelling used to collaborate with dicts alphabetic ordering of keys
L2=[]
fname = sys.argv[1]
with open(fname) as file:
    for line in file:
        m=RGX.match(line)
        if m is not None:
            #L2.append(m.groupdict())
            L2.append(m.groups())
file.closed
L4=[] # convert to list of 4tuples
for i in L2:
    L4.append(tuple(map(int, i)))
mx=[0, 0]
for i in L4:
    # get max x and max y
    if i[0]>mx[0]:
        mx[0]=i[0]
    elif i[2]>mx[0]:
        mx[0]=i[2]
    if i[1]>mx[1]:
        mx[1]=i[1]
    elif i[3]>mx[1]:
        mx[1]=i[3]
        # straight lines only
print(L4)
#and, as we were dealing with indices:
mx[0] +=1
mx[1] +=1
# print("maxes: %i, %i" % (mx[0], mx[1]))

g=[[0 for i in range(mx[0])] for j in range(mx[1])]
# lg =len(g)
# for i in g:
# print(g)
for i in L4:
    if i[0]==i[2]:
        if i[1]<i[3]:
            print("x=%i, y from %i to %i" % (i[0], i[1], i[3]+1))
            for j in range(i[1], i[3]+1):
                g[j][i[0]]+=1
        else:
            print("x=%i, y from %i to %i" % (i[0], i[1], i[3]-1))
            for j in range(i[1], i[3]-1, -1):
                g[j][i[0]]+=1
    elif i[1]==i[3]:
        if i[0]<i[2]:
            print("y=%i, x from %i to %i" % (i[1], i[0], i[2]+1))
            for j in range(i[0], i[2]+1):
                g[i[1]][j]+=1
        else:
            print("y=%i, x from %i to %i" % (i[1], i[0], i[2]-1))
            for j in range(i[0], i[2]-1, -1):
                g[i[1]][j]+=1
    elif i[0]<i[2] and i[1]<i[3]:
        d=i[2]-i[0]+1
        print(">,> steps%i x from %i up %i and y from %i uo %i" % (d, i[0], d, i[1], d))
        for j in range(d):
            g[i[1]+j][i[0]+j]+=1
    elif i[0]>i[2] and i[1]<i[3]:
        d=i[3]-i[1]+1
        print("<,> steps%i x from %i down %i and y from %i up %i" % (d, i[0], d, i[1], d))
        for j in range(d):
            g[i[1]+j][i[0]-j]+=1
    elif i[0]<i[2] and i[1]>i[3]:
        d=i[2]-i[0]+1
        print(">,< steps%i x from %i to %i and y from %i to %i" % (d, i[0], i[2]+1, i[1], i[3]-1))
        for j in range(d):
            g[i[1]-j][i[0]+j]+=1
    elif i[0]>i[2] and i[1]>i[3]:
        d=i[0]-i[2]+1
        print("<,< steps%i x from %i down %i and y from %i down %i" % (d, i[0], d, i[1], d))
        for j in range(d):
            g[i[1]-j][i[0]-j]+=1
pg(g)
twop=0 # two plus
for i in range(mx[0]):
    for j in range(mx[1]):
        if g[i][j]>1:
            twop+=1
print("# two or more points = %i" % twop)
