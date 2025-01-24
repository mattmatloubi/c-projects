#include "mydefs.h"

int main(int argc, char** argv)
{
   test();
   if (argc != 2){
      printf("Incorrect number of arguments.\n");
      return EXIT_FAILURE;
   }
   unsigned char* str = build_str(argv[1]);
   if (str == NULL){
      printf("File doesn't exist.\n");
      return EXIT_FAILURE;
   }
   file2str(argv[1], str);
   queue* q = init_queue(str);
   free(str);
   node* root = build_tree(q);
   generate_output(root);
   free_tree(root);
   free_queue(q);
   return EXIT_SUCCESS;
}
