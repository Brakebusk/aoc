state = open("input.txt").read().split(",")
state = list(map(int, state))

state[1] = 12
state[2] = 2

pc = 0
while pc < len(state):
    oc = state[pc]
    if oc == 1:
        #add
        r1 = state[pc+1]
        r2 = state[pc+2]
        rd = state[pc+3]
        print(f"Add: {r1} and {r2} and put res in {rd}")

        state[rd] = state[r1] + state[r2]
    elif oc == 2:
        #mul
        r1 = state[pc+1]
        r2 = state[pc+2]
        rd = state[pc+3]
        print(f"Mul: {r1} and {r2} and put res in {rd}")

        state[rd] = state[r1] * state[r2]
    elif oc == 99:
        #halt
        print("Halt opcode encountered")
        break
    else:
        #error
        print(f"Error at pc {pc}")

    pc += 4

print(state[0])