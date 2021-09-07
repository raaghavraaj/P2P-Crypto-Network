import matplotlib.pyplot as plt
import numpy as np

a = []
file =  open('inp2.txt', 'r')
lines = file.readlines()
for line in lines:
    nums = line.split(' ')
    for x in nums:
        # print(x)
        a.append(int(x))

bin_size = 10
print(a)
plt.hist(a, bins = int((max(a)-min(a))/bin_size))
plt.xlabel('Branch Length')
plt.ylabel('Number of Branches')
plt.show()
