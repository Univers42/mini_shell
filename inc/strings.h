#ifndef STRINGS_H
# define STRINGS_H

typedef struct s_strings
{
	char	*data;
	size_t	length;
	size_t	capacity;
}			stringss;

t_stringss	*string_create(const char* initial);
t_stringss	*string_create_empty(size_t initial_capacity);
void		string_append(t_stringss* str, const char* text);
void		string_append_char(t_stringss* str, char c);
char		*string_cstr(const t_stringss* str);  // Get null-terminated C string
void		string_destroy(t_stringss* str);
#endif