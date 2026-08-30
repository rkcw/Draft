# Task 2 (Mini Scrabble)

## Focus/Aims

This task will focus on testing your skills on:

- Basic C (loop and if conditions)
- C String
- Structs
- Pointers

This task will be focusing on using structs and user-defined function ~~and also for fun :\)~~

## Brief summary of how Scrabble works (in case you do not know what Scrabble is (._.) )  

Scrabble is a well-known word board game, where 2 players take turns spelling out interconnecting English words on a 15x15 grid. And for this task, we will try to recreate this game in the terminal. (With some slight modifications.)

Initial game state: A blank board (15x15 grid), a bag with 100 tiles, 2 empty racks, 2 players (P1 and P2)  
At the start, the 2 players each draw 7 tiles from the bag. And they take turns putting down a valid word (a word in a specific dictionary) on the board.
The first move of the game (or in special cases, when there are no tiles on the board), the player's word must cover H8 (the middle of the 15x15 grid).
After the first move (aka there are tiles on the game board), every new word must connect to a letter already on the board. You can add letters to an existing word or build a new word that intersects with one on the board.  
All the tiles you place on a single turn must be in one continuous line (either across or down), and when you play tiles next to existing ones, every new word that is formed must be a valid word.

## Given Structures

All of the following structs and the enum are provided in `Task2.h` and used throughout the game. They are given as-is — do not change their fields.

### MOD (enum)

The type of special square on the board. `BLANK` means an ordinary square with no bonus.

| Value           | Meaning                                           |
|-----------------|---------------------------------------------------|
| `BLANK`         | No bonus (a normal square)                        |
| `DOUBLE_LETTER` | Doubles the letter value of a newly placed tile   |
| `TRIPLE_LETTER` | Triples the letter value of a newly placed tile   |
| `DOUBLE_WORD`   | Doubles the whole word's score                    |
| `TRIPLE_WORD`   | Triples the whole word's score                    |
| `START`         | The centre star; acts like a double-word square   |

### Tile

A single square on the board.

```c
typedef struct{
    char letter; // the tile's letter ('\0' if the square is empty)
    MOD mod;     // the special square type under this tile
} Tile;
```

### Board

The 15x15 playing grid. `board[r][c]` is the tile at row `r`, column `c` (both 0-based).

```c
typedef struct{
    Tile board[15][15];
} Board;
```

### Player

One player's rack and running score.

```c
typedef struct{
    char rack[8]; // the tiles held (up to 7, null-terminated; '*' = wildcard)
    int score;    // the player's current score
} Player;
```

### Bag

The bag of remaining tiles. `tiles` is a null-terminated string holding every tile still in the bag, and `tiles_left` is how many there are.

```c
typedef struct{
    char tiles[101]; // up to 100 tiles + null terminator
    int tiles_left;  // how many tiles are still in the bag
} Bag;
```

The bag will contain all the tiles in the Scrabble game, and players take turns drawing tiles from the bag.  
The starting tiles are:

- 1 point: E ×12, A ×9, I ×9, O ×8, N ×6, R ×6, T ×6, L ×4, S ×4, U ×4
- 2 points: D ×4, G ×3
- 3 points: B ×2, C ×2, M ×2, P ×2
- 4 points: F ×2, H ×2, V ×2, W ×2, Y ×2
- 5 points: K ×1
- 8 points: J ×1, X ×1
- 10 points: Q ×1, Z ×1
- 0 points: 2 wildcards (can replace any letter)

### Game

The whole game: the board, the bag, both players, and whose turn it is.

```c
typedef struct{
    Board board;   // the 15x15 board
    Bag bag;       // the remaining tiles
    Player P1;     // player 1
    Player P2;     // player 2
    int turn;      // 1 for P1, 2 for P2
} Game;
```

## Given Functions

All of these are already implemented for you in `given.c` and declared in `Task2.h`. Call them whenever you need to — you should not have to rewrite them.

