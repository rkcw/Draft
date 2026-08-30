//DO NOT MODIFY THIS FILE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_WORD_LEN 16          // 15 characters + null terminator
#define INITIAL_TABLE_SIZE 600011  // prime number > 2 * 300000
#define DICT_FILE "lib/CSW24.txt"  // word list used for validation

typedef struct {
    char word[MAX_WORD_LEN];
    int used;                    // 0 = empty, 1 = occupied, -1 = deleted
} HashEntry;

typedef struct {
    HashEntry* entries;
    size_t size;
    size_t count;
} Dictionary;

// djb2 hash function (good distribution for strings)
static uint64_t hash_string(const char *str) {
    uint64_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }
    return hash;
}

// Create and initialize a dictionary
static Dictionary *dict_create(size_t initial_size) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (!dict) return NULL;
    dict->entries = calloc(initial_size, sizeof(HashEntry));
    if (!dict->entries) {
        free(dict);
        return NULL;
    }
    dict->size = initial_size;
    dict->count = 0;
    return dict;
}

// Insert a word into the dictionary (used internally)
static int dict_insert(Dictionary *dict, const char *word) {
    if (!dict || !word) return 0;

    // Resize if load factor > 0.7
    if (dict->count * 10 >= dict->size * 7) {
        size_t new_size = dict->size * 2 + 1;
        HashEntry *new_entries = calloc(new_size, sizeof(HashEntry));
        if (!new_entries) return 0;

        // Rehash all existing entries
        for (size_t i = 0; i < dict->size; i++) {
            if (dict->entries[i].used == 1) {
                uint64_t h = hash_string(dict->entries[i].word);
                size_t idx = h % new_size;
                while (new_entries[idx].used == 1) {
                    idx = (idx + 1) % new_size;
                }
                strcpy(new_entries[idx].word, dict->entries[i].word);
                new_entries[idx].used = 1;
            }
        }
        free(dict->entries);
        dict->entries = new_entries;
        dict->size = new_size;
    }

    uint64_t h = hash_string(word);
    size_t idx = h % dict->size;
    while (dict->entries[idx].used == 1) {
        if (strcmp(dict->entries[idx].word, word) == 0) {
            // Duplicate, ignore
            return 1;
        }
        idx = (idx + 1) % dict->size;
    }

    strcpy(dict->entries[idx].word, word);
    dict->entries[idx].used = 1;
    dict->count++;
    return 1;
}

// Free the dictionary
static void dict_free(Dictionary *dict) {
    if (!dict) return;
    free(dict->entries);
    free(dict);
}

// Load dictionary from file. Returns Dictionary* or NULL on error.
// Uses two passes over the file: first it counts the words, then it sizes
// the hash table to the minimum needed and inserts the words. This avoids
// over-allocating memory when loading the (large) dictionary.
static Dictionary *load_dictionary(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return NULL;
    }

    char line[MAX_WORD_LEN];  // clean, minimal buffer (15 chars + null)

    // Pass 1: count the words so we can size the table exactly
    size_t word_count = 0;
    while (fgets(line, sizeof(line), fp)) {
        size_t len = strlen(line);
        if (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[len-1] = '\0';
            if (len > 1 && line[len-2] == '\r') line[len-2] = '\0';
        }
        if (line[0] == '\0') continue;
        word_count++;
    }

    // Minimum table size: keep the load factor below the 0.7 resize
    // threshold, i.e. about 1.5x the number of words (~0.67 load factor).
    // This is the smallest clean size that avoids rehashing/over-allocation.
    size_t table_size = INITIAL_TABLE_SIZE;
    if (word_count > 0) {
        table_size = word_count + word_count / 2 + 1;
        if (table_size % 2 == 0) table_size++;  // keep it odd
    }

    Dictionary *dict = dict_create(table_size);
    if (!dict) {
        fclose(fp);
        return NULL;
    }

    // Pass 2: insert the words
    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        size_t len = strlen(line);
        if (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[len-1] = '\0';
            if (len > 1 && line[len-2] == '\r') line[len-2] = '\0';
        }
        if (line[0] == '\0') continue;

        if (!dict_insert(dict, line)) {
            fprintf(stderr, "Failed to insert word: %s\n", line);
            dict_free(dict);
            fclose(fp);
            return NULL;
        }
    }

    fclose(fp);
    return dict;
}

// Check if a word exists in the dictionary.
// Returns 1 if found, 0 if not.
int is_word(const Dictionary *dict, const char *word) {
    if (!dict || !word) return 0;

    uint64_t h = hash_string(word);
    size_t idx = h % dict->size;
    while (dict->entries[idx].used != 0) {
        if (dict->entries[idx].used == 1 &&
            strcmp(dict->entries[idx].word, word) == 0) {
            return 1;
        }
        idx = (idx + 1) % dict->size;
        if (idx == h % dict->size) break;
    }
    return 0;
}

// Lazily-loaded dictionary (loaded once on first call to is_valid)
static Dictionary *g_dict = NULL;

// Checks whether a word is valid according to CSW24.txt.
// Returns 1 if the word exists in the dictionary, 0 otherwise.
// Matching is case-insensitive (CSW24.txt is stored in uppercase).
int is_valid(const char *word) {
    if (!word || word[0] == '\0') return 0;

    // Load the dictionary once, on first use
    if (!g_dict) {
        g_dict = load_dictionary(DICT_FILE);
        if (!g_dict) return 0;
    }

    size_t len = strlen(word);
    if (len >= MAX_WORD_LEN) return 0;  // too long to be a valid Scrabble word

    // Normalise the word to uppercase before checking
    char upper[MAX_WORD_LEN];
    for (size_t i = 0; i <= len; i++) {
        char c = word[i];
        if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
        }
        upper[i] = c;
    }

    return is_word(g_dict, upper);
}