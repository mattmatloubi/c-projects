#include "mydefs.h"

void generate_output(node* root)
{
   char str[1000]; // if depth > 1000 i guess im screwed but prob not possible given ascii sz?
   int depth = 0; 
   int bits = 0;
   int original_bits = 0;
   print_statement(root, str, &depth, &bits, &original_bits);
   printf("%d bits\n", bits);
   printf("%.2f bytes\n", (float)bits/8.0);
   printf("%.2f kilobytes\n", (float)bits/8000.0);
   printf("Original size: %.2f kilobytes (given 1 byte per char)\n", (float)original_bits/8000.0);
   printf("Compression rate: %.1f%% (excluding tree storage)\n", (1.0 - ((float)bits/(float)original_bits))*100.0);
}


// bit of a monstrosity but whatever
void print_statement(node* n, char* str, int* depth, int* bits, int* original_bits)
{
   if (n->node_type != ROOT){
      str[*depth] = bin2str(n->val);
      str[(*depth)+1] = '\0';
      (*depth)++;
      if (n->node_type == CHARACTER){
         if (n->c == '\n'){
            printf("'\\n' : %17s ( %3d * %5d )\n", str, *depth, n->frequency);
         }
         else if (n->c == '\r'){
            printf("'\\r' : %17s ( %3d * %5d )\n", str, *depth, n->frequency);
         }
         else {               
            printf("'%c'  : %17s ( %3d * %5d )\n", n->c, str, *depth, n->frequency);
         }
         int len = strlen(str);
         (*bits) += (len * n->frequency);
         (*original_bits) += (BYTE * n->frequency);
      }
   }
   if (n->left){
      print_statement(n->left, str, depth, bits, original_bits);
   }
   if (n->right){
      print_statement(n->right, str, depth, bits, original_bits);
   }
   (*depth)--;
}

char bin2str(int bin)
{
   if (bin == 1){
      return '1';
   }
   return '0';
}


