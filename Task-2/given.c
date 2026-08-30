//All the given functions for Task 2
//DO NOT MODIFY
#include "Task2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Given function, do not modify
void init_bag(Bag* bag){
    snprintf(bag->tiles, sizeof(bag->tiles), "AAAAAAAAABBCCDDDDEEEEEEEEEEEEFFGGGHHIIIIIIIIIJKLLLLMMNNNNNNOOOOOOOOPPQRRRRRRSSSSTTTTTTUUUUVVWWXYYZ**");
    bag->tiles_left = 100;
}

//Given function, do not modify
void init_game(Game* game){
    game->turn=1;
    game->P1.score=0;
    game->P2.score=0;
    init_bag(&game->bag);
    for(int i=0;i<8;i++){
        game->P1.rack[i]='\0';
        game->P2.rack[i]='\0';
    }
    static const MOD layout[15][15] = {
        {TRIPLE_WORD, BLANK,        BLANK,        DOUBLE_LETTER, BLANK,        BLANK,        BLANK,        TRIPLE_WORD,  BLANK,        BLANK,        BLANK,        DOUBLE_LETTER, BLANK,        BLANK,        TRIPLE_WORD},
        {BLANK,       DOUBLE_WORD,  BLANK,        BLANK,         BLANK,        TRIPLE_LETTER,BLANK,        BLANK,        BLANK,        TRIPLE_LETTER,BLANK,        BLANK,         BLANK,        DOUBLE_WORD,  BLANK},
        {BLANK,       BLANK,        DOUBLE_WORD,  BLANK,         BLANK,        BLANK,        DOUBLE_LETTER,BLANK,        DOUBLE_LETTER,BLANK,        BLANK,        BLANK,         DOUBLE_WORD,  BLANK,        BLANK},
        {DOUBLE_LETTER,BLANK,       BLANK,        DOUBLE_WORD,   BLANK,        BLANK,        BLANK,        DOUBLE_LETTER,BLANK,        BLANK,        BLANK,        DOUBLE_WORD,   BLANK,        BLANK,        DOUBLE_LETTER},
        {BLANK,       BLANK,        BLANK,        BLANK,         DOUBLE_WORD,  BLANK,        BLANK,        BLANK,        BLANK,        BLANK,        DOUBLE_WORD,  BLANK,         BLANK,        BLANK,        BLANK},
        {BLANK,       TRIPLE_LETTER,BLANK,        BLANK,         BLANK,        TRIPLE_LETTER,BLANK,        BLANK,        BLANK,        TRIPLE_LETTER,BLANK,        BLANK,         BLANK,        TRIPLE_LETTER,BLANK},
        {BLANK,       BLANK,        DOUBLE_LETTER,BLANK,         BLANK,        BLANK,        DOUBLE_LETTER,BLANK,        DOUBLE_LETTER,BLANK,        BLANK,        BLANK,         DOUBLE_LETTER,BLANK,        BLANK},
        {TRIPLE_WORD, BLANK,        BLANK,        DOUBLE_LETTER, BLANK,        BLANK,        BLANK,        START,        BLANK,        BLANK,        BLANK,        DOUBLE_LETTER, BLANK,        BLANK,        TRIPLE_WORD},
        {BLANK,       BLANK,        DOUBLE_LETTER,BLANK,         BLANK,        BLANK,        DOUBLE_LETTER,BLANK,        DOUBLE_LETTER,BLANK,        BLANK,        BLANK,         DOUBLE_LETTER,BLANK,        BLANK},
        {BLANK,       TRIPLE_LETTER,BLANK,        BLANK,         BLANK,        TRIPLE_LETTER,BLANK,        BLANK,        BLANK,        TRIPLE_LETTER,BLANK,        BLANK,         BLANK,        TRIPLE_LETTER,BLANK},
        {BLANK,       BLANK,        BLANK,        BLANK,         DOUBLE_WORD,  BLANK,        BLANK,        BLANK,        BLANK,        BLANK,        DOUBLE_WORD,  BLANK,         BLANK,        BLANK,        BLANK},
        {DOUBLE_LETTER,BLANK,       BLANK,        DOUBLE_WORD,   BLANK,        BLANK,        BLANK,        DOUBLE_LETTER,BLANK,        BLANK,        BLANK,        DOUBLE_WORD,   BLANK,        BLANK,        DOUBLE_LETTER},
        {BLANK,       BLANK,        DOUBLE_WORD,  BLANK,         BLANK,        BLANK,        DOUBLE_LETTER,BLANK,        DOUBLE_LETTER,BLANK,        BLANK,        BLANK,         DOUBLE_WORD,  BLANK,        BLANK},
        {BLANK,       DOUBLE_WORD,  BLANK,        BLANK,         BLANK,        TRIPLE_LETTER,BLANK,        BLANK,        BLANK,        TRIPLE_LETTER,BLANK,        BLANK,         BLANK,        DOUBLE_WORD,  BLANK},
        {TRIPLE_WORD, BLANK,        BLANK,        DOUBLE_LETTER, BLANK,        BLANK,        BLANK,        TRIPLE_WORD,  BLANK,        BLANK,        BLANK,        DOUBLE_LETTER, BLANK,        BLANK,        TRIPLE_WORD}
    };
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            game->board.board[i][j].letter='\0';
            game->board.board[i][j].mod=layout[i][j];
        }
    }
    for(int i=0;i<7;i++){
        draw_tile(&game->bag, &game->P1);
    }
    for(int i=0;i<7;i++){
        draw_tile(&game->bag, &game->P2);
    }
}

