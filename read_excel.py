import pandas as pd
try:
    df = pd.read_excel(r"C:\Users\pc\Downloads\AKMAL.xlsx")
    print("Columns:", list(df.columns))
    print("Head:\n", df.head())
except Exception as e:
    print("Error:", e)
