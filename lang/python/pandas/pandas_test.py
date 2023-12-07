import pandas as pd

# how to create dataframe
data = [[1, 2, 3],
        [4, 5, 6],
        [10, 11, 12],
        [5, 6, 7],
        [7, 8, 9]]
df = pd.DataFrame(data, columns=['col0', 'col2', 'col3'])

# how to select rows by  list of boolean
arr = [True, True, False, True]
print(df.loc[arr])

# how to get mean of columns
avg = df.loc[arr][['col0', 'col3']].mean(axis=0)
avg['sum'] = avg['col0'] + avg['col3']
print(avg)
print(avg / avg['col0'] * 100)

median = df.median()
print("median:", median)
