
vals = [eval(x) for x in open("input.txt", "r").readlines() if x.strip()]  
vals.append([[2]])
vals.append([[6]])

def check(l, r, d):
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

### quicksort ....

def partition(lst, low, high):
    pivot = high
    i = low - 1
 
    for j in range(low, high):
        t = check(lst[j], lst[pivot], 0)
        if t == 0 or t == 2:
            i = i + 1
            (lst[i], lst[j]) = (lst[j], lst[i])
    (lst[i + 1], lst[high]) = (lst[high], lst[i + 1])
    return i + 1
 

def quickSort(lst, low, high):
    if low < high:
        pi = partition(lst, low, high)
        quickSort(lst, low, pi - 1)
        quickSort(lst, pi + 1, high)
 
a = 0
b = 0
quickSort(vals, 0, len(vals) - 1)
for x in range(len(vals)):
    print(vals[x])
    if vals[x] == [[2]]: a = x + 1
    if vals[x] == [[6]]: b = x + 1

print(f"{a} {b} {a * b}")
