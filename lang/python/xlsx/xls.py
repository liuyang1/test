"""
test on xlsxwriter
"""

import xlsxwriter

workbook = xlsxwriter.Workbook('/home/liuyang/ex/test.xlsx')
worksheet = workbook.add_worksheet()

header = ('hash', 'author', 'date', 'module', 'sub module', 'commit',
          # 'description', 'root cause', 'solution', 'self test', 'impacts',
          'description',)
          # 'review-on', 'reviewer')
boldFmt = workbook.add_format({'bold': 1})
dateFmt = workbook.add_format({'num_format': 'mmmm d yyyy'})
wrapFmt = workbook.add_format({'text_wrap': 1})
vcenFmt = workbook.add_format({'valign': 'vcenter'})
wrapvcenFmt = workbook.add_format({'text_wrap': 1, 'valign': 'vcenter'})


def wrHeader(ws, row, header):
    col = 0
    for i in header:
        ws.write_string(row, col, i, boldFmt)
        col += 1


def wrItem(ws, row, item):
    col = 0
    for i in item:
        if i.startswith('http://'):
            ws.write_url(row, col, i)
        # elif len(i) > 50:
        else:
            ws.write_string(row, col, i, wrapvcenFmt)
        # else:
            # ws.write_string(row, col, i)
        col += 1

wrHeader(worksheet, 0, header)
item0 = ('bd096d4', 'aaaaa', '2015-01-09', 'mdl', 'sub',
         'this is a test string\n'
         'this is another test string\n'
         )
wrItem(worksheet, 1, item0)
worksheet.set_column(5, 5, 30)
worksheet.set_column(6, 6, 70)
worksheet.set_column(8, 8, 20)
worksheet.freeze_panes(1, 0)
workbook.close()
