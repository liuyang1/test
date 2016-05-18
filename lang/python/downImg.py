import os
from ftplib import FTP


# config
host = 'abc.com'
user = 'user'
password = 'password'
project, local = 'aaa', 'bbb'
# end of config


def listcwd(f):
    lst = f.nlst('.')
    lst = [i[2:] for i in lst]  # skip first './'
    return lst


def downfile(f, src, dst=None):
    if dst is None:
        dst = src
    print "download file %s\t->\t%s" % (src, dst)
    f.retrbinary('RETR ' + src, open(dst, 'wb').write)
    print "download file %s\t->\t%s\tdone" % (src, dst)


def downdir(f, src, dst):
    if os.path.exists(dst):
        raise "dir %s already exist, quit" % (dst)
    os.mkdir(dst)
    print "download dir  %s\t->\t%s" % (src, dst)
    f.cwd(src)
    lst = listcwd(f)
    for i in lst:
        downfile(f, i, dst + '/' + i)
    print "download dir  %s\t->\t%s\tdone" % (src, dst)


def downLastest(pjt, abbr):
    """
    try to download lastest image
    PJT/[lastest No]/eMMCimg/ -> ABBR[lastest No]/
    """
    ftp = FTP(host)
    ftp.login(user, password)
    ftp.cwd(pjt)
    lst = listcwd(ftp)
    d = max(lst, key=int)  # find lastest image
    ftp.cwd(d)
    lst = listcwd(ftp)
    downdir(ftp, "eMMCimg", abbr + d)
    ftp.quit()


if __name__ == "__main__":
    downLastest(project, local)
