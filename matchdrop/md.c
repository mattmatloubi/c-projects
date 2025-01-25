#include "mydefs.h"

bool file2str(const char* fname, char* str)
{
   if (fname == NULL || str == NULL){
      return false;
   }
   FILE* fp = fopen(fname, "r");
   if (fp == NULL){
      return false;
   }
   if (arr_overflow_guard(fname) == false){
      return false;
   }
   if (build_str(fp, str) == false){
      fclose(fp);
      return false;
   }
   fclose(fp);
   return true;
}

bool arr_overflow_guard(const char* fname)
{
   FILE* fp = fopen(fname, "r");
   char c;
   int count = 0;
   while ((c = fgetc(fp)) != EOF){
      if (c != '\r'){
         count++;
      }
   }
   fclose(fp);
   if (count >= MAXSTR){
      return false;
   }
   return true;
}

bool build_str(FILE* fp, char* str)
{
   if (hawk2str(fp, str) == false){
      return false;
   }
   if (grid2str(fp, str) == false){
      return false;
   }
   if (valid_str(str) == false){
      return false;
   }
   return true; 
}

bool hawk2str(FILE* fp, char* str)
{
   char c = fgetc(fp);
   if (isupper(c) == false){
      return false;
   }
   str[0] = c;
   return true;
}

bool grid2str(FILE* fp, char* str)
{
   char c;
   int i = HAWKSZ;
   int* ip = &i;
   while ((c = fgetc(fp)) != EOF){
      if (char_handling(c, str, ip) == false){
         return false;
      }
   }
   if (i == GRDSTART){
      return false;
   }
   if (str_ending(str, i-1, MAXSTR)){
      return true;
   }
   return false;
}

// deals with multiple or no new line characters at end of string
bool str_ending(char* str, int i, int max_len){
   if (i > 0 && i < max_len){
      while (isupper(str[i]) == false){
         str[i] = '\0';
         i--;
      }
      str[i+1] = '\0';
      return true;
   }
   return false;
}

bool char_handling(char c, char* str, int* ip)
{
   if (c != '\r'){
      if (c == '\n'){
         str[*ip] = DASH;
      }
      else if (isupper(c) == false){
         return false;
      }
      else {
         str[*ip] = c;
      }
      (*ip)++;
   }
   return true;
}

state* str2state(const char* str)
{
   if (valid_str(str) == false){
      return NULL;
   }
   state* s = calloc(1, sizeof(state));
   if (s == NULL){
      return NULL;
   }
   initialise_state(s, str);
   return s;
}

void initialise_state(state* s, const char* str)
{   
   s->rows = row_count(str);
   s->cols = col_count(str, GRDSTART);
   s->arr[0] = create_board(s,str);
}

board create_board(const state* s, const char* str)
{
   board b;
   initialise_grid(s, &b, str);
   b.hawk = str[0];
   b.parent = 0;
   b.index = 0;
   b.moves = 0;
   b.solved = false;
   return b;
}

void initialise_grid(const state* s, board* b, const char* str)
{
   int i = GRDSTART;
   for (int row = 0; row < s->rows; row++){
      for (int col = 0; col < s->cols; col++){
         b->grid[row][col] = str[i];
         i++;
      }
      i += SKIPDASH;
   }
}

bool valid_str(const char* str)
{
   if (str == NULL || str[0] == '\0'){
      return false;
   }
   if (adjacent_dash(str) == false){
      return false;
   }
   if (length_checks(str) == false){
      return false;
   }
   if (col_count(str, GRDSTART) > BRDSZ){
      return false;
   }
   if (hawk_check(str) == false){
      return false;
   }
   if (valid_chars(str) == false){
      return false;
   }
   return true;
}

bool valid_chars(const char* str)
{
   int i = 0;
   while (str[i] != '\0'){
      if (str[i] != DASH && isupper(str[i]) == false){
         return false;
      }
      i++;
   }
   return true;
}

