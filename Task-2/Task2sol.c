//You are NOT ALLOWED to add any other libraries
//
#include "Task2.h"

#include <stdio.h>
#include <stdlib.h>
#include "lib/dict.h"
//

//You are allowed to define helpers in this file to assist you in implementing the required functions. However, you are NOT allowed to modify the function signatures of the provided functions in Task2.h.

/*
Task: Implement a simple insertion sort to sort the player's rack in alphabetical order. This function should modify the player's rack in place. The wildcard * should come after Z in the rack.
Insertion sort logic:
1. Start at the 2nd element
2. Compare the element directly before it, and swap the 2 elements of the element directly before is larger
3. Repeat step 2 until the element directly before it is smaller, or the element reached the start of the list
4. Go to the next element and repeat the process. (Proceed to the 3rd element, repeat steps 2 and 3, then go to the 4th element, repeat steps 2 and 3, and so on.)
*/
//Helper: returns 1 if tile a should sort after tile b (wildcard '*' sorts last)
static int tile_after(char a, char b){
    if(a=='*' && b!='*'){
        return 1;
    }
    if(b=='*'){
        return 0;
    }
    return a > b;
}

void sort_rack(Player* player){
    int len = 0;
    while(player->rack[len]!='\0'){
        len++;
    }
    for(int i=1;i<len;i++){
        int j = i;
        while(j>0 && tile_after(player->rack[j-1], player->rack[j])){
            char tmp = player->rack[j-1];
            player->rack[j-1] = player->rack[j];
            player->rack[j] = tmp;
            j--;
        }
    }
}

//Tiny string helpers (avoids needing <string.h>, which is not allowed).
static int my_strlen(const char* s){
    int n = 0;
    while(s[n] != '\0'){
        n++;
    }
    return n;
}
static void my_strcpy(char* dst, const char* src){
    while((*dst = *src) != '\0'){
        dst++;
        src++;
    }
}

//Case-insensitive string comparison (used for dedup and alphabetical sorting).
static int my_strcasecmp(const char* a, const char* b){
    while(*a && (*a == *b || (*a | 32) == (*b | 32))){
        a++;
        b++;
    }
    return (*a | 32) - (*b | 32);
}

//Counts how many letters in s are lowercase; each lowercase letter is one
//wildcard represented by that letter.
static int num_wildcards(const char* s){
    int n = 0;
    for(int i = 0; s[i] != '\0'; i++){
        if(s[i] >= 'a' && s[i] <= 'z'){
            n++;
        }
    }
    return n;
}

//Helper: removes exactly one occurrence of target from a null-terminated tile
//string (no wildcard fallback). Returns 1 if a tile was removed, else 0.
static int remove_exact(char* rack, char target){
    int len = 0;
    while(rack[len]!='\0'){
        len++;
    }
    for(int i=0;i<len;i++){
        if(rack[i]==target){
            for(int j=i;j<len;j++){
                rack[j] = rack[j+1];
            }
            return 1;
        }
    }
    return 0;
}

/*
Task: Implement the logic to exchange tiles in the player's rack.
This function should remove the specified tiles from the player's rack and replace them with new tiles from the bag.
*/
int exchange_tiles(Game* game, Player* player, const char* tiles){
    // Reject the exchange if any requested tile is not actually in the rack
    char rack_copy[8];
    for(int i=0;i<8;i++){
        rack_copy[i] = player->rack[i];
    }
    int count = 0;
    for(int i=0; tiles[i]!='\0'; i++){
        if(!remove_exact(rack_copy, tiles[i])){
            return 0; // invalid exchange: a requested tile is not in the rack
        }
        count++;
    }

    // Valid exchange: remove the tiles from the rack, put them back in the
    // bag with add_tile, then draw the same number of replacement tiles
    for(int i=0; i<count; i++){
        remove_exact(player->rack, tiles[i]);
        add_tile(&game->bag, tiles[i]);
    }
    for(int i=0; i<count && tiles_left(&game->bag)>0; i++){
        draw_tile(&game->bag, player);
    }
    return 1;
}

