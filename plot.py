# libraries
import matplotlib.pyplot as plt

pts = []

with open('socket.log', 'r') as f:
    for line in f.readlines():
        _, a, b = line.split()
        pts.append((float(a.strip()), float(b.strip())))

pts.sort()
x = [i[0] for i in pts]
y = [i[1] for i in pts]
# create data
# use the plot function
plt.plot(x, y)
plt.show()  # display