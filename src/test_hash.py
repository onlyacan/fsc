import time

tb = {}
t = time.clock()
print 'install'
for i in range(1000000):
    pair = (i,i)
    tb[pair] = float(i*i)

print "time:", time.clock() - t
t = time.clock()

print 'search'
for i in range(1000000):
    pair = (i,i)
    if float(i*i) != tb[pair]:
        print "fail"

print "time:", time.clock() - t
t = time.clock()

