#include "t27.h"

// 'Internal' function prototypes 
// ...
#define INVALID 99
#define APOS 26
#define MAXSTR 50

struct dictcmp {
   unsigned count;
   unsigned fin;
   bool found;
   dict* prev;
};
typedef struct dictcmp dictcmp;

struct autocmp {
   int max;
   dict* target;
   dict* start;
};
typedef struct autocmp autocmp;

struct string {
   char curr_str[MAXSTR];
   char fin_str[MAXSTR];
   int index;
   bool found;
};
typedef struct string string;

enum search_type {WORD_CNT, NODE_CNT, MST_CMN};
typedef enum search_type search_type;

bool invalid_check(const char* str);
int char2int(char c);
char int2char(int i);
bool add_word(dict* p, const char* str);
bool repeat_check(dict* root, dict* curr);
void recur_search(const dict* p, int* count, search_type type);
void search_type_flow(const dict* p, int* count, search_type type);
void search_dwn(dict* curr, dict* p2, dictcmp* data);
void recur_free(dict* p);
dict* autocomp_algo(dict* curr);
void recur_autocmp(dict* curr, autocmp* data);
void set_max_target(dict* curr, autocmp* data);
char* return_str(const dict* start, const dict* target);
void build_str(const dict* start, const dict* target, string* data);
dict* find_dict(const dict* p, const char* str);
bool search_condition(dict* p, dictcmp* data);
bool target_found(const dict* start, const dict* target, string* data);
bool node_found(dict* curr, dict* p2, dictcmp* data);
void remove_hyphen(const char* wd, char* new);

// from general.c
void on_error(const char* s);
void* ncalloc(int n, size_t size);

