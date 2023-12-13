init = state = open("input.txt").read().split(",")
init = list(map(int, state))

for noun in range(100):
    for verb in range(100):
        state = init.copy()

        state[1] = noun
        state[2] = verb

        pc = 0
        while pc < len(state):
            oc = state[pc]
            if oc == 1:
                #add
                r1 = state[pc+1]
                r2 = state[pc+2]
                rd = state[pc+3]
                #print(f"Add: {r1} and {r2} and put res in {rd}")

                state[rd] = state[r1] + state[r2]
            elif oc == 2:
                #mul
                r1 = state[pc+1]
                r2 = state[pc+2]
                rd = state[pc+3]
                #print(f"Mul: {r1} and {r2} and put res in {rd}")

                state[rd] = state[r1] * state[r2]
            elif oc == 99:
                #halt
                print("Halt opcode encountered")
                break
            else:
                #error
                print(f"Error at pc {pc}")

            pc += 4

        if state[0] == 19690720:
            print(f"Nound: {noun} Verb:{verb}")
            exit()