bool length_checks(const char* str)
{
   int length = col_count(str, GRDSTART);
   if (length > BRDSZ){
      return false;
   }
   int i = GRDSTART + length;
   while (str[i] != '\0'){
      i += DASHSZ;
      int new_length = col_count(str, i);
      if (length != new_length){
         return false;
      }
      if (new_length > BRDSZ){
         return false;
      }
      i += new_length; 
   }
   return true;
}

int col_count(const char* str, int start_index)
{
   int len = 0;
   int i = start_index;
   while (str[i] != DASH && str[i] != '\0'){
      len++;
      i++;
   }
   return len;
}

// deals with files with consecutive new line chars
bool adjacent_dash(const char* str)
{
   int i = HAWKSZ;
   while (str[i] != '\0'){
      if (str[i] == DASH && str[i-1] == DASH){
         return false;
      }
      i++;
   }
   return true;
}

int row_count(const char* str)
{
   int i = 0;
   int dash_cnt = 0;
   while (str[i] != '\0'){
      if (str[i] == DASH){
         dash_cnt++;
      }
      i++;
   }
   return dash_cnt;
}

bool hawk_check(const char* str)
{
   if (isupper(str[0]) == false){
      return false;
   }
   else if (str[1] != DASH){
      return false;
   }
   return true;
}

int solve(state* s, bool verbose)
{
   if (s == NULL){
      return IMPOS;
   }
   if (f0_solved(s) == true){
      if (verbose == true){
         return verbose_flag(s);
      }
      else {
         return s->moves;
      }
   }
   while (s->solved == false){
      if (solve_algo(s) == false){
         return IMPOS;
      }
      if (impos_by_repetition(s) == true){
         return IMPOS;
      }
      s->f++;
   }
   if (verbose == true){
      return verbose_flag(s);
   }
   return s->moves;
}

int verbose_flag(state* s)
{
   board verbose_arr[LARGEARR];
   board solution = s->arr[s->count];
   fill_verbose_arr(s, solution, verbose_arr);
   for (int i = 0; i <= s->moves; i++){
      print_board(&verbose_arr[i], s);
   }
   return s->moves;
}

void fill_verbose_arr(state* s, board solution, board* arr)
{
   int i = s->moves;
   do {
      arr[i] = solution;
      solution = s->arr[solution.parent];
      i--;
   } while (i >= 0);
}

void print_board(const board* b, const state* s)
{
   for (int row = 0; row < s->rows; row++){
      for (int col = 0; col < s->cols; col++){
         printf("%c", b->grid[row][col]);
      }
      printf("\n");
   }
   printf("\n");
}

bool solve_algo(state* s)
{
   board parent = s->arr[s->f];
   for (int col = 0; col < s->cols; col++){
      board child = create_child(s, parent, col);
      if (is_solved(s, &child)){
         to_state(s, child, s->count+1, &parent, SOLVED);
         return true;
      }
      else if (is_unique(s, &child)){
         to_state(s, child, s->count+1, &parent, UNSOLVED);
      }
      if (impos_by_count(s)){
         return false;
      }
   }
   return true;
}

bool impos_by_count(const state* s)
{
   if (s->count < MAXBRDS-1){
      return false;
   }
   if (s->solved == true){
      return false;
   }
   return true;
}

bool impos_by_repetition(const state* s)
{
   if (s->f < s->count){
      return false;
   }
   if (s->solved == true){
      return false;
   }
   return true;
}

bool f0_solved(state* s)
{
   if (is_solved(s, &s->arr[0])){
      s->solved = true;
      return true;
   }
   return false;
}

board create_child(state* s, board parent, int column)
{
   board child = parent;
   if (col_complete(s, &child, column) == false){
      hawk_swap(s, &child, column);
   }
   return child;
}

