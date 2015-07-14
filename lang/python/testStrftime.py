import datetime

def strfdelta(tdelta, fmt):
    """
    >>> dt = datetime.timedelta(microseconds=50)
    >>> print strfdelta(dt, "{ms:03}ms")
    050ms
    """
    d = {"days": tdelta.days}
    d["hours"], rem = divmod(tdelta.seconds, 3600)
    d["minutes"], d["seconds"] = divmod(rem, 60)
    d["ms"] = tdelta.microseconds
    return fmt.format(**d)