/*
Task: Implement the logic when a certain word is played on the board.
Assume the word is valid, and calculate the score of the word after applying the modifiers, and the bingo score bonus.
After the calculation, return the score of the word.
*/
int word_score(Game* game, const char* word, int row, int col, char direction){
    int dr = (direction=='V') ? 1 : 0;
    int dc = (direction=='H') ? 1 : 0;
    int letter_score = 0;
    int word_mult = 1;
    int new_tiles = 0;
    for(int k=0; word[k]!='\0'; k++){
        int r = row + dr*k;
        int c = col + dc*k;
        if(game->board.board[r][c].letter!='\0'){
            // Tile already on the board: no letter/word multiplier applies
            letter_score += value(word[k]);
        }
        else{
            // Newly placed tile: apply the square's multipliers
            new_tiles++;
            int letter_mult = 1;
            switch(game->board.board[r][c].mod){
                case DOUBLE_LETTER: letter_mult = 2; break;
                case TRIPLE_LETTER: letter_mult = 3; break;
                case DOUBLE_WORD:
                case START:         word_mult *= 2; break; // centre star = double word
                case TRIPLE_WORD:   word_mult *= 3; break;
                default:            break;
            }
            letter_score += value(word[k]) * letter_mult;
        }
    }
    int score = letter_score * word_mult;
    if(new_tiles==7){
        score += 50; // bingo bonus for using all 7 tiles
    }
    return score;
}

/*Task: Implement the logic to play a valid word on the board.
This function should update the board, the player's rack, and the player's score accordingly.
The direction parameter will be either 'H' for horizontal or 'V' for vertical.

Logic: 
First, check of the word is playable in that location/whether the player can actually play that word.
Then, check whether the word itself is valid.
Check if word is valid -> if invalid, return.
Check if word is valid -> if valid, edit word on board -> update player's rack -> calculate word score -> update player's score 
*/
//Helper: removes one occurrence of target (falling back to a wildcard '*')
//from a null-terminated tile string. Returns 1 if a tile was removed, else 0.
static int take_tile(char* rack, char target, int* used_wild){
    int len = 0;
    while(rack[len]!='\0'){
        len++;
    }
    for(int pass=0; pass<2; pass++){
        char want = (pass==0) ? target : '*';
        for(int i=0;i<len;i++){
            if(rack[i]==want){
                for(int j=i;j<len;j++){
                    rack[j] = rack[j+1];
                }
                if(used_wild){
                    *used_wild = (pass == 1);  // a wildcard '*' was consumed
                }
                return 1;
            }
        }
    }
    return 0;
}

//Helper: checks the cross word formed in the perpendicular direction when a
//new letter is placed at (r,c). Returns 1 if it is valid (or a single letter,
//which forms no cross word), 0 if an invalid cross word would be created.
static int check_cross(Game* game, int r, int c, char letter, char direction){
    // The cross word runs perpendicular to the play direction
    int scan_r = (direction=='H') ? 1 : 0;  // H play -> vertical cross (scan rows)
    int scan_c = (direction=='H') ? 0 : 1;  // V play -> horizontal cross (scan cols)

    int lo, hi;
    if(scan_r){
        // Vertical cross word: scan up/down at column c
        lo = r;
        while(lo-1>=0 && game->board.board[lo-1][c].letter!='\0'){
            lo--;
        }
        hi = r;
        while(hi+1<15 && game->board.board[hi+1][c].letter!='\0'){
            hi++;
        }
    }
    else{
        // Horizontal cross word: scan left/right at row r
        lo = c;
        while(lo-1>=0 && game->board.board[r][lo-1].letter!='\0'){
            lo--;
        }
        hi = c;
        while(hi+1<15 && game->board.board[r][hi+1].letter!='\0'){
            hi++;
        }
    }

    if(hi - lo + 1 < 2){
        return 1;  // single letter, no cross word to check
    }

    char cross[16];
    int idx = 0;
    for(int i=lo; i<=hi; i++){
        if(scan_r){
            if(i==r){
                cross[idx++] = letter;
            }
            else{
                cross[idx++] = game->board.board[i][c].letter;
            }
        }
        else{
            if(i==c){
                cross[idx++] = letter;
            }
            else{
                cross[idx++] = game->board.board[r][i].letter;
            }
        }
    }
    cross[idx] = '\0';
    return is_valid(cross);
}

