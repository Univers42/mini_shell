#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stddef.h>

/**
 * Dynamic array for tokens - Optimal for lexing and parsing
 */
typedef struct s_token_array {
    struct s_token*	data;   // Array of tokens
    size_t			count;           // Number of tokens
    size_t 			apacity;        // Allocated capacity
} 					t_token_array;

/**
 * Token Types - Extended for shell-specific tokens
 */
typedef enum e_token_type {
    // Single-character tokens
    TOKEN_LEFT_PAREN,       // (
    TOKEN_RIGHT_PAREN,      // )
    TOKEN_LEFT_BRACE,       // {
    TOKEN_RIGHT_BRACE,      // }
    TOKEN_COMMA,            // ,
    TOKEN_DOT,              // .
    TOKEN_MINUS,            // -
    TOKEN_PLUS,             // +
    TOKEN_SEMICOLON,        // ;
    TOKEN_SLASH,            // /
    TOKEN_STAR,             // *
    TOKEN_PIPE,             // | (shell pipe)
    TOKEN_AMPERSAND,        // & (background)
    
    // One or two character tokens
    TOKEN_BANG,             // !
    TOKEN_BANG_EQUAL,       // !=
    TOKEN_EQUAL,            // =
    TOKEN_EQUAL_EQUAL,      // ==
    TOKEN_GREATER,          // >
    TOKEN_GREATER_EQUAL,    // >=
    TOKEN_GREATER_GREATER,  // >> (append redirect)
    TOKEN_LESS,             // <
    TOKEN_LESS_EQUAL,       // <=
    TOKEN_LESS_LESS,        // << (here document)
    TOKEN_AND_AND,          // && (logical and)
    TOKEN_OR_OR,            // || (logical or)
    
    // Literals
    TOKEN_IDENTIFIER,       // variable names, command names
    TOKEN_STRING,           // "quoted strings"
    TOKEN_NUMBER,           // numeric literals
    TOKEN_WORD,             // unquoted words/arguments
    
    // Keywords (programming language keywords)
    TOKEN_AND,
    TOKEN_CLASS,
    TOKEN_ELSE,
    TOKEN_FALSE,
    TOKEN_FUN,
    TOKEN_FOR,
    TOKEN_IF,
    TOKEN_NIL,
    TOKEN_OR,
    TOKEN_PRINT,
    TOKEN_RETURN,
    TOKEN_SUPER,
    TOKEN_THIS,
    TOKEN_TRUE,
    TOKEN_VAR,
    TOKEN_WHILE,
    
    // Shell built-ins (for future extension)
    TOKEN_CD,
    TOKEN_ECHO,
    TOKEN_EXIT,
    TOKEN_EXPORT,
    TOKEN_PWD,
    TOKEN_ENV,
    
    // Special tokens
    TOKEN_NEWLINE,          // End of command
    TOKEN_EOF,              // End of file
    TOKEN_ERROR             // Lexical error
} t_token_type;

/**
 * Operator precedence for parsing
 */
typedef enum e_precedence {
    PREC_NONE,
    PREC_ASSIGNMENT,        // =
    PREC_OR,               // or, ||
    PREC_AND,              // and, &&
    PREC_EQUALITY,         // == !=
    PREC_COMPARISON,       // < > <= >=
    PREC_TERM,             // + -
    PREC_FACTOR,           // * /
    PREC_UNARY,            // ! -
    PREC_CALL,             // . ()
    PREC_PRIMARY
} t_precedence;

/**
 * Literal value types (for token values)
 */
typedef enum e_literal_type {
    LITERAL_NONE,
    LITERAL_STRING,
    LITERAL_NUMBER,
    LITERAL_BOOLEAN
} t_literal_type;

typedef struct s_literal {
    t_literal_type type;
    union {
        char* string;
        double number;
        bool boolean;
    } as;
} t_literal;

/**
 * Source location information
 */
typedef struct s_location {
    int line;
    int column;
    int offset;             // Absolute position in source
} t_location;

/**
 * Token structure
 */
typedef struct s_token {
    t_token_type type;      // Token type
    const char* start;      // Pointer to start in source
    int length;             // Length of token
    t_literal literal;      // Literal value (if any)
    t_location location;    // Source location
} t_token;

/**
 * Scanner/Lexer state
 */
