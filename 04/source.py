

def setInSet(ga,gb):
  return int(ga[0]) >= int(gb[0]) and int(ga[1]) <= int(gb[1])

def setOverlap(ga,gb):
  return (int(ga[0]) >= int(gb[0]) and int(ga[0]) <= int(gb[1]) or
          int(ga[1]) >= int(gb[0]) and int(ga[1]) <= int(gb[1]))

s1 = 0
s2 = 0
for l in open('input.txt', 'r').readlines():
  l = l.strip('\n')
  if l:
    g = l.split(',')
    ga = g[0].split('-')
    gb = g[1].split('-')

    # part 1
    if setInSet(ga,gb) or setInSet(gb,ga):
      s1 = s1 + 1
      #print(f'{ga[0]}-{ga[1]},{gb[0]}-{gb[1]} {s}')

    # part 2
    if setOverlap(ga,gb) or setInSet(gb,ga):
      s2 = s2 + 1
      #print(f'{ga[0]}-{ga[1]},{gb[0]}-{gb[1]} {s}')

print(f'Part 1: {s1}. Part 2: {s2}')