int play_valid_word(Game* game, Player* player, const char* word, int row, int col, char direction){
    int dr = (direction=='V') ? 1 : 0;
    int dc = (direction=='H') ? 1 : 0;
    int len = 0;
    while(word[len]!='\0'){
        len++;
    }

    // Check the word fits inside the 15x15 board
    if(row<0 || row>14 || col<0 || col>14){
        return 0;
    }
    int end_r = row + dr*(len-1);
    int end_c = col + dc*(len-1);
    if(end_r<0 || end_r>14 || end_c<0 || end_c>14){
        return 0;
    }

    // The word must be maximal in the play direction: the cells immediately
    // before its start and after its end must be empty. Otherwise the play
    // would merge with an existing tile into a longer, unvalidated word.
    int br = row - dr;
    int bc = col - dc;
    if(br >= 0 && br < 15 && bc >= 0 && bc < 15 &&
       game->board.board[br][bc].letter != '\0'){
        return 0;
    }
    int er = row + dr*len;
    int ec = col + dc*len;
    if(er >= 0 && er < 15 && ec >= 0 && ec < 15 &&
       game->board.board[er][ec].letter != '\0'){
        return 0;
    }

    // Is this the first play (board completely empty)?
    int board_empty = is_board_empty(&game->board);

    // Simulate removing tiles from a copy of the rack to check the player
    // actually has the needed tiles, and check the word connects.
    char rack_copy[8];
    for(int i=0;i<8;i++){
        rack_copy[i] = player->rack[i];
    }
    int connected = 0;
    int covers_start = 0;

    for(int k=0;k<len;k++){
        int r = row + dr*k;
        int c = col + dc*k;
        if(game->board.board[r][c].letter!='\0'){
            // Wildcards are stored lowercase on the board, so compare case-insensitively
            char existing = game->board.board[r][c].letter;
            if(existing >= 'a' && existing <= 'z'){
                existing -= 32;
            }
            if(existing != word[k]){
                return 0; // conflicts with a tile already on the board
            }
        }
        else{
            if(!take_tile(rack_copy, word[k], NULL)){
                return 0; // player does not have the needed tile
            }
            if(!board_empty){
                if((r>0  && game->board.board[r-1][c].letter!='\0') ||
                   (r<14 && game->board.board[r+1][c].letter!='\0') ||
                   (c>0  && game->board.board[r][c-1].letter!='\0') ||
                   (c<14 && game->board.board[r][c+1].letter!='\0')){
                    connected = 1;
                }
            }
            if(r==7 && c==7){
                covers_start = 1;
            }
            // Every newly placed tile must also form valid cross (perpendicular)
            // words with the existing tiles
            if(!check_cross(game, r, c, word[k], direction)){
                return 0; // an overlapping cross word is not valid
            }
        }
    }

    // Placement rule: the first play must pass through the centre; any later
    // play must connect to at least one existing tile
    if(board_empty){
        if(!covers_start) return 0;
    }
    else{
        if(!connected) return 0;
    }

    // Check the word itself is valid
    if(!is_valid(word)){
        return 0;
    }

    // Calculate the score BEFORE placing the tiles, so that word_score can
    // see which squares are newly covered (premium squares only apply to new
    // tiles) and which already contain tiles from previous plays.
    int score = word_score(game, word, row, col, direction);

    // Play the word: place the letters on the board and remove the used
    // tiles from the player's rack
    for(int k=0;k<len;k++){
        int r = row + dr*k;
        int c = col + dc*k;
        if(game->board.board[r][c].letter=='\0'){
            int used_wild = 0;
            take_tile(player->rack, word[k], &used_wild);
            // A wildcard is displayed as the lowercase letter it represents
            game->board.board[r][c].letter = used_wild ? (char)(word[k] + 32) : word[k];
        }
    }
    player->score += score;
    return 1;
}


