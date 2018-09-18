#! /usr/bin/env python3
def applyRule(rule, s):
    a, b = rule[0], rule[1]
    if a == s[0]:
        return b + s[1:]
    return None


def applyRules(rules, ss):
    ret = [applyRule(r, s) for r in rules for s in ss]
    return [i for i in ret if i is not None]


def search(rules, m):
    """
    search from 'a', extend it one step with all rules,
        until to specific length
    """
    ret = ['a']
    for i in range(m - 1):
        ret = applyRules(rules, ret)
    return len(ret)


def main():
    line = [int(i) for i in input().split()]
    m, n = line[0], line[1]
    rules = []
    for i in range(n):
        line = input().split()
        line.reverse()
        rules.append(line)
    l = search(rules, m)
    print(l)


if __name__ == "__main__":
    main()
