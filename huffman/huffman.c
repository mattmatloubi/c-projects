#include "mydefs.h"

unsigned char* build_str(char* filename)
{
   FILE* fp = fopen(filename, "r");
   if (fp == NULL){
      return NULL;
   }
   int i = 0;
   char c;
   while ((c = fgetc(fp)) != EOF){
      i++;
   }
   unsigned char* str = malloc(i+1);
   fclose(fp);
   return str;
}

void file2str(char* filename, unsigned char* str)
{
   FILE* fp = fopen(filename, "r");
   int i = 0;
   char c;
   while ((c = fgetc(fp)) != EOF){
      str[i] = c;
      i++;
   }
   str[i] = '\0';
   fclose(fp);
}

queue* init_queue(unsigned char* str)
{
   int i = 0;
   int c_table[ASCII_SZ] = {0};
   while (str[i] != '\0'){
      c_table[(unsigned char)str[i]]++;
      i++;
   }
   queue* q = calloc(1, sizeof(queue));
   q->arr = calloc(ASCII_SZ, sizeof(node*));
   build_array(q, c_table);
   qsort(q->arr, ASCII_SZ, sizeof(node*), compare);
   q->size = ASCII_SZ;
   int zeros = find_zeros(q);
   free_zeros(q, zeros);
   dequeue(q, zeros);
   return q;
}

int compare(const void *a, const void *b)
{
   const node* node_a = *(const node**)a;
   const node* node_b = *(const node**)b;
   if (node_a->frequency < node_b->frequency){
      return -1;     
   } 
   if (node_a->frequency > node_b->frequency){
      return 1;
   } 
   return 0;
}

void build_array(queue* q, int* c_table)
{
   for (int i = 0; i < ASCII_SZ; i++){
      q->arr[i] = calloc(1, sizeof(node));
      q->arr[i]->c = i;
      q->arr[i]->frequency = c_table[i];
      q->arr[i]->node_type = CHARACTER;
      q->arr[i]->left = NULL;
      q->arr[i]->right = NULL;
   }
   q->size = ASCII_SZ; 
}

void dequeue(queue* q, int times)
{
   for (int i = 0; i < (q->size - times); i++){
      q->arr[i] = q->arr[i+times];
   }
   q->size = q->size - times;
}

int find_zeros(queue* q)
{
   int i = 0;
   while (q->arr[i]->frequency == 0){
      i++;
   }
   return i;
}

// where the magic happens
node* build_tree(queue* q)
{
   node* root = NULL;
   if (q->size <= 1) {
      if (q->size == 1) {
         root = cpy_node(q->arr[0]);
         root->node_type = ROOT;
      }
      return root;
   }
   while (q->size >= 1) {
      node* n1 = q->arr[0];
      node* n2 = q->arr[1];
      dequeue(q, BRANCHES);
      node* sum = build_sum(n1, n2);
      if (q->size == 0) {
         sum->node_type = ROOT;
         return sum;
      } 
      insert(q, sum);
   }
   return NULL;
}

node* cpy_node(node* n)
{
   if (n == NULL){
      return NULL;
   }
   node* cpy = malloc(sizeof(node));
   if (cpy == NULL){
      return NULL;
   }
   *cpy = *n;
   return cpy;
}

node* build_sum(node* left, node* right)
{
   if (left == NULL || right == NULL){
      return NULL;
   }
   node* n = malloc(sizeof(node));
   if (n == NULL){
      return NULL;
   }
   n->node_type = SUM_NODE;
   n->frequency = left->frequency + right->frequency;
   n->left = left;
   n->right = right;
   n->left->val = 0;
   n->right->val = 1;
   return n;
}

// priority queue mechanic ish?
bool insert(queue* q, node* n)
{
   if (q->size >= ASCII_SZ){
      return false;  
   }
   int j = 0;
   while (j < q->size && q->arr[j]->frequency < n->frequency){
      j++;
   }
   for (int i = (q->size - 1); i >= j; i--){
      q->arr[i+1] = q->arr[i];
   }
   q->arr[j] = n;
   q->size++;
   return true;
}

// frees all the allocated memory within build_array
// starts off as an array of nodes but ends up with a giant huffman tree
void free_tree(node* root) 
{ 
   if (root == NULL) {
      return;
   }
   free_tree(root->left);
   free_tree(root->right);
   free(root);
}

void free_zeros(queue* q, int zeros)
{
   for (int i = 0; i < zeros; i++){
      free(q->arr[i]);
   }
}

void free_queue(queue* q)
{
   free(q->arr);
   free(q);
}

void test(void)
{
   char filename[] = "test.txt";
   FILE* fp = fopen(filename, "w");
   fprintf(fp, "aabbccdd"); 
   fclose(fp);

   unsigned char* str = build_str(filename);
   assert(str != NULL);
   file2str(filename, str);

   queue* q = init_queue(str);
   assert(q != NULL);
   assert(q->size > 0);

   bool found_a = false, found_b = false;
   for (int i = 0; i < q->size; i++) {
      if (q->arr[i]->c == 'a') {
         assert(q->arr[i]->frequency == 2);
         found_a = true;
      }
      if (q->arr[i]->c == 'b') {
         assert(q->arr[i]->frequency == 2);
         found_b = true;
      }
   }
   assert(found_a && found_b);

   node* root = build_tree(q);
   assert(root != NULL);
   assert(root->node_type == ROOT);
   assert(root->frequency == 8); 

   assert(root->left != NULL);
   assert(root->right != NULL);


   free_tree(root);
   free_queue(q);
   free(str);
   remove(filename);
}

