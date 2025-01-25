#include "bingrid.h"
#define BOARDSTR (MAX*MAX+1)

// fills unknowns in row with fill
void fill_row(board* brd, const int row, const char fill);

// same as above but with cols
void fill_col(board* brd, const int col, const char fill);

void counting(board* brd);
void counting_algo(board* brd, bool by_row, int one_sum[], int zero_sum[]);
void pairs_algo(board* brd, const int row, const int col);
void oxo_algo(board* brd, const int row, const int col);
bool sqr_check(int str_len);
char opposite(char one_zero);

// checks if indices in question are in bounds and if they are unknown
bool unk_bounds_check(board* brd, int row, int col, int row_shift, int col_shift);

bool str2board(board* brd, char* str){
   int str_len = strlen(str);
   brd -> sz = sqrt(str_len);

   if (str_len == 0){
      return false;
   }

   if (!sqr_check(str_len)){
      return false;
   }

   int index = 0;
   for (int j = 0; j < brd -> sz; j++){
      for (int k = 0; k < brd -> sz; k++){
         brd->b2d[j][k] = str[index];
         index++;
      }
   }
   return true;
}

bool sqr_check(int str_len){
   int i = 2;
   bool square = false;
   while (i <= MAX && square == false){
      if ((str_len / i) == i){
         square = true;
      }
      i+=2;
   }
   return square;
}
      

void board2str(char* str, board* brd)
{  
   if (brd -> sz == 0){
      str[0] = '\0';
      return;
   }

   int index = 0;
   for (int i = 0; i < brd -> sz; i++){
      for (int j = 0; j < brd -> sz; j++){
         str[index] = brd->b2d[i][j];
         index++;
      }
   }
   str[index] = '\0';
}

bool solve_board(board* brd)
{  
   char str[BOARDSTR];
   char temp_str[BOARDSTR];

   do {
      board2str(temp_str, brd);
      for (int row = 0; row < brd -> sz; row++){
         for (int col = 0; col < brd -> sz; col++){
            pairs_algo(brd, row, col);
            oxo_algo(brd, row, col);
         }
      }
      counting(brd);
      board2str(str, brd);
   } 
   while (strcmp(str, temp_str));
 
   int i = 0;
   while(str[i]){
      if (str[i] == UNK){
         return false;
      }
      i++;
   }
   return true;
}

void counting(board* brd)
{
   int one_sum_rows[MAX];
   int zero_sum_rows[MAX];
   int one_sum_cols[MAX];
   int zero_sum_cols[MAX];

   counting_algo(brd, true, one_sum_rows, zero_sum_rows);
   for (int i = 0; i < brd -> sz; i++){
      if (one_sum_rows[i] == (brd -> sz / 2)){
         fill_row(brd, i, ZERO);
      }
      else if (zero_sum_rows[i] == (brd -> sz / 2)){
         fill_row(brd, i, ONE);
      }
   }
   counting_algo(brd, false, one_sum_cols, zero_sum_cols);  
   for (int i = 0; i < brd -> sz; i++){
      if (one_sum_cols[i] == (brd -> sz / 2)){
         fill_col(brd, i, ZERO);
      }
      else if (zero_sum_cols[i] == (brd -> sz / 2)){
         fill_col(brd, i, ONE);
      }
   }
}

void counting_algo(board* brd, bool by_row, int one_sum[], int zero_sum[])
{
   int i, j;
   for (i = 0; i < brd -> sz; i++){
      int one_cnt = 0;
      int zero_cnt = 0;
      for (j = 0; j < brd -> sz; j++){
         if (by_row){
            if (brd->b2d[i][j] == ONE){
               one_cnt++;
            }
            else if (brd->b2d[i][j] ==ZERO){
               zero_cnt++;
            }
         }
         else {
            if (brd->b2d[j][i] == ONE){
               one_cnt++;
            }
            else if (brd->b2d[j][i] ==ZERO){
               zero_cnt++;
            }
         }
      }
      one_sum[i] = one_cnt;
      zero_sum[i] = zero_cnt;
   }
}

void pairs_algo(board* brd, const int row, const int col)
{
   if (brd->b2d[row][col] == UNK){
      if ((unk_bounds_check(brd, row, col, -2, 0)) && (brd->b2d[row-1][col] == brd->b2d[row-2][col])){
         brd->b2d[row][col] = opposite(brd->b2d[row-1][col]);
      }
      else if ((unk_bounds_check(brd, row, col, 0, 2)) && (brd->b2d[row][col+1] == brd->b2d[row][col+2])){
         brd->b2d[row][col] = opposite(brd->b2d[row][col+1]);
      }
      else if ((unk_bounds_check(brd, row, col, 2, 0)) && (brd->b2d[row+1][col] == brd->b2d[row+2][col])){
         brd->b2d[row][col] = opposite(brd->b2d[row+1][col]);
      }
      else if ((unk_bounds_check(brd, row, col, 0, -2)) && (brd->b2d[row][col-1] == brd->b2d[row][col-2])){
            brd->b2d[row][col] = opposite(brd->b2d[row][col-1]);
      }  
   }
}