//PART B (BONUS)

/*
Task: Implement a function to find all words a player can play using only the tiles on their rack.
After finding them out, print them out in the terminal. (In order of length in descending order, then alphabetically)
(Reminder: Wildcards are displayed as the letter they represent, but in lowercase.)
*/
#define ANAGRAM_MAX 8192
//Helper: expands any wildcards in a permutation and stores every word that is
//valid in the dictionary into the word list (duplicates removed).
static void anagram_expand(const char* perm, int len, int idx, char* word,
                           char words[][16], int* count){
    if(idx == len){
        word[idx] = '\0';
        if(is_valid(word)){
            // Skip if we already stored this word. If the same word can be made
            // with and without a wildcard, keep the version using the fewest
            // wildcards (fewest lowercase letters), e.g. prefer "AB" over "Ab".
            for(int i = 0; i < *count; i++){
                if(my_strcasecmp(words[i], word) == 0){
                    if(num_wildcards(word) < num_wildcards(words[i])){
                        my_strcpy(words[i], word);
                    }
                    return;
                }
            }
            if(*count < ANAGRAM_MAX){
                my_strcpy(words[*count], word);
                (*count)++;
            }
        }
        return;
    }
    if(perm[idx] == '*'){
        for(char L = 'A'; L <= 'Z'; L++){
            word[idx] = L + 32;  // wildcard: shown as the letter it represents, lowercase
            anagram_expand(perm, len, idx + 1, word, words, count);
        }
    }
    else{
        word[idx] = perm[idx];
        anagram_expand(perm, len, idx + 1, word, words, count);
    }
}

//Helper: generates every distinct permutation of the rack tiles of length 2..rlen.
static void anagram_gen(const char* letters, int rlen, char* cur, int pos,
                        int used[], char* word, char words[][16], int* count){
    if(pos >= 2){
        anagram_expand(cur, pos, 0, word, words, count);
    }
    if(pos == rlen){
        return;
    }
    char last = '\0';
    for(int i = 0; i < rlen; i++){
        if(used[i]){
            continue;
        }
        if(letters[i] == last){
            continue;  // skip duplicate tiles (identical letters are adjacent)
        }
        last = letters[i];
        used[i] = 1;
        cur[pos] = letters[i];
        anagram_gen(letters, rlen, cur, pos + 1, used, word, words, count);
        used[i] = 0;
    }
}

//Comparison for qsort: longer words first, then alphabetically.
static int cmp_anagram(const void* a, const void* b){
    const char* wa = (const char*)a;
    const char* wb = (const char*)b;
    int la = my_strlen(wa);
    int lb = my_strlen(wb);
    if(la != lb){
        return lb - la;  // length descending
    }
    return my_strcasecmp(wa, wb);  // then alphabetical (case-insensitive)
}

