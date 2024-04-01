# libraries
import matplotlib.pyplot as plt
import sys

pts = {}

with open(sys.argv[1], 'r') as f:
    for line in f.readlines():
        ipc, a, b = line.split()
        if ipc not in pts:
            pts[ipc] = []
        pts[ipc].append((float(a.strip()), float(b.strip())))

for ipctype in pts:
    pts[ipctype].sort()

for ipctype in pts:
    x = [i[0] for i in pts[ipctype]]
    y = [i[1] for i in pts[ipctype]]

    plt.plot(x, y, label=ipctype)
# create data
# use the plot function
# plt.plot(x, y)
plt.legend()
plt.show()  # display
