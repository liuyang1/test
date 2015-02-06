import math


# efficency:
# origin data: 16 * 8bit
# encode data: 2 * 8bit(mean, omiga), 16bit encode data
#              4 * 8bit
# ratio:       25%
def enBlk(lst):
    m = len(lst)
    mean = sum(lst) / m
    omiga2 = sum([(i - mean) ** 2 for i in lst]) / m
    omiga = math.sqrt(omiga2)
    enlst = [1 if i > mean else 0 for i in lst]
    return mean, omiga, enlst


def deBlk(mean, omiga, enlst):
    m = len(enlst)
    q = sum(enlst)
    factor = math.sqrt(q / (m - q))
    a = mean - omiga * factor
    b = mean + omiga / factor
    delst = [a if i == 1 else b for i in enlst]
    return delst
