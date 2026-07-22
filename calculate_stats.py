import pandas as pd
import numpy as np
import statsmodels.api as sm
from scipy import stats

df = pd.read_excel(r"C:\Users\pc\Downloads\AKMAL.xlsx")
cols = df.columns
# Mapping columns
X1_cols = cols[4:8]
X2_cols = cols[8:11]
Y_cols = cols[11:14]

df['Total_X1'] = df[X1_cols].sum(axis=1)
df['Total_X2'] = df[X2_cols].sum(axis=1)
df['Total_Y'] = df[Y_cols].sum(axis=1)

print("=== HASIL UJI NYATA (REAL) BERDASARKAN DATA AKMAL.xlsx ===")

# Validity (Pearson)
def pearson_validity(df, item_cols, total_col):
    res = []
    for c in item_cols:
        r, p = stats.pearsonr(df[c], df[total_col])
        res.append(f"{c[:10]}...: r={r:.3f}, p={p:.3f} ({'Valid' if p<0.05 else 'Tidak Valid'})")
    return "\n".join(res)

print("\n1. Uji Validitas (Pearson):")
print("Kinerja Sistem (X1):")
print(pearson_validity(df, X1_cols, 'Total_X1'))
print("Kestabilan Transaksi (X2):")
print(pearson_validity(df, X2_cols, 'Total_X2'))
print("Pengalaman Pengguna (Y):")
print(pearson_validity(df, Y_cols, 'Total_Y'))

# Reliability (Cronbach's Alpha)
def cronbach_alpha(df, item_cols):
    item_vars = df[item_cols].var(axis=0, ddof=1)
    t_var = df[item_cols].sum(axis=1).var(ddof=1)
    n_items = len(item_cols)
    if t_var == 0: return 0
    return (n_items / (n_items - 1)) * (1 - (item_vars.sum() / t_var))

print("\n2. Uji Reliabilitas (Cronbach's Alpha):")
print(f"X1 Alpha: {cronbach_alpha(df, X1_cols):.3f}")
print(f"X2 Alpha: {cronbach_alpha(df, X2_cols):.3f}")
print(f"Y Alpha: {cronbach_alpha(df, Y_cols):.3f}")

# Regression
X = df[['Total_X1', 'Total_X2']]
X = sm.add_constant(X)
Y = df['Total_Y']
model = sm.OLS(Y, X).fit()

print("\n3. Regresi Linear Berganda:")
print(f"R-squared: {model.rsquared:.3f}")
print(f"F-statistic: {model.fvalue:.3f}, p-value: {model.f_pvalue:.3f}")
print("Coefficients:")
for name, coef, pval, tval in zip(model.model.exog_names, model.params, model.pvalues, model.tvalues):
    print(f"{name}: Beta={coef:.3f}, t={tval:.3f}, p={pval:.3f}")

# Normalitas (Shapiro-Wilk) pada residual
residuals = model.resid
stat, p_sw = stats.shapiro(residuals)
print(f"\n4. Uji Normalitas Residual (Shapiro-Wilk): p-value = {p_sw:.3f}")

# Multikolinearitas (VIF)
from statsmodels.stats.outliers_influence import variance_inflation_factor
vif_x1 = variance_inflation_factor(X.values, 1)
vif_x2 = variance_inflation_factor(X.values, 2)
print(f"\n5. Uji Multikolinearitas (VIF): X1={vif_x1:.3f}, X2={vif_x2:.3f}")

# Heteroskedastisitas (Uji Glejser)
abs_resid = np.abs(residuals)
model_glejser = sm.OLS(abs_resid, X).fit()
print("\n6. Uji Heteroskedastisitas (Uji Glejser):")
print(f"Total_X1 p-value: {model_glejser.pvalues['Total_X1']:.3f}")
print(f"Total_X2 p-value: {model_glejser.pvalues['Total_X2']:.3f}")
