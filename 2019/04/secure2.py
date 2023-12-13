
possible = 0

for t in range(206938, 679129):
    spl = [int(y) for y in str(t)]

    decreasing = False
    double = False
    for i in range(len(spl)-1):
        if spl[i] > spl[i+1]:
            decreasing = True
        if spl[i] == spl[i+1] and (i == 0 or (i > 0 and spl[i-1] != spl[i])) and (i == len(spl)-2 or (i < len(spl) -2 and spl[i] != spl[i+2])):
            double = True

    
    if not decreasing and double:
        possible += 1

print(possible)