void hawk_swap(const state* s, board* b, int column)
{
   char temp = b->hawk;
   b->hawk = b->grid[s->rows-1][column];
   for (int row = s->rows-1; row > 0; row--){
      b->grid[row][column] = b->grid[row-1][column];
   }
   b->grid[0][column] = temp;
}

bool col_complete(const state* s, const board* b, int column)
{
   for (int row = 1; row < s->rows; row++){
      if (b->grid[row-1][column] != b->grid[row][column]){
         return false;
      }
   }
   return true;
}

void to_state(state* s, board b, int counter, const board* parent, bool is_solved)
{
   update_board(&b, counter, parent, is_solved);
   update_state(s, &b, is_solved);
}

void update_board(board* b, int counter, const board* parent, bool is_solved)
{
   b->moves++;
   b->parent = parent->index;
   b->index = counter;
   b->solved = is_solved;
}

void update_state(state* s, const board* b, bool is_solved)
{
   s->count++;
   s->solved = is_solved;
   s->moves = b->moves;
   s->arr[s->count] = *b;
}

bool is_solved(state* s, const board* b)
{
   for (int col = 0; col < s->cols; col++){
      if (col_complete(s, b, col) == false){
         return false;
      }
   }
   return true;
}

bool is_unique(state* s, board* b)
{
   char b_str[BRDSZ*BRDSZ+1];
   board2str(s, b, b_str);
   for (int i = s->count; i >= 0; i--){
      char comp_str[BRDSZ*BRDSZ+1];
      board2str(s, &s->arr[i], comp_str);
      if (strcmp(b_str, comp_str) == 0){
         return false;
      }
   }
   return true;
}

void board2str(const state* s, const board* b, char* str)
{
   int i = 0;
   for (int row = 0; row < s->rows; row++){
      for (int col = 0; col < s->cols; col++){
         str[i] = b->grid[row][col];
         i++;
      }
   }
   str[i] = '\0';
}

void test(void)
{
   solve_tests();
   file_str_tests();
   verbose_tests();
}

