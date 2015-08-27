import sqlite3
import json


rootdir = "./"
db = rootdir + "db/gering.db"
table = "clst"
sqlRecent10 = "SELECT * FROM %s ORDER BY id DESC LIMIT 10;" % (table)
sqlInsertSQL = "INSERT INTO %s (clnum, json) VALUES (?,?);" % (table)
sqlMaxid = "SELECT * FROM %s ORDER BY id DESC LIMIT 1;" % (table)
sqlDelAll = "DELETE FROM %s;" % (table)


class openDB():

    def __init__(self, db):
        self.db = db

    def __enter__(self):
        self.conn = sqlite3.connect(self.db)
        self.csr = self.conn.cursor()
        return self.conn, self.csr

    def __exit__(self, typ, value, trackback):
        self.csr.close()


def insertHist(clnum, jsonobj):
    with openDB(db) as (conn, csr):
        csr.execute(sqlInsertSQL, (clnum, json.dumps(jsonobj)))
        newid = csr.lastrowid
        conn.commit()
        return newid


def getRecent10():
    with openDB(db) as (_, csr):
        csr.execute(sqlRecent10)
        return csr.fetchall()


def getMaxID():
    with openDB(db) as (_, csr):
        csr.execute(sqlMaxid)
        try:
            ret = csr.fetchone()[0]
        except TypeError:
            ret = 0
        return ret


def delAll():
    with openDB(db) as (conn, csr):
        csr.execute(sqlDelAll)
        conn.commit()
        return True


def testInsertHist():
    print insertHist(100, {"test": 1})


def testGetRecent10():
    print getRecent10()


def testGetMaxID():
    print getMaxID()


def testDelAll():
    print "delete all", delAll()


def test():
    # testInsertHist()
    testGetRecent10()
    testGetMaxID()
    testDelAll()


if __name__ == "__main__":
    test()
