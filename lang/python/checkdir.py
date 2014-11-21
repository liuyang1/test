#-*- encoding=utf8 -*-

"""
检查文件夹是否存在,如果不存在则创建.
如果已经存在,那么清除该文件夹下的所有内容
但是,这种方法,会删除原有文件夹,如果其他程序在这个路径下等待
,则会出现问题
"""

import os
import shutil
def checkDir(pathname):
    if os.path.exists(pathname):
        shutil.rmtree(pathname)
    os.mkdir(pathname)

# 这个代码没有上面存在的问题
def checkDir2(pathname):
    import glob
    # 注意:这里不存在上面的问题,就是原有的路径不会删除
    files = glob.glob(pathname + "/*")
    for f in files:
        if os.path.isfile(f):
            os.remove(f)
        if os.path.isdir(f):
            shutil.rmtree(f)

checkDir2("/tmp/123")