void anagram_finder(Player* player) {
    // Collect the rack letters (up to 7); the rack is a null-terminated string
    char letters[8];
    int rlen = 0;
    while(rlen < 7 && player->rack[rlen] != '\0'){
        letters[rlen] = player->rack[rlen];
        rlen++;
    }
    letters[rlen] = '\0';

    if(rlen < 2){
        printf("Words from rack: (none)\n");
        return;
    }

    // Sort the letters so duplicates are adjacent (needed for the dedup)
    for(int i = 0; i < rlen; i++){
        for(int j = i + 1; j < rlen; j++){
            if(letters[i] > letters[j]){
                char t = letters[i];
                letters[i] = letters[j];
                letters[j] = t;
            }
        }
    }

    // Collect every valid word (deduplicated) into a heap-allocated list
    char (*words)[16] = malloc(sizeof(char[16]) * ANAGRAM_MAX);
    if(words == NULL){
        printf("Words from rack: (none)\n");
        return;
    }
    int count = 0;
    char cur[8];
    char word[16];
    int used[8] = {0};
    anagram_gen(letters, rlen, cur, 0, used, word, words, &count);

    // Sort by length (descending), then alphabetically
    qsort(words, count, sizeof(char[16]), cmp_anagram);

    printf("Words from rack: ");
    for(int i = 0; i < count; i++){
        printf("%s ", words[i]);
    }
    if(count == 0){
        printf("(none)");
    }
    printf("\n");

    free(words);
}

/*
Task: Implement an algorithm to find the highest scoring next move, given the player's rack and the current board state.
And returns the highest scoring move as a PLAY command string <word> <cell> <direction> (score)
*/

//Helper: evaluates one candidate move (word starting at sr,sc, going in `dir`,
//1 = horizontal, 0 = vertical) by applying it to a copy of the game, and keeps
//it if it is the highest scoring move found so far.
static void try_move(Game* game, Player* player, const char* word,
                     int sr, int sc, int dir,
                     char* best, int* best_score){
    int is_p1 = (player == &game->P1);
    int base = is_p1 ? game->P1.score : game->P2.score;

    Game copy = *game;
    Player* cp = is_p1 ? &copy.P1 : &copy.P2;
    //printf("Trying word %s at (%d, %d) in direction %s\n", word, sr, sc, dir ? "H" : "V");
    if(!play_valid_word(&copy, cp, word, sr, sc, dir ? 'H' : 'V')){
        return;
    }

    int score = cp->score - base;
    if(score > *best_score){
        *best_score = score;

        // Build the move string: "<word> <col letter><row number> <dir> (score)"
        int row = sr + 1;
        char cell[4];
        cell[0] = 'A' + sc;
        if(row < 10){
            cell[1] = '0' + row;
            cell[2] = '\0';
        }
        else{
            cell[1] = '0' + (row / 10);
            cell[2] = '0' + (row % 10);
            cell[3] = '\0';
        }
        snprintf(best, 40, "%s %s %c (%d)", word, cell, dir ? 'H' : 'V', score);
    }
}

//Helper: recursively builds words along `dir` (1 = horizontal, 0 = vertical)
//starting at the current cell (r,c). Every generated word must include the
//anchor cell (ar,ac) -- an empty cell adjacent to existing tiles (or the centre
//on the first move) -- which guarantees the move connects to the board.
//Occupied cells use their fixed letter; empty cells are filled with rack tiles
//(a wildcard '*' can stand for any letter).
static void gen_word(Game* game, Player* player,
                     const char* letters, int rlen,
                     int ar, int ac, int dir,
                     int sr, int sc,
                     int r, int c,
                     char* word, int len, int used[], int passed,
                     char* best, int* best_score){
    // Reached the board edge: the word ends here
    if(r < 0 || r > 14 || c < 0 || c > 14){
        if(len >= 2 && passed){
            word[len] = '\0';
            try_move(game, player, word, sr, sc, dir, best, best_score);
        }
        return;
    }

    if(game->board.board[r][c].letter != '\0'){
        // Occupied cell: fixed letter, always part of the word.
        // Uppercase it (wildcards are stored lowercase on the board).
        int np = passed || (r == ar && c == ac);
        word[len] = game->board.board[r][c].letter;
        if(word[len] >= 'a' && word[len] <= 'z'){
            word[len] -= 32;
        }
        int nr = dir ? r     : r + 1;
        int nc = dir ? c + 1 : c;
        gen_word(game, player, letters, rlen, ar, ac, dir, sr, sc, nr, nc,
                 word, len + 1, used, np, best, best_score);
        return;
    }

    // Empty cell
    int np = passed || (r == ar && c == ac);

    // Option 1: end the word here (only allowed after the anchor is placed)
    if(passed && len >= 2){
        word[len] = '\0';
        try_move(game, player, word, sr, sc, dir, best, best_score);
    }

    // Option 2: place a rack tile here
    for(int i = 0; i < rlen; i++){
        if(used[i]){
            continue;
        }
        used[i] = 1;
        int nr = dir ? r     : r + 1;
        int nc = dir ? c + 1 : c;
        if(letters[i] == '*'){
            for(char L = 'A'; L <= 'Z'; L++){
                word[len] = L;
                gen_word(game, player, letters, rlen, ar, ac, dir, sr, sc, nr, nc,
                         word, len + 1, used, np, best, best_score);
            }
        }
        else{
            word[len] = letters[i];
            gen_word(game, player, letters, rlen, ar, ac, dir, sr, sc, nr, nc,
                     word, len + 1, used, np, best, best_score);
        }
        used[i] = 0;
    }
}

