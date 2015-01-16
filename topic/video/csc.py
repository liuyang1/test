#! /usr/bin/env python3
"""
CSC: Color Space Convert
ref: [[http://en.wikipedia.org/wiki/YUV]]
"""
RGB2YUVMat = [[0.299, 0.587, 0.114],
              [-0.147, -0.289, 0.436],
              [0.615, -0.515, -0.100]]
RGB2YCbCrMat = [[0.299, 0.587, 0.114],
                [-0.169, -0.331, 0.499],
                [0.499, -0.418, -0.0813]]
YCbCr2RGBMat = [[1, 0, 1.402],
                [1, -0.344, -0.714],
                [1, 1.772, 0]]
RGB2YCbCrBias = [0, 128, 128]
YCbCr2RGBBias = [0, -128, -128]


def clampByte(v):
    if (v < 0):
        return 0
    elif (v > 255):
        return 255
    else:
        return v


def dotProd(v0, v1):
    return sum([a * b for a, b in zip(v0, v1)])


def vecAdd(v0, v1):
    return [a + b for a, b in zip(v0, v1)]


def MatVec(m, v):
    lst = [0 for i in range(len(m))]
    for i, vv in enumerate(m):
        lst[i] = dotProd(vv, v)
    return lst


def RGB2YUV_ntsc(*rgb):
    ret = MatVec(RGB2YUVMat, rgb)
    return ret


def RGB2YCbCr_itu(*RGB):
    ret = MatVec(RGB2YCbCrMat, RGB)
    return ret


def YCbCr2RGB_itu(*YCbCr):
    ret = MatVec(YCbCr2RGBMat, YCbCr)
    ret = [clampByte(i) for i in ret]
    return ret


def YCbCr2RGB_shift(*YCbCr):
    ret = YCbCr
    Y, Cb, Cr = ret[0], ret[1], ret[2]
    R = Y + Cr + (Cr >> 2) + (Cr >> 3) + (Cr >> 5)
    G = Y - ((Cb >> 2) + (Cb >> 4) + (Cb >> 5)) - \
        ((Cr >> 1) + (Cr >> 3) + (Cr >> 4) + (Cr >> 5))
    B = Y + Cb + (Cb >> 1) + (Cb >> 2) + (Cb >> 6)
    return [R, G, B]


def test():
    rgb = (255, 255, 255)
    print(rgb)
    print(RGB2YUV_ntsc(*rgb))
    print(RGB2YCbCr_itu(*rgb))
    yuv = (255, 0, 0)
    print(yuv)
    print(YCbCr2RGB_itu(*yuv))
    print(YCbCr2RGB_shift(*yuv))

if __name__ == "__main__":
    test()
