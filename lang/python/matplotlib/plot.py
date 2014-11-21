import sys

def loadData(f):
    ret = []
    for line in f.readlines():
        v = [float(i) for i in line.split()]
        ret.append(v)
    return zip(*ret)

def plot(X, Y):
    import pylab as pl
    pl.plot(X, Y)
    pl.show()


if __name__ == "__main__":
    f = open(sys.argv[1])
    data = loadData(f)
    f.close()
    plot(data[0], data[1])


