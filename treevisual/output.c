#include "visual.h"

void print_visual(node* root)
{
   grid* g = calloc(1, sizeof(grid));
   set_grid(root, g);
   build_grid(root, g);
   int row_m = row_marker(g);
   int col_m = col_marker(g);
   for (int row = 0; row <= row_m; row++){
      for (int col = 0; col <= col_m; col++){
         printf("%c", g->grid[row][col]);
      }
      printf("\n");
   }
   free(g);
}

void set_grid(node* root, grid* g)
{
   for (int row = 0; row < SZ; row++){
      for (int col = 0; col < SZ; col++){
         g->grid[row][col] = ' ';
      }
   }
   g->row = 0;
   g->col = 0;
   g->right_cnt = 1;
   int max = 0;
   find_max_strlen(root, &max);
   g->hdash_sz = max + BUFFER; 
}

void build_grid(node* root, grid* g)
{
   root->branch_sz = g->right_cnt;
   if (g->right_cnt > 1){
      g->right_cnt = 1;
   }
   insert_val(root, g);
   if (root->left && root->left->val[0] != '*'){
      insert_vdash(root, g);
      build_grid(root->left, g);
      g->row -= VERT;
      shift_cols(g);
   }
   if (root->right && root->right->val[0] != '*'){
      int cnt = insert_hdash(root, g);
      build_grid(root->right, g);
      g->col -= cnt; // does this even do anything?
      g->right_cnt += (root->right->branch_sz+1);
   }
}

void insert_val(node* root, grid* g)
{
   int len = strlen(root->val);
   for (int i = 0; i < len; i++){
      g->grid[g->row][g->col] = root->val[i];
      g->col++;
   }
}

void insert_vdash(node* n, grid* g)
{
   int len = strlen(n->val);
   g->col -= len;
   for (int i = 0; i < VERT; i++){
      g->row++;
      g->grid[g->row][g->col] = '|'; 
   }
}

int insert_hdash(node* n, grid* g)
{
   int curr = strlen(n->val) / 2; 
   int next = strlen(n->right->val) / 2;
   int dashes = (g->hdash_sz * g->right_cnt); 
   for (int i = 0; i < dashes; i++){
      g->col++;
      g->grid[g->row][g->col] = '-';
   }
   return (dashes + curr + next); 
}

void find_max_strlen(node* root, int* max) 
{
   int curr = strlen(root->val);
   if(curr > (*max)){
      (*max) = curr;
   }
   if (root->left){
      find_max_strlen(root->left, max);
   }
   if (root->right){
      find_max_strlen(root->right, max);
   }
   return;
}

void shift_cols(grid* g)
{
   while (g->grid[g->row][g->col] == ' '){
      g->col--;
   }
   while (g->grid[g->row][g->col] != ' '){
      g->col++;
   }
}

int row_marker(grid* g)
{
   int row_marker = 0;
   for (int row = 0; row < SZ; row++){
      for (int col = 0; col < SZ; col++){
         if (g->grid[row][col] != ' ' && row > row_marker){
            row_marker = row;
         }
      }
   }
   return row_marker + 1;
}

int col_marker(grid* g)
{
   int col_marker = 0;
   for (int row = 0; row < SZ; row++){
      for (int col = 0; col < SZ; col++){
         if (g->grid[row][col] != ' ' && col > col_marker){
            col_marker = col;
         }
      }
   }
   return col_marker + 1;
}

void output_test(void)
{
   // here lies some invisible tests...
}