typedef struct s_scanner {
    const char*	source;     // Source code
    const char* start;      // Start of current token
    const char* current;    // Current character
    int			line;       // Current line
    int			column;     // Current column
    t_token_array* tokens;  // Token array
    bool had_error;        // Error flag
} t_scanner;

/**
 * Dynamic array operations for tokens
 */

// Array creation and destruction
t_token_array* token_array_create(void);
t_token_array* token_array_create_with_capacity(size_t initial_capacity);
void token_array_destroy(t_token_array* array);

// Array operations
void token_array_push(t_token_array* array, t_token token);
t_token* token_array_get(t_token_array* array, size_t index);
t_token* token_array_last(t_token_array* array);
size_t token_array_size(t_token_array* array);
bool token_array_is_empty(t_token_array* array);
void token_array_clear(t_token_array* array);

// Array utilities
t_token* token_array_data(t_token_array* array);  // Get raw array
void token_array_reserve(t_token_array* array, size_t capacity);
void token_array_shrink_to_fit(t_token_array* array);

/**
 * Token array iterator for easy traversal
 */
typedef struct s_token_iterator {
    t_token_array* array;
    size_t current_index;
} t_token_iterator;

// Iterator operations
t_token_iterator token_iterator_create(t_token_array* array);
bool token_iterator_has_next(t_token_iterator* iter);
t_token* token_iterator_next(t_token_iterator* iter);
t_token* token_iterator_current(t_token_iterator* iter);
t_token* token_iterator_peek(t_token_iterator* iter);
t_token* token_iterator_peek_ahead(t_token_iterator* iter, size_t offset);
void token_iterator_reset(t_token_iterator* iter);
size_t token_iterator_position(t_token_iterator* iter);
void token_iterator_set_position(t_token_iterator* iter, size_t position);

// Scanner initialization and cleanup
void init_scanner(const char* source);
void cleanup_scanner(void);

// Single token scanning
t_token scan_token(void);
t_token make_token(t_token_type type);
t_token make_error_token(const char* message);

// Token array scanning (scan all at once)
t_token_array* scan_tokens(const char* source);

// Token operations with variadic support
void add_token(t_token_type type, ...);  // Supports both add_token(type) and add_token(type, literal)

// Utility functions
bool is_at_end(void);
char advance(void);
char peek(void);
char peek_next(void);
bool match(char expected);
void skip_whitespace(void);
bool is_digit(char c);
bool is_alpha(char c);
bool is_alnum(char c);

// Token type helpers
const char* token_type_name(t_token_type type);
bool is_keyword(const char* text, int length);
t_token_type keyword_type(const char* text, int length);

// String and number parsing
t_token string_token(void);
t_token number_token(void);
t_token identifier_token(void);

// Shell-specific tokenization (for future)
char** lex_line(const char* line);     // Simple word splitting
int count_tokens(char** tokens);
void free_tokens(char** tokens);

// Error handling
void lexer_error(const char* message);
void report_error(int line, int column, const char* message);

// Memory management helpers for literals
t_literal make_string_literal(const char* value);
t_literal make_number_literal(double value);
t_literal make_bool_literal(bool value);
t_literal make_nil_literal(void);
void free_literal(t_literal* literal);

// Debug functions
void print_token(t_token token);
void print_token_array(t_token_array* tokens);
void print_token_range(t_token_array* tokens, size_t start, size_t end);

/**
 * Helper functions for token creation
 */
t_location token_location(const t_scanner* scanner);
t_token current_token(const t_scanner* scanner, t_token_type type);
t_token current_token_with_literal(const t_scanner* scanner, t_token_type type, t_literal literal);

/**
 * Convenience functions for accessing tokens by index
 */
static inline t_token* get_token_at(t_token_array* tokens, size_t index) {
    return token_array_get(tokens, index);
}

static inline t_token* peek_token_at(t_token_array* tokens, size_t current, size_t lookahead) {
    size_t target = current + lookahead;
    return (target < token_array_size(tokens)) ? token_array_get(tokens, target) : NULL;
}

static inline bool has_more_tokens(t_token_array* tokens, size_t current) {
    return current < token_array_size(tokens);
}

/**
 * Token array slicing (for advanced parsing)
 */
t_token_array* token_array_slice(t_token_array* source, size_t start, size_t length);
t_token_array* token_array_copy(t_token_array* source);

#endif