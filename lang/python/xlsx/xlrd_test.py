#! /usr/bin/env python2
import xlrd


def readXlsx(fn):
    bk = xlrd.open_workbook(fn)
    sht = bk.sheet_by_index(0)
    for i in range(1, sht.nrows):
        line = sht.row_values(i)
        print line


if __name__ == "__main__":
    readXlsx('test.xlsx')
