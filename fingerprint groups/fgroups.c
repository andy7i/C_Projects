// Andrew Jang and Eli Kohlenberg
// Comp40, Assignment 1, Part B
// 

#include <stdio.h>
#include <table.h>
#include <list.h>
#include <atom.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <mem.h>
#include <str.h>
#include <assert.h>

char* get_fingerprint();
char* get_name();
void add_to_table (Table_T fp_table, char *new_fp, char *new_name);
void get_data (Table_T fp_table);
void print_data (Table_T fp_table);
static void print_group(const void *key, void **value, void *cl);
static void print_n_free(void **x, void *cl);
static void just_free(void **value, void *cl);
//void free_atom(char* atom);


const int SIZE_HINT = 100000;       //since we've been notified
const int goodToGo = 1;         // sentinel for  when to start making newlines
int makeNewlines = 0;           // the check variable to start making newlines
int group_count = 0;


int main(){
    Table_T fp_table = Table_new(SIZE_HINT, NULL, NULL);
    get_data(fp_table);
    print_data(fp_table);
    Table_free(&fp_table);
    return 0;
}

void get_data (Table_T fp_table){
    char c = fgetc(stdin);
    while (c != EOF){
        if (c == '\n'){
            c = fgetc(stdin);
            continue;
        }
        char new_fp[513];
        char *new_name = NULL;
        new_name = malloc(512);
        int i;
        for (i = 0; i < 512; i++){
            new_fp[i] = '\0';
            new_name[i] = '\0';
        }
        new_fp[512] = '\0';
        new_fp[0] = c;
        i = 1;
        c = fgetc(stdin);       // can we just use *new_input instead?
        while (!isspace(c) && i < 512){
            new_fp[i] = c;
            c = fgetc(stdin);
            i++;
        }
        if (c == '\n' || c == EOF)
        {
            c = fgetc(stdin);
            continue;
        }
        else if (!isspace(c)){
            fprintf(stderr, "Fingerprint too big.");
            while (!isspace(c)){
                c = fgetc(stdin);
            }
        }
        c = fgetc(stdin);
        while (isspace(c)){
            c = fgetc(stdin);
        }
        i = 0;
        while (c != '\n' && i < 512){
            new_name[i] = c;
            c = fgetc(stdin);
            i++;
        }
        if (new_name[0] == '\0'){
            FREE(new_name);
            continue;}
        add_to_table (fp_table, new_fp, new_name);
        if (c == EOF){
            break;               // skips the next stdin line if EOF
        }
        c = fgetc(stdin);
    }
}

void add_to_table (Table_T fp_table, char *new_fp, char *new_name){
    //const char* fp_atom;
    //NEW0(fp_atom);
    //fp_atom = Atom_new(new_fp, strlen(new_fp));
    List_T table_value = Table_get(fp_table, Atom_new(new_fp, strlen(new_fp)));
    if (table_value == NULL){
        List_T fp_group = List_push (NULL, new_name);
        Table_put(fp_table, Atom_new(new_fp, strlen(new_fp)), fp_group);
        group_count++;
    }
    else {
        List_T new_list = List_push(table_value, new_name);
        Table_put(fp_table, Atom_new(new_fp, strlen(new_fp)), new_list);
    }
    //free(fp_atom);
}

void print_data(Table_T fp_table){
    void (*pt2print_group)(const void*, void**, void*);
    pt2print_group = print_group;
    Table_map(fp_table, pt2print_group, NULL);
}

static void print_group(const void *key, void **value, void *cl){
    assert(key);
    assert(cl == cl);
    void (*pt2print)(void**, void*);
    void (*pt2just_free)(void**, void*);
    pt2print = print_n_free;
    pt2just_free = just_free;
    group_count--;
    if (List_length(*value) > 1){
        if (group_count > 0 && makeNewlines == goodToGo){
            printf("\n");
        }
        List_map(*value, pt2print, NULL);
    }
    else {
        List_map(*value, pt2just_free, NULL);
    }
    List_free((List_T*)value);
}

static void print_n_free(void **value, void *cl){
    assert(cl == cl);
    if (value != NULL){
        printf("%s\n", (char*)*value);
        FREE(*value);
    }
    makeNewlines = goodToGo;
}

static void just_free(void **value, void *cl){
    assert(cl == cl);
    if (value != NULL){
        FREE(*value);
    }
}

/*void free_atom(char *atom){
    char* to_free = atom;
    FREE(to_free);
}
*/
