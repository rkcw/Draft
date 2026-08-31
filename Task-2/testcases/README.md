# Test Cases

Each subfolder is one automated test. Run all of them from the `Task-2` folder:

```powershell
powershell -ExecutionPolicy Bypass -File run_tests.ps1   # Windows
bash run_tests.sh                                         # macOS / Linux
```

The runner rebuilds `main.exe` first, so tests always exercise the current code.

## What each test contains

| File | Purpose |
|------|---------|
| `state.txt` | A board state, loaded with the `LOAD` menu option (see `example_state.txt` for the format). |
| `input.txt` | The exact stdin script fed to the game (e.g. `LOAD`, then `HIGH`, `PLAY ...`, `QUIT`). |
| `output.txt` | The **exact output** produced by the reference solution for that `input.txt`. The solution file is not given to students, so this is the reference to compare your program's output against. |
| `expected.txt` | The automated checks derived from `output.txt`. One required substring per line; a leading `!` means that substring must **not** appear. Lines starting with `#` are comments. |

The `state.txt` path inside `input.txt` is relative to the `Task-2` folder (where the game must run so it can find `lib/CSW24.txt`).

## The test cases

| Test | What it checks |
|------|----------------|
| `first_move` | `HIGH` on an empty board with rack `JET` suggests a first move through the centre scoring 20. |
| `mid_game_high` | `HIGH` on the example board with a single `E` suggests `BE M2 V` for 8. |
| `mid_game_play` | Playing `BE M2 V` is accepted and scores 8 (413 → 421). |
| `invalid_play` | Playing `YEA L11 V` is rejected (it would merge with the `R` at L10 into `RYEA`). |
| `wildcard_play` | Playing `CAT` with rack `*AT` shows the wildcard `C` lowercase on the board and scores 10. |
| `exchange_invalid` | Exchanging a tile not in the rack (`Z` vs rack `ABC`) is rejected. |
| `anagram_ab` | `ANAG` on rack `AB` prints `AB BA`. |

## Adding a new test

1. Create a new folder under `testcases/`, e.g. `testcases/my_case/`.
2. Add `state.txt` (the board position) and `input.txt` (the commands).
3. Run it once manually to capture the real output, verify it is correct, then write `expected.txt` with the key lines that must (or must not, with `!`) appear.
4. Re-run `run_tests.ps1`.
