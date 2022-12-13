
data = iter([eval(x) for x in open("input.txt", "r").readlines() if x.strip()])

def check(l, r, d):
    print(f"CMP ({d}) {l} and {r}")
    if isinstance(l, int) and isinstance(r, int):
        if l < r: return 0
        if l > r: return 1
        if l == r: return 2
    else:
        l = [l] if isinstance(l, int) else l 
        r = [r] if isinstance(r, int) else r
        x = min(len(l), len(r))
        
        for i in range(x):
            c = check(l[i], r[i], d + 1)
            if c != 2: return c
        if len(l) < len(r): return 0
        if len(l) > len(r): return 1
        if len(l) == len(r): return 2

s = 0;
r = [check(x[0], x[1], 0) for x in zip(data, data)]
for i in range(len(r)):
    s += (i + 1) if r[i] == 0 else 0

print(r)
print(s)
