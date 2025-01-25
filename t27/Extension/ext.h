/* A 'tree 27' implementation
   of the Dictionary ADT */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// 26 letters, plus the '
#define ALPHA 27
#define INVALID 99
#define APOS 26
#define MAXSTR 50
#define PRIME_ONE 1398269
#define PRIME_TWO 6972593
#define PRIME_THREE 13466917
#define PRIME_SZ 1257787

// You'd normally not expose this structure
// to the user, and it's members should *never*
// be used in e.g. driver.c

struct word {
   int freq;
   char str[MAXSTR];
   struct word* next;
};
typedef struct word word;

struct s_data {
   bool found;
   char srch_str[MAXSTR];
};
typedef struct s_data s_data;

struct dict {
   word** arr;
};
typedef struct dict dict;

enum search_type {WORD_CNT, MST_CMN};
typedef enum search_type search_type;

typedef unsigned hash;


bool invalid_check(const char* str);
int char2int(char c); 
void remove_hyphen(const char* wd, char* str);
void free_chain(word* w); 
void search_dict(word* w, int* count, search_type type); 
void count_word(const word* w, int* count); 
void count_freq(const word* w, int* max); 
bool add_word(dict* p, char* str); 
hash hash_function(const char* wd); 
bool isin_dict(const dict* p, const char* wd); 
bool isin_chain(const word* w, const char* wd); 
void add_repeat(dict* p, const char* wd); 
void add_newword(dict* p, const char* wd); 
void add_chain(word* curr, const char* wd);
void str_lower(char* dest, const char* src);

// from general.c
void on_error(const char* s);
void* ncalloc(int n, size_t size);


// Creates new dictionary
dict* dict_init(void);

/* Top of Dictionary = p,
   add word str. Return false
   if p or str is NULL, or if word
   is already in the dictionary.
   True otherwise.
*/
bool dict_addword(dict* p, const char* wd);

/* Total number of times that any words
   have been added into the tree =
   sum of freq count of all terminals.*/
int dict_wordcount(const dict* p);

/* Returns the dict* where str is
   marked as 'terminal', or else NULL.
*/
dict* dict_spell(const dict* p, const char* str);

/* Frees all memory used by dictionary p.
   Sets the original pointer back to NULL */
void dict_free(dict** p);

/* Returns number of times most common
   word in dictionary has been added
   (when you insert a word and it already
   exists, increment the count)
*/
int dict_mostcommon(const dict* p);

void test(void);
