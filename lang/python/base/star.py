def func(*args, **kwargs):
    print args
    print kwargs

if __name__ == "__main__":
    func(1, 2, 3, test=1)
