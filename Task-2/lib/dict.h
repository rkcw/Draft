#ifndef DICT_H
#define DICT_H

// Checks whether a word is valid according to the CSW24 word list (CSW24.txt).
// The dictionary is loaded lazily on the first call.
// Returns 1 if the word is valid, 0 otherwise.
int is_valid(const char *word);

#endif