| Function | What it does |
|----------|--------------|
| `void init_bag(Bag* bag)` | Fills the bag with the full 100-tile Scrabble set and sets `tiles_left` to 100. |
| `void init_game(Game* game)` | Sets up a brand-new game: turn = 1, both scores = 0, empty racks, the standard premium-square layout on the board, and draws 7 tiles into each player's rack. |
| `void draw_tile(Bag* bag, Player* player)` | Takes a random tile out of the bag and puts it into the player's rack (does nothing if the rack is full or the bag is empty). |
| `void add_tile(Bag* bag, char tile)` | Puts one tile back into the bag (used when exchanging tiles). |
| `int tiles_left(Bag* bag)` | Returns the number of tiles still in the bag. |
| `void tiles_info(Bag* bag)` | Prints all the tiles left in the bag and how many there are. |
| `int value(char letter)` | Returns the Scrabble point value of a letter; `*` (wildcard) = 0, and invalid letters = -1. |
| `int rack_count(Player* player)` | Returns how many tiles are currently on the player's rack (0-7). |
| `void unseen_tiles_info(Game* game, Player* player)` | Prints the tiles the player cannot see (the bag + the opponent's rack), sorted with wildcards last, so the opponent's exact tiles stay hidden. |
| `void print_board(Game game)` | Draws the 15x15 board with column letters and row numbers, showing placed tiles and the special squares (`+` double letter, `#` triple letter, `$` double word, `%` triple word, `@` centre). |
| `void print_turn(Game game)` | Prints the board, both players' scores, and whose turn it is. |
| `void print_rack(Player player)` | Prints the current player's rack. |
| `void import_state(Game* game, const char* filename)` | Loads a saved game state from a text file so you can debug a specific board position (see `example_state.txt`). |
| `int is_board_empty(Board* board)` | Returns 1 if the board has no tiles on it (every square empty), otherwise 0. |

## Tasks

All the functions you must write are in `Task2.c` — each one is a stub with a `//TODO for the student.` comment. Implement them in that file.

Rules:

- You **may** add your own helper functions inside `Task2.c`.
- You **must NOT** change the function signatures declared in `Task2.h`.
- You are **NOT allowed** to add any other `#include` libraries — the existing `stdio.h`, `stdlib.h` and `lib/dict.h` are all you get.
- Use `is_valid()` (from `lib/dict.h`) to check whether a word is in the dictionary, and `value()` for tile scores.

### Part A

#### 1. `void sort_rack(Player* player)`

Sort the player's rack in alphabetical order, in place. The wildcard `*` sorts **after** `Z`.

Insertion sort is the easiest:

1. Start at the 2nd tile.
2. Compare it with the tile before it, and swap if the one before is larger.
3. Keep swapping left while the tile before it is larger (or until you reach the start).
4. Move to the next tile and repeat.

(You may use any sorting algorithm you like, insertion is just the simplest.)

#### 2. `int exchange_tiles(Game* game, Player* player, const char* tiles)`

Exchange some of the player's tiles for new ones from the bag.

- Remove each requested tile from the rack and draw the same number of replacement tiles from the bag.
- If any requested tile is **not** in the player's rack, the exchange is invalid: make **no** changes and return **0**.
- Otherwise perform the exchange and return **1**.

> Tip: `draw_tile()` adds a tile to the rack; `add_tile()` puts a tile back into the bag.

#### 3. `int word_score(Game* game, const char* word, int row, int col, char direction)`

Assume the word is already valid and playable, and just calculate its score.

- `direction` is `'H'` (horizontal) or `'V'` (vertical); `(row, col)` is the starting square.
- **Newly placed** tiles use their square's modifier (`DOUBLE_LETTER`, `TRIPLE_LETTER`, `DOUBLE_WORD`, `TRIPLE_WORD`, `START` = double word).
- Tiles **already on the board** get no modifier (they were scored when they were played).
- Add **+50** (the *bingo* bonus) if the play uses all **7** rack tiles.
- Return the final score.

#### 4. `int play_valid_word(Game* game, Player* player, const char* word, int row, int col, char direction)`

Validate and, if legal, play the word. Return **1** on success, **0** if it cannot be played.

A play is valid only if **all** of these hold:

1. **Fits** — the whole word fits on the 15x15 board.
2. **Owns the tiles** — the player has each needed letter (a wildcard `*` may stand for any letter).
3. **Connects** — on an empty board the word must cover the centre (`H8`); otherwise at least one new tile must touch a tile already on the board.
4. **Maximal** — the word must not simply extend an existing word into a longer, unvalidated word.
5. **Valid word** — `is_valid(word)` returns true.
6. **Valid cross words** — every new word formed in the perpendicular direction must also be valid.

If valid:

- Place the letters on the board (a wildcard is shown as the **lowercase** letter it represents).
- Remove the used tiles from the player's rack.
- Add `word_score(...)` to the player's score.
- Return **1**.

### Part B (Bonus)

#### 1. `void anagram_finder(Player* player)`

Find and print every valid word that can be made using only the tiles on the rack (a wildcard can be any letter).

- Print them in order of **length (descending)**, then **alphabetically**.
- Wildcards are shown as the **lowercase** letter they represent.

#### 1. `char* highest_score(Game* game, Player* player)`

Find the highest-scoring legal next move for the player.

- Return the move as a string in the form `<WORD> <CELL> <DIR> (SCORE)`, e.g. `BE M2 V (8)`.
- If no move exists, return `"PASS"`.
- This is the hardest task — a full exhaustive search of every square is not the best approach. Hint: only empty squares **touching** existing tiles can be anchors for a new word.

## Compiling & Testing

Compile (from the `Task-2` folder) with your `Task2.c`:

```text
gcc main.c Task2.c given.c lib/dict.c -o main "-Wl,--stack,2097152"
```

(To run the reference solution instead, swap `Task2.c` for `Task2sol.c`.)

Run the automated test suite to check your work:

```text
powershell -ExecutionPolicy Bypass -File run_tests.ps1
```

You can also load a prepared board with the `LOAD` menu option (see `testcases/` and `example_state.txt`) to debug specific positions. The game menu options map to the functions as follows:

| Menu option | Function used |
| ------------- | --------------- |
| `PLAY` | `play_valid_word` (+ `word_score`) |
| `EXCH` | `exchange_tiles` |
| `HIGH` | `highest_score` |
| `ANAG` | `anagram_finder` |
| `LOAD` | `import_state` (given) |
