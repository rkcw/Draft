//You are NOT ALLOWED to add any other libraries
//
#include "Task2.h"

#include <stdio.h>
#include <stdlib.h>
#include "lib/dict.h"
//

//You are allowed to define helpers in this file to assist you in implementing the required functions. However, you are NOT allowed to modify the function signatures of the provided functions in Task2.h.

// PART A (MANDATORY)

/*
Task: Implement a simple insertion sort to sort the player's rack in alphabetical order. This function should modify the player's rack in place. The wildcard * should come after Z in the rack.
Insertion sort logic:
1. Start at the 2nd element
2. Compare the element directly before it, and swap the 2 elements of the element directly before is larger
3. Repeat step 2 until the element directly before it is smaller, or the element reached the start of the list
4. Go to the next element and repeat the process. (Proceed to the 3rd element, repeat steps 2 and 3, then go to the 4th element, repeat steps 2 and 3, and so on.)
tbh you can use any sorting algorithm you like but insertion is the easiest by far.
*/
void sort_rack(Player* player){
    //TODO for the student.
}

/*
Task: Implement the logic to exchange tiles in the player's rack.
This function should remove the specified tiles from the player's rack and replace them with new tiles from the bag.
If the exchange is invalid (player does not have the requested tiles), the function should return 0. Otherwise, it should return 1.
*/
int exchange_tiles(Game* game, Player* player, const char* tiles){
    //TODO for the student.
}

/*
Task: Implement the logic when a certain word is played on the board.
Assume the word is valid, and calculate the score of the word after applying the modifiers, and the bingo score bonus.
After the calculation, return the score of the word.
*/
int word_score(Game* game, const char* word, int row, int col, char direction){
    //TODO for the student.
}

/*Task: Implement the logic to play a valid word on the board.
This function should update the board, the player's rack, and the player's score accordingly.
The direction parameter will be either 'H' for horizontal or 'V' for vertical.
If play is not valid in any way, return 0, else return 1.
When placed on the board, wildcards are displayed as the letter they represent, but in lowercase.

Logic: 
First, check of the word is playable in that location/whether the player can actually play that word. (Remember, if the board is empty, the first word must be covering the center square.)
Then, check whether the word itself is valid.
Check if word is valid -> if invalid, return.
Check if word is valid -> if valid, check whether the overlaps (cross words) are also valid words
Edit word on board -> update player's rack -> calculate word score -> update player's score

*/
int play_valid_word(Game* game, Player* player, const char* word, int row, int col, char direction){
    //TODO for the student.
}



//PART B (OPTIONAL)

/*
Task: Implement a function to find all words a player can play using only the tiles on their rack.
After finding them out, print them out in the terminal.  (In order of length in descending order, then alphabetically)
(Reminder: Wildcards are displayed as the letter they represent, but in lowercase.)
*/
void anagram_finder(Player* player) {
    //TODO for the student.
}

/*
Task: Implement an algorithm to find the highest scoring next move.
And returns the highest scoring move as a string.
(This is a very difficult task to complete, and using an exhaustive search algorithm might not be the best choice.)
*/

char* highest_score(Game* game, Player* player) {
    //TODO for the student.
    return "PASS";
}
