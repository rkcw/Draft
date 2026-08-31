# Test Cases

Each subfolder is one automated test. Run all of them from the `Task-3` folder:

```powershell
powershell -ExecutionPolicy Bypass -File run_tests.ps1   # Windows
bash run_tests.sh                                         # macOS / Linux
```

The runner rebuilds `main.exe` first, so tests always exercise the current code.

## What each test contains

| File | Purpose |
|------|---------|
| `input.txt` | The exact stdin script fed to the program (menu choice, values, matrices — one per line). |
| `output.txt` | The **exact output** produced by the reference solution for that `input.txt`. The solution file is not given to students, so this is the reference to compare your program's output against. |
| `expected.txt` | The automated checks derived from `output.txt`. One required substring per line; a leading `!` means that substring must **not** appear. Lines starting with `#` are comments. |

## The test cases

| Test | What it checks |
|------|----------------|
| `rolling_basic` | Rolling average of `1..5` with window 3. |
| `rolling_window_one` | Window 1 — every average is the reading itself. |
| `rolling_wide_window` | Window larger than the data — averages grow over the full prefix. |
| `kalman_constant` | Constant measurements — the estimate converges toward them. |
| `kalman_noisy` | The example from the README — the estimate blends toward each measurement. |
| `convolve_sharpen` | 3×3 input with a 3×3 sharpen kernel → a single 5.00 output cell. |
| `convolve_box` | 5×5 input with a 3×3 all-ones kernel → 3×3 sum-of-window output. |
| `convolve_identity` | Identity kernel → output identical to the input. |
| `median_outlier` | A lone `9` outlier is removed by a 3×3 median filter. |
| `median_edges` | 5×5 sequential grid — also exercises zero padding at the borders. |

## Adding a new test

1. Create a new folder under `testcases/`, e.g. `testcases/my_case/`.
2. Add `input.txt` (the exact stdin script).
3. Run it once manually to capture the real output, verify it is correct, then write `expected.txt` with the key lines that must (or must not, with `!`) appear.
4. Re-run `run_tests.ps1`.