void solve_tests(void)
{
   state* s1 = str2state("A-AAA");
   assert(s1);
   s1->f = 4;
   s1->count = 5;
   s1->solved = false;
   assert(impos_by_repetition(s1) == false);
   s1->f = 99998;
   assert(impos_by_repetition(s1) == true);
   s1->count = 99999;
   assert(impos_by_repetition(s1) == false);
   assert(impos_by_count(s1) == false);
   s1->count = MAXBRDS-1;
   assert(impos_by_count(s1) == true);
   s1->solved = true;
   assert(impos_by_count(s1) == false);
   free(s1);

   state* s2 = str2state("A-AAA-AAA-AAA");
   assert(s2);
   assert(is_solved(s2, &s2->arr[0]) == true);
   assert(s2->solved == false);
   assert(f0_solved(s2) == true);
   assert(s2->solved == true);
   assert(col_complete(s2, &s2->arr[0], 0) == true);
   free(s2);
   state* s3 = str2state("A-AAB-AAA-AAA");
   assert(s3);
   assert(is_solved(s3, &s3->arr[0]) == false);
   assert(s3->solved == false);
   assert(f0_solved(s3) == false);
   assert(s3->solved == false);
   assert(col_complete(s3, &s3->arr[0], 2) == false);
   free(s3);
   state* s4 = str2state("Z-C");
   assert(s4);
   assert(is_solved(s4, &s4->arr[0]) == true);
   assert(s4->solved == false);
   assert(f0_solved(s4) == true);
   assert(s4->solved == true);
   assert(col_complete(s4, &s4->arr[0], 0) == true);
   free(s4);

   state* s5 = str2state("O-COMS-COMS-COMS-CMMS-CSSO");
   assert(s5);
   solve(s5, false);
   board b0 = create_board(s5, "A-BBBB-ABCC-ABCC-ABCC-CCCC");
   assert(is_unique(s5, &b0) == true);
   free(s5);
   state* s6 = str2state("A-ABC-CBA-CAB");
   assert(s6);
   solve(s6, false);
   board b01 = create_board(s6, "A-ZZZ-ABC-ABC");
   assert(is_unique(s6, &b01) == true);
   free(s6);
   state* s7 = str2state("O-COMS-COMS-COMS-CMMS-CSSO");
   assert(s7);
   solve(s7, false);
   assert(is_unique(s7, &s7->arr[5]) == false);
   free(s7);

   state* s8 = str2state("A-AAA");
   assert(s8);
   update_state(s8, &s8->arr[0], UNSOLVED);
   assert(s8->count == 1);
   assert(s8->solved == false);
   assert(s8->moves == 0);
   char str1[STRSZ], str2[STRSZ];
   board2str(s8, &s8->arr[0], str1);
   board2str(s8, &s8->arr[1], str2);
   assert(strcmp(str1, str2) == 0);
   board b1 = create_board(s8, "A-BBB");
   update_state(s8, &b1, UNSOLVED);
   assert(s8->count == 2);
   assert(s8->solved == false);
   assert(s8->moves == 0);
   char str3[STRSZ];
   board2str(s8, &s8->arr[2], str3);
   assert(strcmp(str2, str3) != 0);
   board b2 = create_board(s8, "A-CCC");
   update_state(s8, &b2, SOLVED);
   assert(s8->count == 3);
   assert(s8->solved == true);
   assert(s8->moves == 0);
   char str4[STRSZ];
   board2str(s8, &s8->arr[3], str4);
   assert(strcmp(str3, str4) != 0);

   board b3 = create_board(s8, "A-CCC");
   board b4 = create_board(s8, "A-ZZZ");
   b3.index = 1;
   update_board(&b4, 2, &b3, UNSOLVED);
   assert(b4.moves == 1);
   assert(b4.parent == 1);
   assert(b4.index == 2);
   assert(b4.solved == 0);
   board b5 = create_board(s8, "A-ZZZ");
   update_board(&b5, 5, &b4, UNSOLVED);
   assert(b5.moves == 1);
   assert(b5.parent == 2);
   assert(b5.index == 5);
   assert(b5.solved == 0);
   board b6 = create_board(s8, "A-ZZZ");
   update_board(&b6, 10, &b5, SOLVED);
   assert(b6.moves == 1);
   assert(b6.parent == 5);
   assert(b6.index == 10);
   assert(b6.solved == 1);
   free(s8);

   state* s9 = str2state("A-AAA");
   board b7 = create_board(s9, "A-QQQ");
   to_state(s9, b7, 1, &s9->arr[0], UNSOLVED);
   assert(s9->count == 1);
   assert(s9->solved == false);
   assert(s9->moves == 1);
   char str5[STRSZ], str6[STRSZ];
   board2str(s9, &s9->arr[0], str5);
   board2str(s9, &s9->arr[1], str6);
   assert(strcmp(str5, str6) != 0);
   assert(b7.moves == 0);
   assert(b7.parent == 0);
   assert(b7.index == 0);
   assert(b7.solved == 0);
   board b8 = create_board(s9, "A-KKK");
   to_state(s9, b8, 100, &s9->arr[1], SOLVED);
   assert(s9->count == 2);
   assert(s9->solved == true);
   assert(s9->moves == 1);
   char str7[STRSZ];
   board2str(s9, &s9->arr[2], str6);
   assert(strcmp(str6, str7) != 0);
   assert(b8.moves == 0);
   assert(b8.parent == 0);
   assert(b8.index == 0);
   assert(b8.solved == 0);
   free(s9);

   state* s10 = str2state ("A-ABC");
   board b9;
   board b10;
   hawk_swap(s10, &s10->arr[0], 0);
   b9 = s10->arr[0];
   assert(b9.hawk == 'A');
   assert(b9.grid[0][0] == 'A');
   b10 = create_child(s10, s10->arr[0], 0);
   assert(b10.hawk == 'A');
   assert(b10.grid[0][0] == 'A');
   hawk_swap(s10, &s10->arr[0], 1);
   b9 = s10->arr[0];
   assert(b9.hawk == 'B');
   assert(b9.grid[0][1] == 'A');
   b10 = create_child(s10, s10->arr[0], 1);
   assert(b10.hawk == 'B');
   assert(b10.grid[0][1] == 'A');
   hawk_swap(s10, &s10->arr[0], 2);
   b9 = s10->arr[0];
   assert(b9.hawk == 'C');
   assert(b9.grid[0][2] == 'B');
   b10 = create_child(s10, s10->arr[0], 2);
   assert(b10.hawk == 'C');
   assert(b10.grid[0][2] == 'B');
   free(s10);

   state* s11 = str2state("A-ABC-CBA-BCA");
   assert(s11);
   assert(solve_algo(s11));
   assert(solve(s11, false) == 5);
   free(s11);
   state* s12 = str2state("A-ZZZ-ZZZ-ZZA");
   assert(s12);
   assert(solve_algo(s12));
   assert(solve(s12, false) == -1);
   free(s12);
   state* s13 = str2state("Z-ZZZ-ZZZ-ZZA");
   assert(s13);
   assert(solve_algo(s13));
   assert(solve(s13, false) == 1);
   free(s13);
}

