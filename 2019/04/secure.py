
possible = 0

for t in range(206938, 679129):
    spl = [int(y) for y in str(t)]

    decreasing = False
    double = False
    for i in range(len(spl)-1):
        if spl[i] > spl[i+1]:
            decreasing = True
        if spl[i] == spl[i+1]:
            double = True

    
    if not decreasing and double:
        possible += 1

print(possible)