//Given function, do not modify
//Returns a random tile from the bag and removes it from the bag.
void draw_tile(Bag* bag, Player* player){
    if(player->rack[6]!='\0'||bag->tiles_left==0){
        return;
    }
    int index=rand()%bag->tiles_left;
    char tile=bag->tiles[index];
    for(;bag->tiles[index]!='\0';index++){
        bag->tiles[index] = bag->tiles[index+1];
    }
    bag->tiles_left--;
    for(int i=0;i<7;i++){
        if(player->rack[i]=='\0'){
            player->rack[i]=tile;
            break;
        }
    }
}

//Given function, do not modify
//Put tiles back in the bag during the exchange manoeuvre
void add_tile(Bag* bag, char tile){
    if(bag->tiles_left >= 100){ //Assuming a maximum of 100 tiles in the bag
        return;
    }
    bag->tiles[bag->tiles_left] = tile;
    for(int i=0;i<=bag->tiles_left;i++){
        for(int j=i+1;j<=bag->tiles_left;j++){
            //Wildcards '*' sort after every letter (i.e. to the back of the bag)
            int after;
            if(bag->tiles[i]=='*' && bag->tiles[j]!='*'){
                after = 1;
            }
            else if(bag->tiles[j]=='*'){
                after = 0;
            }
            else{
                after = bag->tiles[i] > bag->tiles[j];
            }
            if(after){
                char temp = bag->tiles[i];
                bag->tiles[i] = bag->tiles[j];
                bag->tiles[j] = temp;
            }
        }
    }
    bag->tiles_left++;
}

//Given function, do not modify
//Returns the current number of tiles on a player's rack
int rack_count(Player* player){
    int n = 0;
    while(n<7 && player->rack[n]!='\0'){
        n++;
    }
    return n;
}

//Given function, do not modify
//Returns number of tiles in bag
int tiles_left(Bag* bag){
    return bag->tiles_left; 
}

//Given function, do not modify
//Prints out all the tiles left in the bag and the number of tiles left
void tiles_info(Bag* bag){
    printf("Tiles in bag: %s (%d)\n", bag->tiles, tiles_left(bag));
}

//Given function, do not modify
//Prints out all the unseen tiles for this player
//(the tiles left in the bag + the opponent's rack), sorted alphabetically
//with wildcards '*' at the end, so the opponent's tiles cannot be identified.
void unseen_tiles_info(Game* game, Player* player){
    char unseen[108];  // up to 100 bag tiles + 7 opponent tiles + null
    int count = 0;

    // Everything left in the bag
    for(int i=0; game->bag.tiles[i]!='\0'; i++){
        unseen[count++] = game->bag.tiles[i];
    }

    // The opponent's rack
    Player* opponent = (player == &game->P1) ? &game->P2 : &game->P1;
    for(int i=0;i<7;i++){
        if(opponent->rack[i]!='\0'){
            unseen[count++] = opponent->rack[i];
        }
    }
    unseen[count] = '\0';

    // Sort alphabetically, wildcards '*' at the back
    for(int i=0;i<count;i++){
        for(int j=i+1;j<count;j++){
            int after;
            if(unseen[i]=='*' && unseen[j]!='*'){
                after = 1;
            }
            else if(unseen[j]=='*'){
                after = 0;
            }
            else{
                after = unseen[i] > unseen[j];
            }
            if(after){
                char temp = unseen[i];
                unseen[i] = unseen[j];
                unseen[j] = temp;
            }
        }
    }

    printf("Unseen tiles for player: %s (%d)\n", unseen, count);
}

//Given Function, do not modify
//Returns the value of a given letter. If the letter is not valid, returns -1
int value(char tile) {
    static const int values[26] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10};
    if(tile=='*'){
        return 0;
    }
    if(tile>='A'&&tile<='Z'){
        return values[tile-'A'];
    }
    return -1;
}

//Helper Function, do not modify
//Prints a horizontal separator line for the board grid
void print_row_separator(void){
    printf("+---+");
    for(int j=1;j<15;j++){
        printf("---+");
    }
    printf("\n");
}

