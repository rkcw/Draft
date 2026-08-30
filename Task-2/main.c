/*
 * HOW TO COMPILE (Windows / MinGW gcc, run from the Task-2 folder):
 *   gcc main.c Task2sol.c given.c lib/dict.c -o main "-Wl,--stack,2097152"
 *
 * Uses Task2sol.c (the completed solution) instead of Task2.c.
 * given.c holds the shared "given" functions.
 */
#include "Task2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(){
    srand(2);
    Game game;
    init_game(&game);

    int passes = 0;     // consecutive passes; 2 in a row ends the game
    int game_over = 0;
    int skip_display = 0;  // skip re-printing the board after a HIGH hint

    while(!game_over){
        Player* player = (game.turn==1) ? &game.P1 : &game.P2;

        if(!skip_display){
            print_turn(game);
            // tiles_info(&game.bag);
            
            unseen_tiles_info(&game, player);
            sort_rack(player);
            print_rack(*player);
        }
        skip_display = 0;

        // --- Menu: choose an action ---
        printf("Select an option:\n");
        printf("  PLAY  - play a word\n");
        printf("  EXCH  - exchange tiles\n");
        printf("  PASS  - skip your turn\n");
        printf("  QUIT  - end the game\n");
        printf("  HIGH  - show the highest scoring move\n");
        printf("  ANAG  - list all words you can make from your rack\n");
        printf("  LOAD  - import a game state from a file\n");
        printf("> ");

        char line[100];
        if(!fgets(line, sizeof(line), stdin)){
            break;  // end of input
        }

        char option[16];
        if(sscanf(line, "%15s", option)!=1){
            continue;  // empty line, ask again
        }
        // Uppercase the option so it works in any case
        for(int i=0; option[i]!='\0'; i++){
            if(option[i]>='a' && option[i]<='z'){
                option[i] -= 32;
            }
        }

        if(strcmp(option, "PLAY")==0){
            // Further ask for the move details
            printf("Enter move (WORD CELL DIR, e.g. JET H8 H): ");
            if(!fgets(line, sizeof(line), stdin)){
                break;
            }

            char word[16];
            char cell[4];
            char dir;
            if(sscanf(line, "%15s %3s %c", word, cell, &dir)==3){
                // Normalise the word, cell letter and direction to uppercase
                for(int i=0; word[i]!='\0'; i++){
                    if(word[i]>='a' && word[i]<='z'){
                        word[i] -= 32;
                    }
                }
                if(cell[0]>='a' && cell[0]<='z'){
                    cell[0] -= 32;
                }
                if(dir>='a' && dir<='z'){
                    dir -= 32;
                }

                // Strict cell check: first char A-O, the rest must be digits
                int cell_ok = (cell[0]>='A' && cell[0]<='O');
                for(int i=1; cell_ok && cell[i]!='\0'; i++){
                    if(cell[i]<'0' || cell[i]>'9'){
                        cell_ok = 0;
                    }
                }

                // Parse the algebraic cell: column letter A-O + row number 1-15
                int col = cell[0] - 'A';
                int row = atoi(cell+1) - 1;

                if(!cell_ok || row<0 || row>14){
                    printf("Invalid cell: use a column letter A-O and row 1-15 (e.g. H8).\n");
                }
                else if(play_valid_word(&game, player, word, row, col, dir)){
                    // The word was played: refill the rack, then check game over
                    passes = 0;
                    while(tiles_left(&game.bag)>0 && rack_count(player)<7){
                        draw_tile(&game.bag, player);
                    }
                    sort_rack(player);
                    if(tiles_left(&game.bag)==0 && rack_count(player)==0){
                        game_over = 1;  // bag empty and this player used their last tiles
                    }
                    else{
                        game.turn = (game.turn==1) ? 2 : 1;
                    }
                }
                else{
                    printf("Invalid move: not a valid word or cannot be played there.\n");
                }
            }
            else{
                printf("Invalid move format. Use WORD CELL DIR, e.g. JET H8 H.\n");
            }
        }
        else if(strcmp(option, "EXCH")==0){
            // Further ask for the tiles to exchange
            printf("Enter tiles to exchange (e.g. AE): ");
            if(!fgets(line, sizeof(line), stdin)){
                break;
            }

            char tiles[16];
            if(sscanf(line, "%15s", tiles)==1){
                // Normalise the tiles to uppercase
                for(int i=0; tiles[i]!='\0'; i++){
                    if(tiles[i]>='a' && tiles[i]<='z'){
                        tiles[i] -= 32;
                    }
                }
                if(tiles_left(&game.bag) < 7){
                    printf("Cannot exchange: fewer than 7 tiles left in the bag.\n");
                }
                else if(exchange_tiles(&game, player, tiles)){
                    passes = 0;
                    game.turn = (game.turn==1) ? 2 : 1;
                }
                else{
                    printf("Invalid exchange: one of those tiles is not in your rack.\n");
                }
            }
            else{
                printf("Invalid exchange. Enter the tiles to exchange, e.g. AE.\n");
            }
        }
        else if(strcmp(option, "HIGH")==0){
            // Show the best move, then go straight back to the menu
            printf("Suggested move: %s\n", highest_score(&game, player));
            skip_display = 1;
        }
        else if(strcmp(option, "ANAG")==0){
            // List every valid word from the current rack, then back to the menu
            anagram_finder(player);
            skip_display = 1;
        }
        else if(strcmp(option, "LOAD")==0){
            // Load a debug game state from a file
            printf("Enter state file name (e.g. example_state.txt): ");
            if(!fgets(line, sizeof(line), stdin)){
                break;
            }
            char fname[100];
            if(sscanf(line, "%99s", fname)==1){
                import_state(&game, fname);
                passes = 0;
                skip_display = 0;  // show the freshly imported board
            }
        }
        else if(strcmp(option, "PASS")==0){
            passes++;
            if(passes>=2){
                game_over = 1;
            }
            else{
                game.turn = (game.turn==1) ? 2 : 1;
            }
        }
        else if(strcmp(option, "QUIT")==0){
            game_over = 1;
        }
        else{
            printf("Unknown option. Choose PLAY, EXCH, PASS, QUIT, HIGH, ANAG or LOAD.\n");
        }
    }

    int p1_left = 0;
    for(int i=0; i<7 && game.P1.rack[i]!='\0'; i++){
        p1_left += value(game.P1.rack[i]);
    }
    int p2_left = 0;
    for(int i=0; i<7 && game.P2.rack[i]!='\0'; i++){
        p2_left += value(game.P2.rack[i]);
    }
    game.P2.score += 2*p1_left;
    game.P1.score += 2*p2_left;

    printf("\n================ GAME OVER ================\n");
    if(p1_left > 0){
        printf("Player 1 has %d points of tiles left, so Player 2 gains %d.\n",
               p1_left, 2*p1_left);
    }
    if(p2_left > 0){
        printf("Player 2 has %d points of tiles left, so Player 1 gains %d.\n",
               p2_left, 2*p2_left);
    }
    printf("Player 1: %d points\n", game.P1.score);
    printf("Player 2: %d points\n", game.P2.score);
    if(game.P1.score > game.P2.score){
        printf("Player 1 wins!\n");
    }
    else if(game.P2.score > game.P1.score){
        printf("Player 2 wins!\n");
    }
    else{
        printf("It's a tie!\n");
    }
    return 0;
}

