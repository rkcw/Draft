#ifndef LIST_H
#define LIST_H

typedef enum{
    BLANK,
    DOUBLE_LETTER,
    TRIPLE_LETTER,
    DOUBLE_WORD,
    TRIPLE_WORD,
    START
} MOD;

typedef struct{
    char letter;
    MOD mod;
} Tile;

typedef struct{
    Tile board[15][15];
} Board;

typedef struct{
    char rack[8]; // 7 + 1
    int score;
} Player;

typedef struct{
    char tiles[101]; // 100 + 1
    int tiles_left;
} Bag;

typedef struct{
    Board board;
    Bag bag;
    Player P1;
    Player P2;
    int turn; // 1 for P1, 2 for P2
} Game;

//Given functions are declared in given.c
void init_bag(Bag* bag); //Given function
void init_game(Game* game); //Given function
void draw_tile(Bag* bag, Player* player); //Given function
void add_tile(Bag* bag, char tile); //Given function
void tiles_info(Bag* bag); //Given function
int tiles_left(Bag* bag); //Given function
int value(char letter); //Given function
void print_board(Game game); //Given function
void print_turn(Game game); //Given function
void print_rack(Player player); //Given function
void unseen_tiles_info(Game* game, Player* player); //Given function
int rack_count(Player* player); //Given function
void import_state(Game* game, const char* filename); //Given function
int is_board_empty(Board* board); //Given function

//Tasks
void sort_rack(Player* player);
int exchange_tiles(Game* game, Player* player, const char* tiles);
int word_score(Game* game, const char* word, int row, int col, char direction);
int play_valid_word(Game* game, Player* player, const char* word, int row, int col, char direction);

//PART B
void anagram_finder(Player* player);
char* highest_score(Game* game, Player* player);

#endif