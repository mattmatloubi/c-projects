#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <stdbool.h>

#define BRDSZ 6
#define MAXBRDS 200000
#define IMPOS -1
#define SOLVED 1
#define HAWKSZ 1
#define UNSOLVED 0
#define DASH '-'
#define DASHSZ 1
#define GRDSTART 2
#define SKIPDASH 1
#define LARGEARR 10000
#define STRSZ (BRDSZ*BRDSZ+1)
#define MAXSTR (BRDSZ*BRDSZ+BRDSZ+2)

struct board{
   char grid[BRDSZ][BRDSZ];
   char hawk;
   int parent;
   int index; 
   int moves; 
   bool solved; 
};
typedef struct board board;

struct state{
    board arr[MAXBRDS];
    int rows;
    int cols;
    int f;
    int count; 
    int moves;
    bool solved; 
};
typedef struct state state;

// solve() helpers
bool solve_algo(state* s);
board create_child(state* s, board parent, int column); 
void hawk_swap(const state* s, board* b, int column); 
void to_state(state* s, board b, int counter, const board* parent, bool is_solved); 
void update_board(board* b, int counter, const board* parent, bool is_solved); 
void update_state(state* s, const board* b, bool is_solved);
bool is_solved(state* s, const board* b); 
bool col_complete(const state* s, const board* b, int column); 
bool is_unique(state* s, board* b); 
bool f0_solved(state* s); 
bool impos_by_count(const state* s); 
bool impos_by_repetition(const state* s); 

// file and str handling
void board2str(const state* s, const board* b, char* str); 
bool arr_overflow_guard(const char* fname); 
bool build_str(FILE* fp, char* str);  
bool hawk2str(FILE* fp, char* str); 
bool grid2str(FILE* fp, char* str); 
bool valid_str(const char* str); 
bool length_checks(const char* str); 
bool hawk_check(const char* str); 
bool adjacent_dash(const char* str); 
bool valid_chars(const char* str); 
board create_board(const state* s, const char* str); 
void initialise_grid(const state* s, board* b, const char* str); 
void initialise_state(state* s, const char* str); 
int row_count(const char* str); 
int col_count(const char* str, int start_index); 
bool char_handling(char c, char* str, int* i); 
bool str_ending(char* str, int i, int max_len); 

// verbose related
int verbose_flag(state* s); 
void print_board(const board* b, const state* s); 
void fill_verbose_arr(state* s, board solution, board* arr); 

// tests
void solve_tests(void);
void file_str_tests(void);
void verbose_tests(void);


#include "md.h"
