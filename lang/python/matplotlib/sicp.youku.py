#! /usr/bin/env python2
#-*- encoding=utf8 -*-
data = []
with open("sicp.youku.txt") as fp:
    for line in fp.readlines():
        line = line.split()
        data.append((line[0], int(line[2])))
print data
data = zip(*data)
name = data[0]
nums = data[1]

from pylab import *
# xkcd style plot
xkcd()
# need Humor Sans font to support xkcd-style font

# from palettable.colorbrewer.qualitative import Dark2_8

fig, ax = subplots()
# ax.set_color_cycle(Dark2_8.mpl_colors)

red = '#dc322f'
blue = '#268bd2'
# ax.semilogy(nums, color=red)
ax.plot(nums, color=red)
ax.xaxis.set_ticklabels(name)
title('SICP youku', color=blue)
xlabel('Sets')
ylabel('Views')
# ax.axis["bottom"].major_ticklabels.set_visible(False)
# ax.axis["bottom"].set_visible(False)
# show()
# tight to avoid white spaces
savefig('sicp.youku.png', bbox_inches='tight')
