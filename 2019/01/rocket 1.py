stars = open("input.txt").readlines()

def fuel(star):
    return int(star / 3) -2

print(sum(map(fuel, map(int, stars))))