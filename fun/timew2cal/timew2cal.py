import sys, json
from datetime import datetime
from datetime import timedelta
import applescript

scpt = applescript.AppleScript('''
on run {y0, m0, d0, h0, min0, y1, m1, d1, h1, min1, s}
set theStartDate to (current date)
tell theStartDate to set its month to m0
tell theStartDate to set its day to d0
set hours of theStartDate to h0
set minutes of theStartDate to min0

set theEndDate to (current date)
tell theStartDate to set its month to m1
tell theEndDate to set its day to d1
set hours of theEndDate to h1
set minutes of theEndDate to min1

tell application "Calendar"
    tell calendar "Work"
        make new event with properties {summary:s, start date:theStartDate, end date:theEndDate}
    end tell
end tell
end run
                               ''')
def todt(s):
    dt = datetime.fromisoformat(s)
    return dt


def fromdt(dt):
    """
    dt -> (y,m,d,h,m)
    """
    fts = ['%Y', '%m', '%d', '%H', '%M']
    return [dt.strftime(ft) for ft in fts]

tzone=timedelta(hours=8)
data = json.load(sys.stdin)
for it in data:
    ts = ' '.join(it['tags'])
    t0, t1 = todt(it['start']), todt(it['end'])
    t0 += tzone
    t1 += tzone
    print(t0, t1, ts)
    #t0, t1 = it['start'], it['end']
    y0, m0, d0, h0, min0 = fromdt(t0)
    y1, m1, d1, h1, min1 = fromdt(t1)
    #print(y0, m0, d0, h0, min0, y1, m1, d1, h1, min1, ts)
    scpt.run(y0, m0, d0, h0, min0, y1, m1, d1, h1, min1, ts)
