#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#define NLNODE 3
#define SZ 1000
#define BUFFER 3
#define VERT 2


typedef enum dir {LEFT, RIGHT} dir;

struct node {
	char val[100]; // realistically if len of str is >100 it deserves to crash
	struct node* left;
	struct node* right;
   int branch_sz;
};
typedef struct node node;

struct grid {
   char grid[SZ][SZ];
   int row;
   int col;
   int hdash_sz;
   int right_cnt;
};
typedef struct grid grid;



node* init_tree(char* str);
node* init_root(char* str, int *i);
void build_node(node* n, char* str, int* i);
void traverse(node* root, char* str, int* i);
bool valid_char(char c);
void open_bracket(node* root, char* str, int* i, bool left_right);
bool close_bracket(char* str, int* i);
void free_tree(node* root);
void print_visual(node* root);
void set_grid(node* root, grid* g);
void build_grid(node* root, grid* g);
void insert_val(node* root, grid* g);
void insert_vdash(node* n, grid* g);
int insert_hdash(node* n, grid* g);
void find_max_strlen(node* root, int* max);
void shift_cols(grid* g);
int row_marker(grid* g);
int col_marker(grid* g);


void test(void);

