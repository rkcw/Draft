# Task 3 (Mathematical Models)

## Focus/Aims

This task will focus on testing your skills on:

- Basic C (loop and if conditions)
- 2D Array Manipulation
- Structs
- Pointers

This task is mainly focused ~~(loosely based)~~ on **mathematical models for signal and image processing**. (Similar concepts will be very useful when you deal with sensors, cameras, or control systems later on.) We will implement three classic tools: a **rolling average**, a **Kalman filter**, and **kernel convolution**.


## Given Structures

Both structs below are provided in `Task3.h` and are used throughout the task. They are given as-is — do not change their fields.

### Matrix

A fixed-capacity 2D array of `double`s, plus its dimensions. `matrix->data[r][c]` is the value at row `r`, column `c` (both 0-based).

```c
#define MAX_DIM 16

typedef struct{
    double data[MAX_DIM][MAX_DIM];
    int rows; //number of valid rows (1..MAX_DIM)
    int cols; //number of valid columns (1..MAX_DIM)
} Matrix;
```

### Kalman

The state of a one-dimensional Kalman filter.

```c
typedef struct{
    double estimate;  //current state estimate x
    double error_cov; //current error covariance P
} Kalman;
```

## Tasks

All the functions you must write are in `Task3.c` — each one is a stub with a `//TODO for the student.` comment. Implement them in that file.

Rules:

- You **may** add your own helper functions inside `Task3.c`.
- You **must NOT** change the function signatures declared in `Task3.h`.
- You are **NOT allowed** to add any other `#include` libraries — the existing `stdio.h` is all you get.

### Part A

#### i) `void rolling_average(double* data, int n, int window)`

We will start with a warmup.

The rolling average (also called a moving average) is a classic way to smooth out a noisy stream of measurements. Instead of using every single reading on its own, we average each reading together with the few readings that came just before it.

`data` is an array of `n` sensor readings, and `window` is how many readings go into each average. The **rolling average at index `i`** is the average of the last `window` readings **ending at `i`**, i.e. `data[i-window+1 .. i]`.

- If there are fewer than `window` readings available (i.e. `i < window-1`), just average the readings that *are* available, i.e. `data[0 .. i]`.
- The function should print a header line, followed by one line per reading: the index, the reading, and its rolling average, all to **2 decimal places**.

Output example (for `data = {1, 2, 3, 4, 5}`, `n = 5`, `window = 3`):

```console
Rolling average (window 3):
Index  Reading   Average
0      1.00      1.00
1      2.00      1.50
2      3.00      2.00
3      4.00      3.00
4      5.00      4.00
```

##### Assumptions (3A(i))

- `n >= 1` and `window >= 1`.
- All readings are valid doubles.

#### ii) `void kalman_init(Kalman* k, double estimate, double error_cov)` and `double kalman_step(Kalman* k, double measurement, double process_noise, double measurement_noise)`

The rolling average smooths a signal, but it treats every reading equally. The **Kalman filter** is smarter: it keeps track of both a current *estimate* of the true value and how *confident* we are in that estimate. When a new, noisy measurement arrives, the filter blends the measurement with its current estimate, weighting each side by how trustworthy it thinks it is.

The struct `Kalman` holds the two pieces of state:

- `estimate` — our current best guess of the true value ($\hat{x}$).
- `error_cov` — our current uncertainty, the error covariance ($P$). Bigger = less confident.

**`kalman_init`** should simply store the starting estimate and error covariance into the struct pointed to by `k`.

**`kalman_step`** should perform **one full filter iteration** (predict + update) using the measurement `measurement`, and return the new estimate. The 1D equations are:

**Prediction** (the state itself is static, only the uncertainty grows by the process noise $Q$):

$$
\begin{aligned}
\hat{x}_{pred} &= \hat{x} \\
P_{pred} &= P + Q
\end{aligned}
$$

**Update** (fuse the measurement $z$ in, weighted by the Kalman gain $K$):

$$
\begin{aligned}
K &= \frac{P_{pred}}{P_{pred} + R} \\
\hat{x} &= \hat{x}_{pred} + K\,(z - \hat{x}_{pred}) \\
P &= (1 - K)\,P_{pred}
\end{aligned}
$$

where `process_noise` is $Q$ and `measurement_noise` is $R$.

Output example (for `initial estimate = 0`, `error_cov = 1`, measurements `0.5 1.2 1.0 2.0 2.5`, `Q = 0.1`, `R = 1`):

```console
Kalman filter output:
Step 1: measurement 0.50 -> estimate 0.26
Step 2: measurement 1.20 -> estimate 0.62
Step 3: measurement 1.00 -> estimate 0.75
Step 4: measurement 2.00 -> estimate 1.12
Step 5: measurement 2.50 -> estimate 1.51
```

