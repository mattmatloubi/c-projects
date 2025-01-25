#include "ms.h"

// Maybe some of your own function prototypes here

// returns number of char c in the moore neighbourhood of cell[i][j]
char moore_count(board* b, int i, int j, char c);
// returns true if all mines have been discovered
bool mines_found(board* b);
// fills unknowns in moore neighbourhood with mines
void fill_moore(board* b, int i, int j, char c);

void rule_one(board* b);
void rule_two(board* b);


board solve_board(board b)
{
   char str1[MAXSQ*MAXSQ+1];
   char str2[MAXSQ*MAXSQ+1];
   do {
      board2str(str1, b);
      rule_one(&b);
      rule_two(&b);
      board2str(str2, b);
   } while (strcmp(str1, str2));

   return b;
}

void board2str(char s[MAXSQ*MAXSQ+1], board b)
{
   int str_cnt = 0;
   int i, j;
   for (i = 0; i < b.h; i++){
      for (j = 0; j < b.w; j++){
         s[str_cnt] = b.grid[i][j];
         str_cnt++;
      }
   }
   s[str_cnt] = '\0';
}

bool syntax_check(unsigned totmines, unsigned width, unsigned height, char inp[MAXSQ*MAXSQ+1])
{
   // length check
   if (strlen(inp) != (width * height)){
      return false;
   }

   // valid char check and mine check
   char valid[] = "0123456789?X";
   int i = 0, j = 0, mine_cnt = 0, valid_char = 0;
   while (inp[i]){
      if (inp[i] == MINE){
         mine_cnt++;
      }
      while (valid[j]){
         if (inp[i] == valid[j]){
            valid_char++;
         }
         j++;
      }
      if (valid_char == 0){
         return false;
      }
      valid_char = 0;
      j = 0;
      i++;
   }
   if (mine_cnt > (int) totmines){
      return false;
   }

   return true;
}

board make_board(int totmines, int width, int height, char inp[MAXSQ*MAXSQ+1])
{  
   board b;
   b.w = width;
   b.h = height;
   b.totmines = totmines;
   int str_cnt = 0;
   int i, j;
   for (i = 0; i < height; i++){
      for (j = 0; j < width; j++){
         b.grid[i][j] = inp[str_cnt];
         str_cnt++;
      }
   }
   return b;
}

void rule_one(board* b)
{  
   if (mines_found(b) == false){
      return;
   }
   int i,j;
   for (i = 0; i < b -> h; i++){
      for (j = 0; j < b -> w; j++){
         if (b -> grid[i][j] == UNK){
            b -> grid[i][j] = moore_count(b, i, j, MINE);
         }
      }
   }
}

void rule_two(board* b)
{
   int i, j;
   for (i = 0; i < b -> h; i++){
      for (j = 0; j < b -> w; j++){
         // converting to ints for the boolean test in if statement below
         int unknowns = moore_count(b, i, j, UNK) - '0';
         int mines = moore_count(b, i, j, MINE) - '0';
         int curr_cell = b -> grid[i][j] - '0';
         if ((curr_cell - mines) == unknowns){
            fill_moore(b, i, j, MINE);
         }
      }
   }
}

void fill_moore(board* b, int i, int j, char c)
{
   int row, col;
   for (row = i - 1; row < i + 2; row++){
      for (col = j - 1; col < j + 2; col++){
         // in bounds check
         if (col > -1 && row > -1 && col < b -> h && row < b -> w){
            if (b -> grid[row][col] == UNK){
               b -> grid[row][col] = c;
            }
         }
      }
   }
}

char moore_count(board* b, int i, int j, char c)
{
   int cnt = 0;
   int row, col;
   for (row = i - 1; row < i + 2; row++){
      for (col = j - 1; col < j + 2; col++){
         // in bounds check
         if (col > -1 && row > -1 && col < b -> h && row < b -> w){
            if (b -> grid[row][col] == c){
               cnt++;
            }
         }
      }
   }
   return cnt + '0';
}

bool mines_found(board* b)
{
   int i, j;
   int mine_cnt = 0;
   for (i = 0; i < b -> h; i++){
      for (j = 0; j < b -> w; j++){
         if (b -> grid[i][j] == MINE){
            mine_cnt++;
         }
      }
   }
   if (mine_cnt == b -> totmines){
      return true;
   }
   return false;
}

void test(void)
{
}
