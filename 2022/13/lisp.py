
from functools import cmp_to_key

def compare(left, right):
  result = 0

  for i in range(min(len(left), len(right))):
    isListLeft = isinstance(left[i], list)
    isListRight = isinstance(right[i], list)

    if isListLeft and isListRight:
      result = compare(left[i], right[i])
    elif isListLeft:
      result = compare(left[i], [right[i]])
    elif isListRight:
      result = compare([left[i]], right[i])
    else:
      if left[i] > right[i]:
        result = 1
      elif left[i] == right[i]:
        result = 0
      else:
        result = -1

    if result:
      return result

  if len(left) > len(right):
    return 1
  elif len(left) == len(right):
    return 0
  else:
    return -1

packets = [eval(line) for line in open("input").read().split("\n") if len(line)]

leftPackets = packets[::2]
rightPackets = packets[1::2]

ordered = 0

for p in range(len(leftPackets)):
  left = leftPackets[p]
  right = rightPackets[p]
  if compare(left, right) < 0:
    ordered += (p + 1)

print("Part 1:", ordered)

packets.append([[2]])
packets.append([[6]])

packets.sort(key=cmp_to_key(compare))

d1 = 0
d2 = 0

for p in range(len(packets)):
  if compare(packets[p], [[2]]) == 0:
    d1 = p + 1
  elif compare(packets[p], [[6]]) == 0:
    d2 = p + 1

print("Part 2:", d1 * d2)