#include "visual.h"

node* init_tree(char* str)
{
   int i = 0;
   node* root = init_root(str, &i);
   traverse(root, str, &i);
   return root;
}

node* init_root(char* str, int *i)
{
   node* root = calloc(1, sizeof(node));
   build_node(root, str, i);
   return root;
}

void build_node(node* n, char* str, int* i)
{
   int j = 0;
   while (valid_char(str[*i])){
      n->val[j] = str[*i];
      j++;
      (*i)++;
   }
   n->val[j] = '\0';
   if (n->val[0] == '-'){
      n->val[0] = '(';
      n->val[j] = ')';
      n->val[j+1] = '\0';
   }
   n->branch_sz = 1;
}

void traverse(node* root, char* str, int* i)
{
   if (close_bracket(str, i) == true){
      return;
   }
   open_bracket(root, str, i, LEFT);
   open_bracket(root, str, i, RIGHT);
   (*i)++;
}

void open_bracket(node* root, char* str, int* i, bool left_right)
{
   if (str[*i] == '('){
      (*i)++;
      node* n1 = calloc(1, sizeof(node));
      build_node(n1, str , i);
      if (left_right == LEFT){
         root->left = n1;
         traverse(root->left, str, i);
      }
      else {
         root->right = n1;
         traverse(root->right, str, i);  
      }
   }
}

bool close_bracket(char* str, int* i)
{
   if (str[*i] == ')'){
      (*i)++;
      return true;
   }
   return false;
}

bool valid_char(char c)
{
   if (isdigit(c)){
      return true;
   }
   if (c == '*'){
      return true;
   }
   if (c == '-'){
      return true;
   }
   return false;
}

void free_tree(node* root)
{    
   if (root == NULL){
      return;
   }
   free_tree(root->left);
   free_tree(root->right);
   free(root);
}

void test(void)
{
   int a = 0;
   char* test_str = "42";
   node* root1 = init_root(test_str, &a);
   assert(strcmp(root1->val, "42") == 0);
   assert(root1->left == NULL);
   assert(root1->right == NULL);
   free_tree(root1);

   char* test_str1 = "1(2)(3)";
   node* root2 = init_tree(test_str1);
   assert(strcmp(root2->val, "1") == 0);
   assert(strcmp(root2->left->val, "2") == 0);
   assert(strcmp(root2->right->val, "3") == 0);
   free_tree(root2);

   assert(valid_char('5') == true);
   assert(valid_char('*') == true);
   assert(valid_char('-') == true);
   assert(valid_char('a') == false);

   int b = 0;
   char* test_str2 = "-42";
   node* n = calloc(1, sizeof(node));
   build_node(n, test_str2, &b);
   assert(strcmp(n->val, "(42)") == 0);
   free(n);

   int c = 0;
   char* test_str3 = ")42";
   assert(close_bracket(test_str3, &c) == true);
   assert(c == 1);
}
