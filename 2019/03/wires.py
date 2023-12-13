import numpy as np

wires = open("input.txt").readlines()

aOps = wires[0].split(",")
bOps = wires[1].split(",")

intersections = []

aHistory = []
aPos = [0, 0]

for i in range(len(aOps)):
    _dir = aOps[i][0]
    mag = int(aOps[i][1:])

    while (mag := mag - 1) >= 0:
        if _dir == "U":
            aPos[1] += 1
        elif _dir == "R":
            aPos[0] += 1
        elif _dir == "D":
            aPos[1] -= 1
        elif _dir == "L":
            aPos[0] -= 1
        aHistory.append((aPos[0], aPos[1]))

bPos = [0, 0]
for i in range(len(bOps)):
    _dir = bOps[i][0]
    mag = int(bOps[i][1:])

    print(i, len(bOps), end="\r")

    while (mag := mag - 1) >= 0:
        if _dir == "U":
            bPos[1] += 1
        elif _dir == "R":
            bPos[0] += 1
        elif _dir == "D":
            bPos[1] -= 1
        elif _dir == "L":
            bPos[0] -= 1
        
        if tuple(bPos) in aHistory:
            intersections.append((bPos[0], bPos[1]))

distances = []
for iPos in intersections:
    distances.append(abs(iPos[0]) + abs(iPos[1]))

print()
print(min(distances))