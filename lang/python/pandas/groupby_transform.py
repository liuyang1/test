import pandas as pd

f = pd.DataFrame({'A' : ['foo', 'bar', 'foo', 'bar',
                          'foo', 'bar'],
                   'B' : ['one', 'one', 'two', 'three',
                          'two', 'two'],
                   'C' : [0, 1, 2, 3, 4, 5],
                   'D' : [2.0, 5., 8., 1., 2., 9.]})

f['rank'] = f.groupby('A')['D'].transform(lambda x: x.rank(ascending=False) <= len(x) * 0.5)
# r = [g.sort_values(ascending=False) for _, g in gs]

# g.transfrom(lambda x
print(f)