void oxo_algo(board* brd, const int row, const int col)
{
   if (brd->b2d[row][col] == UNK){
      if (unk_bounds_check(brd, row, col, -1, 0) && (unk_bounds_check(brd, row, col, 1, 0))){
         if(brd->b2d[row-1][col] == brd->b2d[row+1][col]){
            brd->b2d[row][col] = opposite(brd->b2d[row-1][col]);
            return;
         }
      }
      if (unk_bounds_check(brd, row, col, 0, -1) && (unk_bounds_check(brd, row, col, 0, 1))){
         if(brd->b2d[row][col-1] == brd->b2d[row][col+1]){
            brd->b2d[row][col] = opposite(brd->b2d[row][col-1]);
         }
      }
   }
}

void fill_row(board* brd, const int row, const char fill)
{
   for (int j = 0; j < brd -> sz; j++){
      if (brd->b2d[row][j] == UNK){
         brd->b2d[row][j] = fill;
      }
   } 
}

void fill_col(board* brd, const int col, const char fill)
{
   for (int i = 0; i < brd -> sz; i++){
      if (brd->b2d[i][col] == UNK){
         brd->b2d[i][col] = fill;
      }
   } 
}

bool unk_bounds_check(board* brd, int row, int col, int row_shift, int col_shift)
{
   int row_new = row + row_shift;
   int col_new = col + col_shift;
   if ((row_new < 0) || (row_new >= brd -> sz) || (col_new < 0) || (col_new >= brd->sz)){
      return false;
   }
   if (brd->b2d[row_new][col_new] == UNK){
   return false;
   }
   return true;
}

// doesn't deal with unknowns because only 1 or 0 will be passed in here
char opposite(char one_zero)
{
   if (one_zero == ONE){
      return ZERO;
   }
   return ONE;
}

void test(void)
{
   board b;
   char str[BOARDSTR];
   int one_sum_rows[MAX];
   int zero_sum_rows[MAX];
   int one_sum_cols[MAX];
   int zero_sum_cols[MAX];

   assert(str2board(&b, "..............00"));
   pairs_algo(&b, 3, 1);
   board2str(str, &b);
   assert(strcmp(str, ".............100")==0);

   assert(str2board(&b, "......1...1....."));
   pairs_algo(&b, 0, 2);
   board2str(str, &b);
   assert(strcmp(str, "..0...1...1.....")==0);

   assert(str2board(&b, "1.1............."));
   oxo_algo(&b, 0, 1);
   board2str(str, &b);
   assert(strcmp(str, "101.............")==0);

   assert(str2board(&b, "..1.......1.0000"));
   oxo_algo(&b, 1, 2);
   board2str(str, &b);
   assert(strcmp(str, "..1...0...1.0000")==0);

   assert(str2board(&b, "111...........................110..."));
   counting(&b);
   board2str(str, &b);
   assert(strcmp(str, "111000........................110...")==0);

   assert(str2board(&b, ".....0.....0...........0............"));
   counting(&b);
   board2str(str, &b);
   assert(strcmp(str, ".....0.....0.....1.....0.....1.....1")==0);

   assert(str2board(&b, ".....0.....0...........0............"));
   counting(&b);
   board2str(str, &b);
   assert(strcmp(str, ".....0.....0.....1.....0.....1.....1")==0);

   assert(str2board(&b, "11..00..1.....01"));
   counting_algo(&b, true, one_sum_rows, zero_sum_rows);
   assert(one_sum_rows[0]==2);
   assert(one_sum_rows[1]==0);
   assert(one_sum_rows[2]==1);
   assert(one_sum_rows[3]==1);
   assert(zero_sum_rows[0]==0);
   assert(zero_sum_rows[1]==2);
   assert(zero_sum_rows[2]==0);
   assert(zero_sum_rows[3]==1);

   counting_algo(&b, false, one_sum_cols, zero_sum_cols);
   assert(one_sum_cols[0]==2);
   assert(one_sum_cols[1]==1);
   assert(one_sum_cols[2]==0);
   assert(one_sum_cols[3]==1);
   assert(zero_sum_cols[0]==1);
   assert(zero_sum_cols[1]==1);
   assert(zero_sum_cols[2]==1);
   assert(zero_sum_cols[3]==0);

   assert(str2board(&b, ".....0.....0...........0............"));
   fill_col(&b, 5, ONE);
   board2str(str, &b);
   assert(strcmp(str, ".....0.....0.....1.....0.....1.....1")==0);

   assert(str2board(&b, "1.....1.....0.....1................."));
   fill_col(&b, 0, ZERO);
   board2str(str, &b);
   assert(strcmp(str, "1.....1.....0.....1.....0.....0.....")==0);

   assert(str2board(&b, "111...........................110..."));
   fill_row(&b, 0, ZERO);
   board2str(str, &b);
   assert(strcmp(str, "111000........................110...")==0);

   assert(str2board(&b, "......0.0.0...................110..."));
   fill_row(&b, 1, ONE);
   board2str(str, &b);
   assert(strcmp(str, "......010101..................110...")==0);

   assert(str2board(&b, "......0.0.0...................110..."));
   assert(unk_bounds_check(&b, 0, 5, 0, 1) == false);
   assert(unk_bounds_check(&b, 5, 3, 1, 0) == false);
   assert(unk_bounds_check(&b, 5, 0, 0, 1) == true);
   assert(unk_bounds_check(&b, 1, 0, 0, 2) == true);

   assert(opposite(ONE) == ZERO);
   assert(opposite(ZERO) == ONE);

   assert(sqr_check(16) == true);
   assert(sqr_check(9) == false);
   assert(sqr_check(10) == false);
   assert(sqr_check(25) == false);

}