dict* dict_init(void)
{
   dict* p = (dict*)ncalloc(1, sizeof(dict));
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

void remove_hyphen(const char* wd, char* new)
{
   int i = 0;
   int j = 0;
   while (wd[i] != '\0'){
      if (wd[i] != '-'){
         new[j] = wd[i];
         j++;
      }
      i++;
   }
   new[j] = '\0';
}

void dict_free(dict** d)
{
   recur_free(*d);
   *d = NULL;
}


int dict_wordcount(const dict* p)
{
   if (p == NULL){
      return 0;
   }
   int count = 0;
   recur_search(p, &count, WORD_CNT);
   return count;
}

int dict_nodecount(const dict* p)
{
   if (p == NULL){
      return 0;
   }
   int count = 0;
   recur_search(p, &count, NODE_CNT);
   return count;
}

dict* dict_spell(const dict* p, const char* str)
{
   if (p == NULL || str == NULL){
      return NULL;
   }
   if (invalid_check(str) == false){
      return NULL;
   }
   dict* target = find_dict(p, str);
   if (target == NULL || target->terminal == false){
      return NULL;
   }
   return (dict*)target;
}

int dict_mostcommon(const dict* p)
{
   int count = 0;
   recur_search(p, &count, MST_CMN);
   return count;
}

// CHALLENGE1
unsigned dict_cmp(dict* p1, dict* p2)
{
   dict* curr = p1;
   dictcmp* data = (dictcmp*)ncalloc(1, sizeof(dictcmp));
   while (curr != NULL){
      search_dwn(curr, p2, data);
      if (data->found == true){
         unsigned nodes = data->fin;
         free(data);
         return nodes;
      }
      data->prev = curr;
      curr = curr->up;
      data->count++;
   }
   // only get here if p1 & p2 are not in the same tree
   unsigned nodes = data->fin;
   free(data);
   return nodes; 
}

// CHALLENGE2
void dict_autocomplete(const dict* p, const char* wd, char* ret)
{
   if (p == NULL || wd == NULL || ret == NULL){
      return;
   }
   dict* curr = find_dict(p, wd);
   dict* start = curr;
   dict* target = autocomp_algo(curr);
   char* str = return_str(start, target);
   strcpy(ret, str);
   free(str);
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

char int2char(int i)
{
   if (i == APOS){
      return '\'';
   }
   return i + 'a';
}

bool add_word(dict* p, const char* str)
{
   int i = 0;
   dict* curr = p;
   while (str[i] != '\0'){
      int j = char2int(str[i]);
      if (curr->dwn[j]){
         curr = curr->dwn[j];
      }
      else { 
         dict* next = (dict*)ncalloc(1, sizeof(dict));
         curr->dwn[j] = next;
         next->up = curr;
         curr = next;
      }
      i++;
   }
   return repeat_check(p, curr);
}

bool repeat_check(dict* root, dict* curr)
{
   bool check = true;
   if (curr->terminal == true){
      check = false;
   }
   curr->terminal = true;
   curr->freq++;
   curr = root;
   return check;
}

void recur_search(const dict* p, int* count, search_type type)
{
   for (int i = 0; i < ALPHA; i++){
      if (p->dwn[i] != NULL){
         recur_search(p->dwn[i], count, type);
      }
   }
   search_type_flow(p, count, type);
}

void search_type_flow(const dict* p, int* count, search_type type)
{
   if (type == WORD_CNT){
      (*count) += p->freq;
   }
   else if (type == NODE_CNT){
      (*count)++;
   }
   else if (type == MST_CMN) {
      if (p->freq > (*count)){
         (*count) = p->freq;
      }
   }
}

void search_dwn(dict* curr, dict* p2, dictcmp* data)
{
   if (node_found(curr, p2, data)){
      return;
   }
   data->count++;
   for (int i = 0; i < ALPHA; i++){
      if (search_condition(curr->dwn[i], data) == true){
         search_dwn(curr->dwn[i], p2, data);
      }
   }
   data->count--;
}

bool node_found(dict* curr, dict* p2, dictcmp* data)
{
   if (curr == p2){
      (data->fin) = (data->count);
      (data->found) = true;
      return true;
   }
   return false;
}

bool search_condition(dict* p, dictcmp* data)
{
   if (p == NULL){
      return false;
   }
   if (data->found == true){
      return false;
   }
   if (p == data->prev){
      return false;
   }
   return true;
}

dict* autocomp_algo(dict* curr)
{
   autocmp* data = (autocmp*)ncalloc(1, sizeof(autocmp));
   data->start = curr;
   recur_autocmp(curr, data);
   dict* target = data->target;
   free(data);
   return target;
}

void recur_autocmp(dict* curr, autocmp* data)
{
   if (curr == NULL || data == NULL){
      return;
   }
   for (int i = 0; i < ALPHA; i++){
      if (curr->dwn[i] != NULL){
         recur_autocmp(curr->dwn[i], data);
      }
   }
   if (curr == data->start){
      return;
   }
   set_max_target(curr, data);
}

void set_max_target(dict* curr, autocmp* data)
{
   if (curr->freq > data->max){
      data->max = curr->freq;
      data->target = curr;
   }
}

char* return_str(const dict* start, const dict* target)
{
   string* data = (string*)ncalloc(1, sizeof(string));
   build_str(start, target, data);
   char* str = (char*)ncalloc(MAXSTR, sizeof(char));
   strcpy(str, data->fin_str);
   free(data);
   return str;
}

void build_str(const dict* start, const dict* target, string* data)
{
   if (target_found(start, target, data) == true){
      return;
   }
   for (int i = 0; i < ALPHA; i++){
      if (start->dwn[i] != NULL && data->found == false){
         data->curr_str[data->index] = int2char(i);
         data->index++;
         build_str(start->dwn[i], target, data);
         data->index--;
      }
   }
}

bool target_found(const dict* start, const dict* target, string* data)
{
   if (start == target){
      data->found = true;
      data->curr_str[data->index] = '\0';
      strcpy(data->fin_str, data->curr_str);
      return true;
   }
   return false;
}

dict* find_dict(const dict* p, const char* str)
{
   int i = 0;
   while (str[i] != '\0'){
      int j = char2int(str[i]);
      if (p->dwn[j] == NULL){
         return NULL;
      }
      p = p->dwn[j];
      i++;
   }
   return (dict*)p;
}

void recur_free(dict* p)
{
   for (int i = 0; i < ALPHA; i++){
      if (p->dwn[i] != NULL){
         recur_free(p->dwn[i]);
      }
   }
   free(p);
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
   if(v==NULL){
      on_error("Cannot ncalloc() space");
   }
   return v;
}

void test(void)
{
   dict* d = dict_init();
   assert(d);

   assert(dict_nodecount(d) == 1);

   assert(dict_addword(d, "hello") == true);
   assert(dict_addword(d, "woRld") == true);
   assert(dict_addword(d, "t-twentyseven") == true);
   assert(dict_addword(d, "their's") == true);
   assert(dict_addword(d, "hAsh") == true);
   assert(dict_addword(d, "(false)") == false);
   assert(dict_addword(d, "tree") == true);
   assert(dict_addword(d, "tree") == false);

   assert(dict_wordcount(d) == 7); 
   assert(dict_nodecount(d) > 1);

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
   assert(int2char(0) == 'a');
   assert(int2char(25) == 'z');
   assert(int2char(APOS) == '\'');

   dict* d1 = dict_init();
   assert(add_word(d1, "twice") == true);
   assert(dict_mostcommon(d1) == 1);
   assert(add_word(d1, "twice") == false);
   assert(dict_mostcommon(d1) == 2);
   assert(dict_wordcount(d1) == 2);
   dict* d2 = dict_spell(d1, "twice");
   assert(d2 != NULL);
   assert(d2->freq == 2);
   assert(add_word(d1, "once") == true);
   dict* d3 = dict_spell(d1, "once");
   assert(d3 != NULL);
   assert(d3->freq == 1);

   int wc = 0;
   recur_search(d, &wc, WORD_CNT);
   assert(wc == dict_wordcount(d));
   int nc = 0;
   recur_search(d, &nc, NODE_CNT);
   assert(nc == dict_nodecount(d));
   int mc = 0;
   recur_search(d, &mc, MST_CMN);
   assert(mc == dict_mostcommon(d));
   
   int count = 0;
   search_type_flow(d, &count, WORD_CNT);
   assert(count == 0);
   search_type_flow(d, &count, NODE_CNT);
   assert(count == 1);
   search_type_flow(d, &count, MST_CMN);
   assert(count == 1);

   dict* target1 = dict_spell(d, "hello");
   dict* target2 = dict_spell(d, "hello");
   assert(find_dict(d, "hash") != NULL);
   assert(find_dict(d, "has") != NULL);
   assert(dict_spell(d, "has") == NULL);

   string* str_data1 = (string*)ncalloc(1, sizeof(string));
   assert(target_found(target1, target2, str_data1) == true);
   free(str_data1);

   str_data1 = ncalloc(1, sizeof(string));
   target1 = dict_spell(d, "world");
   assert(target_found(target1, target2, str_data1) == false);
   free(str_data1);

   dictcmp* data = (dictcmp*)ncalloc(1, sizeof(dictcmp));
   dict* world = dict_spell(d, "world");
   dict* nonexistent = dict_spell(d, "nonexistent");
   assert(node_found(d, world, data) == false);  
   assert(node_found(world, world, data) == true);  
   assert(node_found(world, nonexistent, data) == false); 
   free(data);

   dict* prev = dict_spell(d, "hash");
   data = (dictcmp*)ncalloc(1, sizeof(dictcmp));
   data->prev = prev;
   assert(search_condition(world, data) == true); 
   assert(search_condition(NULL, data) == false);    
   assert(search_condition(prev, data) == false); 
   free(data);

   dict* start = dict_spell(d, "hello");
   dict* target = dict_spell(d, "hello");
   string* str_data = (string*)ncalloc(1, sizeof(string));
   build_str(start, target, str_data);
   assert(strcmp(str_data->fin_str, "") == 0);
   free(str_data);
   char* ret = return_str(d, target);
   assert(strcmp(ret, "hello") == 0);
   free(ret);
   dict* auto_dict = autocomp_algo(dict_spell(d, "t"));
   char* ret1 = return_str(auto_dict, auto_dict);
   assert(strcmp(ret1, "") == 0);
   free(ret1);

   autocmp* auto_data = (autocmp*)ncalloc(1, sizeof(autocmp));
   auto_data->start = find_dict(d, "hel");
   dict* s = auto_data->start;
   recur_autocmp(s, auto_data);
   assert(auto_data->target != NULL && auto_data->target->terminal == true);
   free(auto_data);

   dictcmp* search_data = (dictcmp*)ncalloc(1, sizeof(dictcmp));
   search_dwn(find_dict(d, "hel"), dict_spell(d, "hello"), search_data);
   assert(search_data->found == true);
   free(search_data);

   dictcmp* data3 = (dictcmp*)ncalloc(1,sizeof(dictcmp));
   assert(node_found(dict_spell(d, "hello"), dict_spell(d, "world"), data3) == false);
   assert(node_found(dict_spell(d, "hello"), dict_spell(d, "hello"), data3) == true);
   free(data3);

   dict_free(&d1);
   assert(d1 == NULL);
   dict_free(&d);
   assert(d == NULL);
}
