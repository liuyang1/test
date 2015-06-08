import numpy as np
import matplotlib.pyplot as plot

x=np.linspace(0,10,1000)
y=np.sin(x)
z=np.cos(x**2)

plot.figure(figsize=(8,4))
plot.plot(x,y,"r-",label="$sin(x)$",color="red",linewidth=2)
plot.plot(x,z,"b--",label="$cos(x^2)$")
plot.xlabel("Times(s)")
plot.ylabel("Volt")
plot.title("PyPlot first example")
plot.ylim(-1.2,1.2)
plot.legend()
plot.show()
