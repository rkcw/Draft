# Mini Scrabble test runner.
#
# For each folder in testcases/ :
#   state.txt      - board state loaded via the LOAD menu option
#   input.txt      - the exact stdin script fed to the game
#   output.txt     - the exact output of the reference solution for input.txt,
#                    so students can see the intended output (solution not given)
#   expected.txt   - one required output substring per line; a line starting
#                    with '!' is a substring that must NOT appear. Lines
#                    starting with '#' are comments and are ignored.
#
# Run from the Task-2 folder:
#   powershell -ExecutionPolicy Bypass -File run_tests.ps1

$ErrorActionPreference = 'Stop'

# Always rebuild so the tests exercise the current code.
Write-Host "Building main.exe ..."
gcc main.c Task2sol.c given.c lib/dict.c -o main "-Wl,--stack,2097152"
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

    # Feed the scripted input to the game and capture everything it prints.
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