//Given Function, do not modify
//Prints the board to the terminal
void print_board(Game game){
    printf("\n------------------------ Game Board -------------------------\n");

    //Column letters header (A..O), aligned above each cell
    char header[62];
    for(int i=0;i<61;i++){
        header[i] = ' ';
    }
    header[61] = '\0';
    for(int j=0;j<15;j++){
        header[4*j+2] = 'A'+j;
    }
    printf("   %s\n", header);

    printf("   ");
    print_row_separator();

    for(int i=0;i<15;i++){
        printf("%2d ", i+1);
        printf("|");
        for(int j=0;j<15;j++){
            Tile tile = game.board.board[i][j];
            char c;
            if(tile.letter!='\0'){
                c = tile.letter;
            }
            else{
                switch(tile.mod){
                    case DOUBLE_LETTER: c='+'; break;
                    case TRIPLE_LETTER: c='#'; break;
                    case DOUBLE_WORD:   c='$'; break;
                    case TRIPLE_WORD:   c='%'; break;
                    case START:         c='@'; break;
                    default:            c=' '; break;
                }
            }
            printf(" %c |", c);
        }
        printf("\n");
        printf("   ");
        print_row_separator();
    }
}

//Given Function, do not modify
//Prints the current turn and player information to the terminal
void print_turn(Game game){
    print_board(game);
    printf("------------------------ Player Info ------------------------\n");
    printf("Player 1: %d points                   ", game.P1.score);
    printf("Player 2: %d points\n", game.P2.score);
    printf("Current turn: Player %d\n", game.turn);
}

//Given Function, do not modify
//Prints the current player's rack to the terminal
void print_rack(Player player){
    printf("Current rack: ");
    for(int i=0;i<7;i++){
        if(player.rack[i]!='\0'){
            printf("%c", player.rack[i]);
        }
    }
    printf("\n");
}

//Given Function, do not modify
//Imports a saved game state from a text file so you can debug a specific
//board position.
//
//File format:
//  - 15 lines for the board (one per row, top to bottom), each with exactly
//    15 cells.  '.' (or ' ' or '_') = empty, A-Z = a placed tile, and a-z =
//    a wildcard shown as the lowercase letter it represents.
//  - After the board, optional lines:
//        RACK1 <tiles>    P1's rack       RACK2 <tiles>    P2's rack
//        SCORE1 <n>       P1's score      SCORE2 <n>       P2's score
//        TURN <1|2>       whose turn      BAG <tiles>      bag tiles
//  - Lines starting with '#' are comments and are ignored.
void import_state(Game* game, const char* filename){
    FILE* f = fopen(filename, "r");
    if(!f){
        printf("Could not open '%s'.\n", filename);
        return;
    }

    // Start from a fresh game: correct premium-square layout, empty racks,
    // scores 0, and the full bag. The imported lines then override this.
    init_game(game);
    game->P1.score = 0;
    game->P2.score = 0;
    for(int i=0;i<8;i++){
        game->P1.rack[i]='\0';
        game->P2.rack[i]='\0';
    }
    init_bag(&game->bag);

    char line[200];
    int row = 0;
    while(fgets(line, sizeof(line), f)){
        // Trim trailing newline / carriage return
        int len = (int)strlen(line);
        while(len>0 && (line[len-1]=='\n' || line[len-1]=='\r')){
            line[--len]='\0';
        }

        if(len==0 || line[0]=='#'){
            continue;
        }

        if(row < 15){
            // A board row: each of the (up to) 15 characters is one cell.
            // Letters A-Z / a-z are placed as tiles; '.' (or ' ' or '_')
            // marks an empty cell.
            int col = 0;
            for(int i=0; i<len && col<15; i++){
                char ch = line[i];
                if((ch>='A' && ch<='Z') || (ch>='a' && ch<='z')){
                    game->board.board[row][col].letter = ch;
                }
                col++;  // every character position is one board cell
            }
            row++;
        }
        else if(strncmp(line, "RACK1", 5)==0){
            const char* t = line+5;
            while(*t==' ') t++;
            int i=0;
            while(*t && *t!=' ' && i<7){ game->P1.rack[i++]=*t; t++; }
            game->P1.rack[i]='\0';
        }
        else if(strncmp(line, "RACK2", 5)==0){
            const char* t = line+5;
            while(*t==' ') t++;
            int i=0;
            while(*t && *t!=' ' && i<7){ game->P2.rack[i++]=*t; t++; }
            game->P2.rack[i]='\0';
        }
        else if(strncmp(line, "SCORE1", 6)==0){
            game->P1.score = atoi(line+6);
        }
        else if(strncmp(line, "SCORE2", 6)==0){
            game->P2.score = atoi(line+6);
        }
        else if(strncmp(line, "TURN", 4)==0){
            int t = atoi(line+4);
            game->turn = (t==1 || t==2) ? t : 1;
        }
        else if(strncmp(line, "BAG", 3)==0){
            const char* t = line+3;
            while(*t==' ') t++;
            int i=0;
            while(*t && *t!=' ' && i<100){ game->bag.tiles[i++]=*t; t++; }
            game->bag.tiles[i]='\0';
            game->bag.tiles_left = i;
        }
    }
    fclose(f);

    sort_rack(&game->P1);
    sort_rack(&game->P2);
    printf("Imported '%s': P1=%d, P2=%d, turn %d.\n",
           filename, game->P1.score, game->P2.score, game->turn);
}

//Given function, do not modify
//Returns 1 if the board has no tiles on it (every square empty), else 0
int is_board_empty(Board* board){
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            if(board->board[i][j].letter!='\0'){
                return 0;
            }
        }
    }
    return 1;
}