(The numbers above are rounded to 2 decimal places.)

> Notice how the first few estimates move quickly toward the measurements, then start trusting the measurements less and less as the filter gains confidence.

##### Assumptions (3A(ii))

- `process_noise` and `measurement_noise` are always positive (`> 0`), so division by zero will not happen.

#### iii) `int convolve(Matrix* input, Matrix* kernel, Matrix* output)`

**Convolution** is the workhorse of image processing. A small grid of weights called a **kernel** is slid over a larger grid (an image / matrix), and at each position the overlapping values are multiplied together and summed. The result is a new matrix.

The kernel is a **square** grid of odd size `k` (e.g. 3×3, 5×5). The **output** value at `(r, c)` is the sum over the whole kernel of the input values overlapping it:

$$
output(r,c) = \sum_{i=0}^{k-1} \sum_{j=0}^{k-1} input(r+i,\ c+j) \times kernel(i,j)
$$

In other words, we place the top-left corner of the kernel on `input[r][c]` and add up the element-wise products. (For this task we apply the kernel **directly** — we do not flip it.)

Because the kernel must fit fully inside the input, the output is smaller than the input:

$$
output\_rows = input\_rows - k + 1 \qquad output\_cols = input\_cols - k + 1
$$

The function should:

- Check that `kernel` is **square, odd-sized, and small enough** to fit inside `input` (i.e. `k <= input->rows` and `k <= input->cols`). If not, return `0` and leave `output` untouched.
- Otherwise, compute the convolved result, store it in `output->data`, set `output->rows` and `output->cols`, and return `1`.

![convolution](https://images-ext-1.discordapp.net/external/1Xqdh5XWANrm4jcLKVPTEqK7M7xR-iJuNDIxziOaJtw/https/d29g4g2dyqv443.cloudfront.net/sites/default/files/pictures/2018/convolution-2.gif?width=658&height=480)

Output example (for a 3×3 input and a 3×3 sharpen kernel `0 -1 0 / -1 5 -1 / 0 -1 0`):

```console
Matrix (1 x 1):
5.00
```

##### Assumptions (3A(iii))

- `input->rows` and `input->cols` are between 1 and `MAX_DIM`, inclusive.
- The kernel is at most 5×5, so the results always fit inside `output` (which is `MAX_DIM × MAX_DIM`).

### Part B (Bonus)

#### iv) `int median_filter(Matrix* input, Matrix* output, int window)`

The rolling average is a *linear* smoother. It works great on random noise, but a single wild outlier (e.g. a dead pixel) drags the average around. The **median filter** fixes that: instead of averaging a neighbourhood, it takes the **middle value** after sorting the neighbourhood. Outliers get thrown away entirely.

For every cell `(r, c)` of the input:

1. Gather the `window × window` neighbourhood centred on `(r, c)`.
2. Sort those values.
3. Put the **middle** value into `output[r][c]`.

`window` is an odd integer (e.g. 3, 5). When the neighbourhood sticks out past the edge of the matrix, treat the out-of-bounds cells as `0` (this is called **zero padding**). The output has the **same dimensions** as the input.

The function should:

- Check that `window` is odd and positive. If not, return `0` and leave `output` untouched.
- Otherwise compute the median-filtered result, store it in `output`, and return `1`.

Output example (for the 3×3 input `1 1 1 / 1 9 1 / 1 1 1` with `window = 3`, the lone `9` outlier disappears, and the zero-padded border cells become `0.00`):

```console
Matrix (3 x 3):
0.00 1.00 0.00
1.00 1.00 1.00
0.00 1.00 0.00
```

##### Assumptions (3A(iv))

- `window` is at most `MAX_DIM`, so the neighbourhood always fits in a temporary array.

## Compiling & Testing

Compile (from the `Task-3` folder) with your `Task3.c`:

```text
gcc main.c Task3.c -o main
```

(To run the reference solution instead, swap `Task3.c` for `Task3sol.c`.)

Run the automated test suite to check your work:

```text
powershell -ExecutionPolicy Bypass -File run_tests.ps1   # Windows
bash run_tests.sh                                         # macOS / Linux
```

Each test folder in `testcases/` ships with the exact scripted input (`input.txt`) **and** the exact output produced by the reference solution (`output.txt`). Since the solution file is not given to you, use `output.txt` as the reference to compare your program's output against.

The menu options map to the functions as follows:

| Menu option | Function used |
| ------------- | --------------- |
| `1` | `rolling_average` |
| `2` | `kalman_init` + `kalman_step` |
| `3` | `convolve` |
| `4` | `median_filter` (bonus) |
