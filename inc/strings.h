#ifndef STRINGS_H
# define STRINGS_H

typedef struct s_string
{
    char    *data;
    size_t  length;
    size_t  capacity;
}           t_string;

t_string* string_create(const char* initial);
t_string* string_create_empty(size_t initial_capacity);
void string_append(t_string* str, const char* text);
void string_append_char(t_string* str, char c);
char* string_cstr(const t_string* str);  // Get null-terminated C string
void string_destroy(t_string* str);
#endif