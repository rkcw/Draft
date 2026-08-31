# Mini Task 1 test runner.
#
# For each folder in testcases/ :
#   input.txt      - the exact stdin script fed to the program (menu choices,
#                    passwords, emails, word lists, etc. - one per line)
#   output.txt     - the exact output of the reference solution for input.txt,
#                    so students can see the intended output (solution not given)
#   expected.txt   - one required output substring per line; a line starting
#                    with '!' is a substring that must NOT appear. Lines
#                    starting with '#' are comments and are ignored.
#
# Run from the Task-1 folder:
#   powershell -ExecutionPolicy Bypass -File run_tests.ps1
#
# By default the runner builds with the student's Task1.c. To validate the
# test harness against the reference solution instead, change Task1.c to
# Task1sol.c in the gcc line below.

$ErrorActionPreference = 'Stop'

# Always rebuild so the tests exercise the current code.
Write-Host "Building main.exe ..."
gcc main.c Task1.c -o main
if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed." -ForegroundColor Red
    exit 1
}

$testsDir = "testcases"
$passed = 0
$failed = 0

Get-ChildItem -Path $testsDir -Directory | Sort-Object Name | ForEach-Object {
    $name = $_.Name
    $inputFile = Join-Path $_.FullName "input.txt"
    $expectedFile = Join-Path $_.FullName "expected.txt"
    if (-not (Test-Path $inputFile) -or -not (Test-Path $expectedFile)) {
        Write-Host "[SKIP] $name (missing input.txt or expected.txt)" -ForegroundColor Yellow
        return
    }

    # Feed the scripted input to the program and capture everything it prints.
    $actual = (Get-Content $inputFile -Raw | & .\main.exe 2>&1 | Out-String)

    $ok = $true
    $problems = @()

    Get-Content $expectedFile | ForEach-Object {
        $line = $_.Trim()
        if ($line -eq "" -or $line.StartsWith("#")) { return }
        if ($line.StartsWith("!")) {
            $sub = $line.Substring(1).Trim()
            if ($actual.Contains($sub)) {
                $ok = $false
                $problems += "  must NOT contain: '$sub'"
            }
        }
        else {
            if (-not $actual.Contains($line)) {
                $ok = $false
                $problems += "  missing: '$line'"
            }
        }
    }

    if ($ok) {
        Write-Host "[PASS] $name" -ForegroundColor Green
        $passed++
    }
    else {
        Write-Host "[FAIL] $name" -ForegroundColor Red
        $problems | ForEach-Object { Write-Host $_ -ForegroundColor Red }
        $failed++
    }
}

Write-Host ""
Write-Host "Passed: $passed   Failed: $failed"
if ($failed -gt 0) { exit 1 }
