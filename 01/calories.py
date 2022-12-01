
# Part 1
print("part1", max(map(lambda elf: sum(map(lambda value: int(value), elf)), map(lambda line: line.splitlines(), open("input").read().split("\n\n")))))

# Part 2
print(sum(sorted(map(lambda elf: sum(map(lambda value: int(value), elf)), map(lambda line: line.splitlines(), open("input").read().split("\n\n"))))[-3:]))