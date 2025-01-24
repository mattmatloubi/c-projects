#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define ASCII_SZ 256
#define BRANCHES 2
#define BYTE 8


typedef enum type {CHARACTER, SUM_NODE, ROOT} type;
typedef bool bit;

struct node {
	char c;
	int frequency;
	bit val;
	type node_type;
	struct node* left;
	struct node* right;
};
typedef struct node node;

struct queue {
	node** arr;
	int size;
};
typedef struct queue queue;


unsigned char* build_str(char* filename);
void file2str(char* filename, unsigned char* str);
queue* init_queue(unsigned char* str);
int compare(const void *a, const void *b);
void build_array(queue* q, int* c_table);
void dequeue(queue* q, int times);
int find_zeros(queue* q);
node* build_tree(queue* q);
node* cpy_node(node* n);
node* build_sum(node* left, node* right);
bool insert(queue* q, node* n);
void free_tree(node* head);
void free_queue(queue* q);
void free_zeros(queue* q, int zeros);

void generate_output(node* root);
void print_statement(node* n, char* str, int* depth, int* bits, int* original_bits);
char bin2str(int bin);

void test(void);
