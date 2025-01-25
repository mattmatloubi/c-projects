#include "ext.h"

dict* dict_init(void)
{
   dict* p = (dict*)ncalloc(1, sizeof(dict));
   p->arr = (word**)ncalloc(PRIME_SZ, sizeof(word*));
   return p;
}

bool dict_addword(dict* p, const char* wd)
{
   if (p == NULL || wd == NULL){
      return false;
   }
   char new[MAXSTR];
   remove_hyphen(wd, new);
   if (invalid_check(new) == false){
      return false;
   }
   return add_word(p, new);
}

void remove_hyphen(const char* wd, char* str)
{
   int i = 0;
   int j = 0;
   while (wd[i] != '\0'){
      if (wd[i] != '-'){
         str[j] = wd[i];
         j++;
      }
      i++;
   }
   str[j] = '\0';
}

void dict_free(dict** d)
{
   for (int i = 0; i < PRIME_SZ; i++){
      if ((*d)->arr[i] != NULL){
         free_chain((*d)->arr[i]);
      }
   }
   free((*d)->arr);
   free(*d);
   *d = NULL;
}

void free_chain(word* w)
{
   if (w == NULL){
      return;
   }
   free_chain(w->next);
   free(w);
}


int dict_wordcount(const dict* p)
{
   int count = 0;
   if (p == NULL){
      return count;
   }
   for (int i = 0; i < PRIME_SZ; i++){
      if (p->arr[i] != NULL){
         search_dict(p->arr[i], &count, WORD_CNT);
      }
   }
   return count;
}

void count_word(const word* w, int* count)
{
   while (w != NULL){
      (*count) += w->freq;
      w = w->next;
   }
}

dict* dict_spell(const dict* p, const char* str)
{
   if (p == NULL || str == NULL){
      return NULL;
   }
   if (invalid_check(str) == false){
      return NULL;
   }
   char new_str[MAXSTR];
   str_lower(new_str, str);
   if (isin_dict(p, new_str) == false){
      return NULL;
   }
   return (dict*)p;
}

void str_lower(char* dest, const char* src)
{
   int i = 0;
   while (src[i] != '\0'){
      dest[i] = tolower(src[i]);
      i++;
   }
   dest[i] = '\0';
}

int dict_mostcommon(const dict* p)
{
   int count = 0;
   if (p == NULL){
      return count;
   }
   for (int i = 0; i < PRIME_SZ; i++){
      if (p->arr[i] != NULL){
         search_dict(p->arr[i], &count, MST_CMN);
      }
   }
   return count;
}

void search_dict(word* w, int* count, search_type type)
{
   if (type == WORD_CNT){
      count_word(w, count);
   }
   else if (type == MST_CMN){
      count_freq(w, count);
   }
}

void count_freq(const word* w, int* max)
{
   while (w != NULL){
      if (w->freq > (*max)){
         (*max) = w->freq;
      }
      w = w->next;
   }
}

bool invalid_check(const char* str)
{
   int i = 0;
   while (str[i] != '\0'){
      if (char2int(str[i]) == INVALID){
         return false;
      }
      i++;
   }
   if (i == 0){
      return false;
   }
   return true;
}

int char2int(char c)
{
   c = tolower(c);
   if (islower(c) == false && c != '\''){
      return INVALID;
   }
   if (c == '\''){
      return APOS;
   }
   return c - 'a';
}

bool add_word(dict* p, char* str)
{
   if (p == NULL || str == NULL){
      return false;
   }
   str_lower(str, str);
   if (isin_dict(p, str) == true){
      add_repeat(p, str);
      return false;
   }
   add_newword(p, str);
   return true;
}

hash hash_function(const char* wd)
{
   unsigned long long index = 0;
   int i = 0;
   while (wd[i] != '\0'){
      index += ((unsigned long long)wd[i] * PRIME_ONE) % PRIME_SZ;
      index = (index ^ PRIME_TWO) % PRIME_SZ;
      index = (index & PRIME_THREE) % PRIME_SZ;
      i++;
   }
   return (hash)index;
}

bool isin_dict(const dict* p, const char* wd)
{
   hash index = hash_function(wd);
   if (p->arr[index] == NULL){
      return false;
   }
   if (isin_chain(p->arr[index], wd) == false){
      return false;
   }
   return true;
}

bool isin_chain(const word* w, const char* wd)
{
   while (w != NULL){
      if (strcmp(w->str, wd) == 0){
         return true;
      }
      w = w->next;
   }
   return false;
}

void add_repeat(dict* p, const char* wd)
{
   hash index = hash_function(wd);
   word* curr = p->arr[index];
   while (strcmp(curr->str, wd) != 0){
      curr = curr->next;
   }
   curr->freq++;
}

void add_newword(dict* p, const char* wd)
{
   hash index = hash_function(wd);
   if (p->arr[index] == NULL){
      p->arr[index] = (word*)ncalloc(1, sizeof(word));
      p->arr[index]->freq = 1;
      strcpy(p->arr[index]->str, wd);
   }
   else {
      add_chain(p->arr[index], wd);
   }
}

