# Test Cases

Each subfolder is one automated test. Run all of them from the `Task-1` folder:

```powershell
powershell -ExecutionPolicy Bypass -File run_tests.ps1   # Windows
bash run_tests.sh                                         # macOS / Linux
```

The runner rebuilds `main.exe` first, so tests always exercise the current code.

## What each test contains

| File | Purpose |
|------|---------|
| `input.txt` | The exact stdin script fed to the program (menu choice, password / email / word list + regex — one per line). |
| `output.txt` | The **exact output** produced by the reference solution for that `input.txt`. The solution file is not given to students, so this is the reference to compare your program's output against. |
| `expected.txt` | The automated checks derived from `output.txt`. One required substring per line; a leading `!` means that substring must **not** appear. Lines starting with `#` are comments. |

## The test cases

| Test | What it checks |
|------|----------------|
| `password_valid` | A password meeting every rule is accepted. |
| `password_boundary` | 8-char and 24-char passwords are both valid (boundaries inclusive). |
| `password_invalid` | Too short / missing category / invalid character / too long are all rejected. |
| `email_simple` | A basic valid email prints its mailbox and domain. |
| `email_case` | Unquoted local-part and domain are lowercased in the output. |
| `email_quoted` | Escapes inside a quoted local-part are removed (`\"` → `"`, `\\` → `\`). |
| `email_invalid_quote` | An unescaped `"` inside a quoted local-part is invalid. |
| `email_invalid_tld` | A top-level domain with digits is invalid. |
| `email_comments` | Comments are stripped; a tag is removed from the mailbox. |
| `email_quoted_comment` | Inside quotes, `(n)` and `+` are literal, not a comment/tag. |
| `email_tag` | A `+tag` is removed from the mailbox. |
| `regex_dot` | `c.*t` matches `cat cot cut cart content`. |
| `regex_group` | `(do)+g` matches `dog dodog dododog`. |
| `regex_negated` | `c[^a]t` matches `cut cot cbt c0t` and rejects `cat`. |
| `regex_quantifier` | `b[a-z]{2,3}` matches `bat bath` only. |

## Adding a new test

1. Create a new folder under `testcases/`, e.g. `testcases/my_case/`.
2. Add `input.txt` (the exact stdin script).
3. Run it once manually to capture the real output, verify it is correct, then write `expected.txt` with the key lines that must (or must not, with `!`) appear.
4. Re-run `run_tests.ps1` (or `run_tests.sh`).
