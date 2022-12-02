
vals = []
t = 0
for l in open('input1.txt', 'r').readlines():
    if l.strip('\n'):
        t += int(l)
    else:
        vals.append(t)
        t = 0
        
a1 = max(vals)
del vals[vals.index(a1)]
a2 = max(vals)
del vals[vals.index(a2)]
a3 = max(vals)
del vals[vals.index(a3)]

print(a1,a2,a3)
print(a1+a2+a3)