void add_chain(word* curr, const char* wd)
{
   while (curr->next != NULL){
      curr = curr->next;
   }
   curr->next = (word*)ncalloc(1, sizeof(word));
   curr->next->freq = 1;
   strcpy(curr->next->str, wd);
}

// taken from general.c
void on_error(const char* s)
{
   fprintf(stderr, "%s\n", s);
   exit(EXIT_FAILURE);
}

// taken from general.c
void* ncalloc(int n, size_t size)
{
   void* v = calloc(n, size);
   if (v==NULL){
      on_error("Cannot ncalloc() space");
   }
   return v;
}

void test(void)
{
   dict* d = dict_init();
   assert(d);

   assert(dict_addword(d, "hello") == true);
   assert(dict_addword(d, "woRld") == true);
   assert(dict_addword(d, "t-twentyseven") == true);
   assert(dict_addword(d, "their's") == true);
   assert(dict_addword(d, "hAsh") == true);
   assert(dict_addword(d, "(false)") == false);
   assert(dict_addword(d, "tree") == true);
   assert(dict_addword(d, "tree") == false);

   assert(dict_wordcount(d) == 7); 

   assert(dict_spell(d, "hello"));
   assert(dict_spell(d, "graph") == NULL);
   assert(dict_mostcommon(d) == 2);

   char str[MAXSTR];
   remove_hyphen("tree-27", str);
   assert(strcmp(str, "tree27") == 0);
   remove_hyphen("hello-world", str);
   assert(strcmp(str, "helloworld") == 0);
   remove_hyphen("-world", str);
   assert(strcmp(str, "world") == 0);

   assert(invalid_check("Valid") == true);
   assert(invalid_check("in-valid") == false);
   assert(invalid_check("word5") == false);
   assert(invalid_check("word!") == false);

   assert(char2int('a') == 0);
   assert(char2int('z') == 25);
   assert(char2int('\'') == APOS);
   assert(char2int('1') == INVALID);

   dict* d1 = dict_init();
   char str1[] = "twice";
   assert(add_word(d1, str1) == true);
   assert(dict_mostcommon(d1) == 1);
   assert(add_word(d1, str1) == false);
   assert(dict_mostcommon(d1) == 2);
   assert(dict_wordcount(d1) == 2);
   dict* d2 = dict_spell(d1, "twice");
   assert(d2);
   char str2[] = "once";
   assert(add_word(d1, str2) == true);
   dict* d3 = dict_spell(d1, "once");
   assert(d3);

   assert(hash_function("hello") < PRIME_SZ);
   assert(hash_function("computerscience") < PRIME_SZ);

   dict* d4 = dict_init();
   add_newword(d4, "hash");
   assert(dict_spell(d4, "hash"));
   assert(isin_dict(d4, "hashing") == false);
   add_newword(d4, "hashing");
   assert(dict_spell(d4, "hashing"));
   assert(isin_dict(d4, "hashing"));
   add_repeat(d4, "hash");
   assert(dict_wordcount(d4) == 3);
   add_repeat(d4, "hashing");
   assert(dict_wordcount(d4) == 4);
   assert(isin_dict(d4, "hash"));
   assert(isin_dict(d4, "notindict") == false);

   char str3[MAXSTR];
   str_lower(str3, "STRING");
   assert(strcmp(str3, "string") == 0);
   str_lower(str3, "UgLyStRiNg");
   assert(strcmp(str3, "uglystring") == 0);

   word w;
   w.freq = 10;
   w.next = NULL;
   int cnt = 0;
   count_word(&w, &cnt);
   assert(cnt == 10);
   search_dict(&w, &cnt, WORD_CNT);
   assert(cnt == 20);
   w.freq = 0;
   count_word(&w, &cnt);
   assert(cnt == 20);
   search_dict(&w, &cnt, WORD_CNT);
   assert(cnt == 20);

   int cnt_freq = 0;
   count_freq(&w, &cnt_freq);
   assert(cnt_freq == 0);
   search_dict(&w, &cnt_freq, MST_CMN);
   assert(cnt_freq == 0);
   w.freq = 50;
   count_freq(&w, &cnt_freq);
   assert(cnt_freq == 50);
   search_dict(&w, &cnt_freq, MST_CMN);
   assert(cnt_freq == 50);

   word* w1 = (word*)ncalloc(1, sizeof(word));
   add_chain(w1, "newword");
   assert(isin_chain(w1, "newword"));
   free_chain(w1);
   word* w2 = (word*)ncalloc(1, sizeof(word));
   add_chain(w2, "anotherword");
   add_chain(w2, "andanotherword");
   add_chain(w2, "howaboutanother");
   assert(isin_chain(w2, "anotherword"));
   assert(isin_chain(w2, "andanotherword"));
   free_chain(w2);

   dict_free(&d4);
   assert(d4 == NULL);
   dict_free(&d1);
   assert(d1 == NULL);
   dict_free(&d);
   assert(d == NULL);
}
