#include "bingrid.h"
#define BOARDSTR (MAX*MAX+1)

void fill_row(board* brd, const int row, const char fill, const char condition);
void fill_col(board* brd, const int col, const char fill, const char condition);
void printboard(board* brd);
void pairs(board* brd);
void oxo(board* brd);
void counting(board* brd);
void pairs_algo(board* brd, const char condition, const char fill, const int i, const int j);
void oxo_algo(board* brd, const char condition, const char fill, const int i, const int j);
void counting_algo(board* brd, bool by_row, int one_sum[], int zero_sum[]);
bool sqr_check(int str_len);



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
         brd -> b2d[j][k] = str[index];
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
         str[index] = brd -> b2d[i][j];
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
      pairs(brd);
      oxo(brd);
      counting(brd);
      board2str(str, brd);
      
   } 
   while (strcmp(str, temp_str));
   
   printboard(brd);
 
   int i = 0;
   while(str[i]){
      if (str[i] == UNK){
         return false;
      }
      i++;
   }
   return true;
}

void printboard(board* brd)
{
   for (int i = 0; i < brd -> sz; i++){
      for (int j = 0; j < brd -> sz; j++){
         printf("%c", brd -> b2d[i][j]);
      }
      printf("\n");
   }
   printf("\n");
}

void pairs(board* brd){
   for (int i = 0; i < brd -> sz; i++){
      for (int j = 0; j < brd -> sz; j++){ 
         pairs_algo(brd, ONE, ZERO, i, j);
         pairs_algo(brd, ZERO, ONE, i, j);
      }
   }
}


void counting(board* brd){

   int one_sum_rows[MAX];
   int zero_sum_rows[MAX];
   int one_sum_cols[MAX];
   int zero_sum_cols[MAX];

   counting_algo(brd, true, one_sum_rows, zero_sum_rows);
   for (int i = 0; i < brd -> sz; i++){
      if (one_sum_rows[i] == (brd -> sz / 2)){
         fill_row(brd, i, ZERO, ONE);
      }
      if (zero_sum_rows[i] == (brd -> sz / 2)){
         fill_row(brd, i, ONE, ZERO);
      }
   }
   counting_algo(brd, false, one_sum_cols, zero_sum_cols);  
   for (int i = 0; i < brd -> sz; i++){
      if (one_sum_cols[i] == (brd -> sz / 2)){
         fill_col(brd, i, ZERO, ONE);
      }
      if (zero_sum_cols[i] == (brd -> sz / 2)){
         fill_col(brd, i, ONE, ZERO);
      }
   }
}

void counting_algo(board* brd, bool by_row, int one_sum[], int zero_sum[]){
   int i, j;
   for (i = 0; i < brd -> sz; i++){
      int one_cnt = 0;
      int zero_cnt = 0;
      for (j = 0; j < brd -> sz; j++){
         if (by_row){
            if (brd -> b2d[i][j] == ONE){
               one_cnt++;
            }
            if (brd -> b2d[i][j] ==ZERO){
               zero_cnt++;
            }
         }
         else {
            if (brd -> b2d[j][i] == ONE){
               one_cnt++;
            }
            if (brd -> b2d[j][i] ==ZERO){
               zero_cnt++;
            }
         }
      }
      one_sum[i] = one_cnt;
      zero_sum[i] = zero_cnt;
   }
}



void pairs_algo(board* brd, const char condition, const char fill, const int i, const int j){
   if (brd -> b2d[i][j] == condition){
      if (brd -> b2d[i][j] == brd -> b2d[i][j+1]){ // pairs condition for rows
         if (j > 0){ //if prev is in bounds then apply
            brd -> b2d[i][j-1] = fill;
         }
         if ((j + 2) < brd -> sz){ // if next + 1 in bounds then apply
            brd -> b2d[i][j+2] = fill;
         }
      }
      if (brd -> b2d[i][j] == brd -> b2d[i+1][j]){ // pairs condition for cols
         if (i > 0){ // if prev is in bounds then apply
            brd -> b2d[i-1][j] = fill;
         }
         if ((i + 2) < brd -> sz){ // if next + 1 in bounds then apply
            brd -> b2d[i+2][j] = fill;
         }
      }
   }
}

void oxo(board* brd){
   for (int i = 0; i < brd -> sz; i++){
      for (int j = 0; j < brd -> sz - 1; j++){ 
         oxo_algo(brd, ZERO, ONE, i, j);
         oxo_algo(brd, ONE, ZERO, i, j);
      }
   }
}

void oxo_algo(board* brd, const char condition, const char fill, const int i, const int j){
   if (brd -> b2d[i][j] == condition){
      if (brd -> b2d[i][j] == brd -> b2d[i][j+2]){ // oxo condition for rows
         brd -> b2d[i][j+1] = fill;
      }
      if (brd -> b2d[i][j] == brd -> b2d[i+2][j]){ // oxo condition for cols
         brd -> b2d[i+1][j] = fill;
      }
   }
}

void fill_row(board* brd, const int row, const char fill, const char condition){
   int j = 0;
   while(j < brd -> sz){ //
      if (brd -> b2d[row][j] != condition){
         brd -> b2d[row][j] = fill;
      }
      j++;
   } 
}

void fill_col(board* brd, const int col, const char fill, const char condition){
   int i = 0;
   while(i < brd -> sz){ //
      if (brd -> b2d[i][col] != condition){
         brd -> b2d[i][col] = fill;
      }
      i++;
   } 
}

void test(void)
{
   board b;
   char str[BOARDSTR];
   int one_sum_rows[MAX];
   int zero_sum_rows[MAX];
   int one_sum_cols[MAX];
   int zero_sum_cols[MAX];

   assert(str2board(&b, "00.............."));
   pairs(&b);
   board2str(str, &b);
   assert(strcmp(str, "001.............")==0);

   assert(str2board(&b, "...1...1........"));
   pairs(&b);
   board2str(str, &b);
   assert(strcmp(str, "...1...1...0....")==0);

   assert(str2board(&b, "..............00"));
   pairs_algo(&b, ZERO, ONE, 3, 2);
   board2str(str, &b);
   assert(strcmp(str, ".............100")==0);

   assert(str2board(&b, "......1...1....."));
   pairs_algo(&b, ONE, ZERO, 1, 2);
   board2str(str, &b);
   assert(strcmp(str, "..0...1...1...0.")==0);

   assert(str2board(&b, ".1.1............"));
   oxo(&b);
   board2str(str, &b);
   assert(strcmp(str, ".101............")==0);

   assert(str2board(&b, "....0.......0..."));
   oxo(&b);
   board2str(str, &b);
   assert(strcmp(str, "....0...1...0...")==0);

   assert(str2board(&b, "1.1............."));
   oxo_algo(&b, ONE, ZERO, 0, 0);
   board2str(str, &b);
   assert(strcmp(str, "101.............")==0);

   assert(str2board(&b, "..1.......1.0000"));
   oxo_algo(&b, ONE, ZERO, 0, 2);
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
}