//Helper: generates every word (horizontal and vertical) that passes through the
//anchor cell (ar,ac) and connects to the board.
static void gen_anchor(Game* game, Player* player,
                       const char* letters, int rlen,
                       int ar, int ac,
                       char* best, int* best_score){
    // Horizontal words through (ar,ac): find the leftmost forced cell
    int p = ac;
    while(p - 1 >= 0 && game->board.board[ar][p - 1].letter != '\0'){
        p--;
    }
    for(int s = 0; s <= p; s++){
        char word[16];
        int used[8] = {0};
        gen_word(game, player, letters, rlen, ar, ac, 1, ar, s, ar, s,
                 word, 0, used, 0, best, best_score);
    }

    // Vertical words through (ar,ac)
    p = ar;
    while(p - 1 >= 0 && game->board.board[p - 1][ac].letter != '\0'){
        p--;
    }
    for(int s = 0; s <= p; s++){
        char word[16];
        int used[8] = {0};
        gen_word(game, player, letters, rlen, ar, ac, 0, s, ac, s, ac,
                 word, 0, used, 0, best, best_score);
    }
}

char* highest_score(Game* game, Player* player) {
    static char best[40] = "PASS";
    int best_score = -1;

    // Collect the rack letters (up to 7); the rack is a null-terminated string
    char letters[8];
    int rlen = 0;
    while(rlen < 7 && player->rack[rlen] != '\0'){
        letters[rlen] = player->rack[rlen];
        rlen++;
    }
    letters[rlen] = '\0';

    // A single tile can still be a valid play (a hook onto existing tiles,
    // e.g. adding E next to a D to form "DE"), so only bail out when the
    // rack is completely empty.
    if(rlen < 1){
        snprintf(best, sizeof(best), "PASS");
        return best;
    }

    // Is the board empty (first move)?
    int board_empty = is_board_empty(&game->board);

    if(board_empty){
        // First move: the word must cover the centre (7,7)
        gen_anchor(game, player, letters, rlen, 7, 7, best, &best_score);
    }
    else{
        // Otherwise: every empty cell adjacent to an existing tile is an anchor
        for(int r = 0; r < 15; r++){
            for(int c = 0; c < 15; c++){
                if(game->board.board[r][c].letter != '\0'){
                    continue;  // occupied cell, not an anchor
                }
                int adjacent = 0;
                if(r > 0   && game->board.board[r-1][c].letter != '\0') adjacent = 1;
                if(r < 14  && game->board.board[r+1][c].letter != '\0') adjacent = 1;
                if(c > 0   && game->board.board[r][c-1].letter != '\0') adjacent = 1;
                if(c < 14  && game->board.board[r][c+1].letter != '\0') adjacent = 1;
                if(adjacent){
                    gen_anchor(game, player, letters, rlen, r, c, best, &best_score);
                }
            }
        }
    }

    if(best_score < 0){
        snprintf(best, sizeof(best), "PASS");
    }
    return best;
}