void file_str_tests(void)
{
   char str1[MAXSTR] = "AAAA";
   assert(str_ending(str1, MAXSTR, MAXSTR) == false);
   assert(str_ending(str1, 4, MAXSTR) == true);
   assert(strcmp(str1, "AAAA") == 0);
   char str2[MAXSTR] = "AAAA-";
   assert(str_ending(str2, 4, MAXSTR) == true);
   assert(strcmp(str2, "AAAA") == 0);

   int i = 0;
   char str3[MAXSTR] = "A\np";
   assert(char_handling(str3[i], str3, &i) == true);
   assert(char_handling(str3[i], str3, &i) == true);
   assert(char_handling(str3[i], str3, &i) == false);
   assert(strcmp(str3, "A-p") == 0);

   char str4[MAXSTR] = "P-ZZZZ-ZZZZ-ZZZZ";
   assert(row_count(str4) == 3);
   assert(col_count(str4, GRDSTART) == 4);
   char str5[MAXSTR] = "P-ZZZZZ-ZZZZZ-ZZZZZ-ZZZZZ";
   assert(row_count(str5) == 4);
   assert(col_count(str5, GRDSTART) == 5);
   char str6[MAXSTR] = "P-ZZZ-ZZZ-ZZZ-ZZZ-ZZZ";
   assert(row_count(str6) == 5);
   assert(col_count(str6, GRDSTART) == 3);

   state* s1 = calloc(1, sizeof(state));
   initialise_state(s1, str4);
   assert(s1->rows == 3);
   assert(s1->cols == 4);
   assert(s1->solved == false);
   board b1;
   initialise_grid(s1, &b1, "P-AAAA-AAAA-AAAA");
   assert(b1.grid[0][0] == 'A');
   assert(b1.grid[2][2] == 'A');
   board b2 = create_board(s1, "P-AAAA-AAAA-AAAA");
   assert(b2.grid[0][0] == 'A');
   assert(b2.grid[2][2] == 'A');
   assert(b2.hawk == 'P');
   assert(b2.parent == 0);
   assert(b2.index == 0);
   assert(b2.moves == 0);
   assert(b2.solved == false);

   initialise_state(s1, str5);
   assert(s1->rows == 4);
   assert(s1->cols == 5);
   assert(s1->moves == 0);
   assert(s1->solved == false);
   board b3;
   initialise_grid(s1, &b3, "Z-AAAAA-AAAAA-AABAA-AAAAA");
   assert(b3.grid[0][0] == 'A');
   assert(b3.grid[2][2] == 'B');
   board b4 = create_board(s1, "Z-AAAAA-AAAAA-AABAA-AAAAA");
   assert(b4.grid[0][0] == 'A');
   assert(b4.grid[2][2] == 'B');
   assert(b4.hawk == 'Z');
   assert(b4.parent == 0);
   assert(b4.index == 0);
   assert(b4.moves == 0);
   assert(b4.solved == false);
   free(s1);

   char str7[MAXSTR] = "*-AAA-AAAA";
   assert(adjacent_dash(str7) == true);
   assert(hawk_check(str7) == false);
   assert(length_checks(str7) == false);
   assert(valid_chars(str7) == false);
   assert(valid_str(str7) == false);
   char str8[MAXSTR] = "A-AAA--AAA";
   assert(adjacent_dash(str8) == false);
   assert(hawk_check(str8) == true);
   assert(valid_chars(str8) == true);
   assert(valid_str(str8) == false);
   char str9[MAXSTR] = "A+AAA-AAA--";
   assert(adjacent_dash(str9) == false);
   assert(hawk_check(str9) == false);
   assert(valid_str(str9) == false);
   char str10[MAXSTR] = "Z-ZZZ-ZZZ-ZZ";
   assert(length_checks(str10) == false);
   char str11[MAXSTR] = "Z-ZZZ-ZZZ-ZZZ";
   assert(length_checks(str11) == true);
   assert(valid_chars(str11) == true);
   assert(valid_str(str11) == true);

   FILE* fp1 = fopen("4moves.txt", "r");
   char str12[BRDSZ*BRDSZ+BRDSZ+2];
   assert(hawk2str(fp1, str12) == true);
   assert(str12[0] == 'O');
   assert(grid2str(fp1, str12) == true);
   assert(str12[1] == DASH);
   assert(str12[5] == 'S');
   fclose(fp1);

   FILE* fp2 = fopen("10moves.brd", "r");
   char str13[BRDSZ*BRDSZ+BRDSZ+2];
   assert(hawk2str(fp2, str13) == true);
   assert(str13[0] == 'C');
   assert(grid2str(fp2, str13) == true);
   assert(str13[1] == DASH);
   assert(str13[8] == 'B');
   fclose(fp2);

   FILE* fp3 = fopen("4moves.txt", "r");
   assert(arr_overflow_guard("4moves.txt") == true);
   char str14[BRDSZ*BRDSZ+BRDSZ+2];
   assert(build_str(fp3, str14) == true);
   fclose(fp3);
   FILE* fp4 = fopen("10moves.brd", "r");
   assert(arr_overflow_guard("10moves.brd") == true);
   char str15[BRDSZ*BRDSZ+BRDSZ+2];
   assert(build_str(fp4, str15) == true);
   fclose(fp4);
   FILE* fp5 = fopen("11moves.brd", "r");
   assert(arr_overflow_guard("11moves.brd") == true);
   char str16[BRDSZ*BRDSZ+BRDSZ+2];
   assert(build_str(fp5, str16) == true);
   fclose(fp5);

   state* s2 = str2state(NULL);
   assert(s2 == NULL);
   free(s2);
   state* s3 = str2state("A-AAA-AAA");
   assert(s3);
   free(s3);
   state* s4 = str2state("A-A&A-AAA");
   assert(s4 == NULL);
   free(s4);
}

void verbose_tests(void)
{
   state* s1 = str2state("Z-ZZZ-ZZZ-ZAZ");
   assert(solve(s1, false) == 1);
   board arr[10000];
   fill_verbose_arr(s1, s1->arr[1], arr);
   char str1[STRSZ], str2[STRSZ];
   board2str(s1, &arr[0], str1);
   board2str(s1, &arr[1], str2);
   assert(strcmp("ZZZZZZZAZ", str1) == 0);
   assert(strcmp("ZZZZZZZZZ", str2) == 0);
   free(s1);
}
