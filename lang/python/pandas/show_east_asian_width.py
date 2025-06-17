import pandas as pd
pd.set_option("display.unicode.east_asian_width", True)
df = pd.DataFrame([["上海", 11],
                   ["安徽省", 22]], columns=["省", "Q"])
print(df)
