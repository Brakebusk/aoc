stars = open("input.txt").readlines()

def fuel(f):
    t = 0
    while (f := int(f / 3) - 2) > 0:
        t += f
    return t

print(sum(map(fuel, map(int, stars))))