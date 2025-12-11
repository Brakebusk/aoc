data = open('input').read().strip().split('\n')

graph = {}
for line in data:
    start, cons = line.split(": ")
    graph[start] = cons.split(" ")

queue = ['you']

p1 = 0
for p in queue:
    if p == 'out':
        p1 += 1
    else:
        for i in graph[p]:
            queue.append(i)
    
print(p1)

cache = {}
def solve(s,e,skip):
    res = 0
    if s == e:
       return 1
    else:
        for i in graph[s]:
            if (i,e,skip) in cache:
                res += cache[(i,e,skip)]
            elif i not in skip:
                res += solve(i,e,skip)
    cache[(s,e,skip)] = res
    return res

a = (solve('svr','dac',('out','fft')))
b = (solve('svr','fft',('out','dac')))
c = (solve('dac','fft',('out','svr')))
d = (solve('fft','dac',('out','svr')))
e = (solve('fft','out',('svr','dac')))
f = (solve('dac','out',('fft','svr')))
print(max(b*d*f,a*c*e))