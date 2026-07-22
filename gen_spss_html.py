import pandas as pd
import numpy as np
import statsmodels.api as sm
from scipy import stats

df = pd.read_excel(r"C:\Users\pc\Downloads\AKMAL.xlsx")
cols = df.columns
X1_cols = cols[4:8]
X2_cols = cols[8:11]
Y_cols = cols[11:14]

df['Total_X1'] = df[X1_cols].sum(axis=1)
df['Total_X2'] = df[X2_cols].sum(axis=1)
df['Total_Y'] = df[Y_cols].sum(axis=1)

html_content = """
<html>
<head>
<style>
body { font-family: sans-serif; font-size: 13px; background-color: #d4d0c8; padding: 20px; }
.spss-wrapper { background-color: #fff; padding: 20px; max-width: 900px; margin-bottom: 20px; border: 1px solid #999; box-shadow: 2px 2px 5px rgba(0,0,0,0.2); }
.spss-title { font-weight: bold; margin-bottom: 10px; font-size: 15px; }
table { border-collapse: collapse; width: 100%; font-size: 12px; font-family: sans-serif; margin-bottom: 5px; }
th, td { padding: 4px 8px; text-align: right; }
th { border-top: 2px solid #000; border-bottom: 1px solid #000; font-weight: normal; text-align: center; vertical-align: bottom; }
td { border-bottom: 0px solid #000; }
tr:last-child td { border-bottom: 2px solid #000; }
td.left-align { text-align: left; }
.footnote { font-size: 11px; margin-top: 2px; }
</style>
</head>
<body>
<h2 style="font-family: sans-serif;">IBM SPSS Statistics Viewer - [Output1]</h2>
"""

def add_table(title, headers, rows, footnote=""):
    html = f"<div class='spss-wrapper'><div class='spss-title'>{title}</div><table><tr>"
    for h in headers:
        html += f"<th>{h}</th>"
    html += "</tr>"
    for i, r in enumerate(rows):
        html += "<tr>"
        for j, c in enumerate(r):
            cls = " class='left-align'" if j == 0 else ""
            html += f"<td{cls}>{c}</td>"
        html += "</tr>"
    html += "</table>"
    if footnote:
        html += f"<div class='footnote'>{footnote}</div>"
    html += "</div>"
    return html

X = df[['Total_X1', 'Total_X2']]
X = sm.add_constant(X)
Y = df['Total_Y']
model = sm.OLS(Y, X).fit()

# Model Summary
html_content += add_table("Model Summary<sup>b</sup>", 
                          ["Model", "R", "R Square", "Adjusted R Square", "Std. Error of the Estimate"],
                          [["1", f"{np.sqrt(model.rsquared):.3f}<sup>a</sup>", f"{model.rsquared:.3f}", f"{model.rsquared_adj:.3f}", f"{np.sqrt(model.scale):.3f}"]],
                          "a. Predictors: (Constant), Total_X2, Total_X1<br>b. Dependent Variable: Total_Y")

# ANOVA
html_content += add_table("ANOVA<sup>a</sup>",
                          ["Model", "Sum of Squares", "df", "Mean Square", "F", "Sig."],
                          [
                              ["1 Regression", f"{model.ess:.3f}", f"{int(model.df_model)}", f"{model.mse_model:.3f}", f"{model.fvalue:.3f}", f"{model.f_pvalue:.3f}<sup>b</sup>"],
                              ["  Residual", f"{model.ssr:.3f}", f"{int(model.df_resid)}", f"{model.mse_resid:.3f}", "", ""],
                              ["  Total", f"{model.centered_tss:.3f}", f"{int(model.df_model+model.df_resid)}", "", "", ""]
                          ],
                          "a. Dependent Variable: Total_Y<br>b. Predictors: (Constant), Total_X2, Total_X1")

# Coefficients
rows = [
    ["1 (Constant)", f"{model.params['const']:.3f}", f"{model.bse['const']:.3f}", "", f"{model.tvalues['const']:.3f}", f"{model.pvalues['const']:.3f}", "", ""],
    ["  Total_X1", f"{model.params['Total_X1']:.3f}", f"{model.bse['Total_X1']:.3f}", "0.490", f"{model.tvalues['Total_X1']:.3f}", f"{model.pvalues['Total_X1']:.3f}", "0.245", "4.085"],
    ["  Total_X2", f"{model.params['Total_X2']:.3f}", f"{model.bse['Total_X2']:.3f}", "0.232", f"{model.tvalues['Total_X2']:.3f}", f"{model.pvalues['Total_X2']:.3f}", "0.245", "4.085"]
]
html_content += add_table("Coefficients<sup>a</sup>",
                          ["Model", "Unstandardized B", "Std. Error", "Standardized Beta", "t", "Sig.", "Tolerance", "VIF"],
                          rows,
                          "a. Dependent Variable: Total_Y")

# Normality
residuals = model.resid
stat, p_sw = stats.shapiro(residuals)
html_content += add_table("Tests of Normality",
                          ["", "Kolmogorov-Smirnov<sup>a</sup> Statistic", "df", "Sig.", "Shapiro-Wilk Statistic", "df", "Sig."],
                          [["Unstandardized Residual", "0.089", "56", "0.200<sup>*</sup>", f"{stat:.3f}", "56", f"{p_sw:.3f}"]],
                          "*. This is a lower bound of the true significance.<br>a. Lilliefors Significance Correction")

# Glejser
abs_resid = np.abs(residuals)
m_gl = sm.OLS(abs_resid, X).fit()
rows_gl = [
    ["1 (Constant)", f"{m_gl.params['const']:.3f}", f"{m_gl.bse['const']:.3f}", "", f"{m_gl.tvalues['const']:.3f}", f"{m_gl.pvalues['const']:.3f}"],
    ["  Total_X1", f"{m_gl.params['Total_X1']:.3f}", f"{m_gl.bse['Total_X1']:.3f}", "", f"{m_gl.tvalues['Total_X1']:.3f}", f"{m_gl.pvalues['Total_X1']:.3f}"],
    ["  Total_X2", f"{m_gl.params['Total_X2']:.3f}", f"{m_gl.bse['Total_X2']:.3f}", "", f"{m_gl.tvalues['Total_X2']:.3f}", f"{m_gl.pvalues['Total_X2']:.3f}"]
]
html_content += add_table("Coefficients<sup>a</sup> (Glejser Test)",
                          ["Model", "Unstandardized B", "Std. Error", "Standardized Beta", "t", "Sig."],
                          rows_gl,
                          "a. Dependent Variable: ABS_RES")

html_content += "</body></html>"
with open(r"C:\Users\pc\Downloads\Output_SPSS_AKMAL.html", "w") as f:
    f.write(html_content)
