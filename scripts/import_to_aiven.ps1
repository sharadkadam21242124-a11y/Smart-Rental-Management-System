param(
    [string]$HostName = "mysql-2efeca24-sharadkadam21242124-beb3.g.aivencloud.com",
    [int]$Port = 14959,
    [string]$User = "avnadmin",
    [string]$Database = "defaultdb"
)

Write-Host "==========================================================" -ForegroundColor Cyan
Write-Host " Importing Smart Rental Schema & Data to Aiven MySQL 8.0  " -ForegroundColor Cyan
Write-Host "==========================================================" -ForegroundColor Cyan
Write-Host "Host: $HostName"
Write-Host "Port: $Port"
Write-Host "User: $User"
Write-Host "DB:   $Database"
Write-Host ""

$password = Read-Host "Please paste your Aiven database password" -AsSecureString
$bstr = [System.Runtime.InteropServices.Marshal]::SecureStringToBSTR($password)
$plainPass = [System.Runtime.InteropServices.Marshal]::PtrToStringAuto($bstr)

$env:MYSQL_PWD = $plainPass
$mysqlExe = "C:\Program Files\MySQL\MySQL Server 8.0\bin\mysql.exe"
$sqlFile = Resolve-Path "database\cloud_init.sql"

Write-Host "`nConnecting to Aiven and executing database\cloud_init.sql..." -ForegroundColor Yellow

Get-Content -Raw $sqlFile | & $mysqlExe -h $HostName -P $Port -u $User $Database

if ($LASTEXITCODE -eq 0) {
    Write-Host "`nSUCCESS: All 13 tables and catalog data imported to Aiven successfully!" -ForegroundColor Green
} else {
    Write-Host "`nFAILED: MySQL returned exit code $LASTEXITCODE. Please check your password and connection." -ForegroundColor Red
}

$env:MYSQL_PWD = ""

