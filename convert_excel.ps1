$excel = New-Object -ComObject Excel.Application
$excel.Visible = $false
$excel.DisplayAlerts = $false
try {
    $wb = $excel.Workbooks.Open("C:\Users\pc\Downloads\AKMAL.xlsx")
    $ws = $wb.Sheets.Item(1)
    $csvPath = "C:\Users\pc\Downloads\AKMAL.csv"
    $ws.SaveAs($csvPath, 6)
    $wb.Close($false)
    Write-Host "Success: File converted to AKMAL.csv"
} catch {
    Write-Host "Error: $_"
} finally {
    $excel.Quit()
    [System.Runtime.Interopservices.Marshal]::ReleaseComObject($excel) | Out-Null
}
