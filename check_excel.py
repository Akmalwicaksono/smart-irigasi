import pandas as pd
import sys

try:
    df = pd.read_excel(r"C:\Users\pc\Downloads\AKMAL.xlsx")
    print("Columns:", list(df.columns))
    print("Data sample:")
    print(df.head())
except Exception as e:
    print("Error reading excel:", e)
    sys.exit(1)
