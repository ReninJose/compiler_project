#ifndef EXPR_H
#define EXPR_H

//  type declaration
typedef enum{
    DECL_INT,
    DECL_FLOAT,
    DECL_CHAR,
    DECL_BOOL,
    DECL_VOID,
    DECL_FUNCTION
}type_t;

struct type {
	type_t kind;
	struct type* subtype;
	struct param_list* params;
};

struct param_list {
	char* name;
	struct type *type;
	struct param_list *next;
};

// Expresions
typedef enum{
    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_AND,
    EXPR_OR,
    EXPR_LTE,
    EXPR_GTE,
    EXPR_LT,
    EXPR_GT,
    EXPR_EQEQ,
    EXPR_NEQ,
    EXPR_NOT,
    EXPR_ASSIGN,
    EXPR_INT_LITERAL,
    EXPR_CHAR_LITERAL,
    EXPR_BOOL_LITERAL,
    EXPR_NAME
} expr_t;

typedef struct expr
{
    expr_t kind;
    struct expr *left;
    struct expr *right;
    char* name;
    int int_value;
    char char_value;
    char* string_value;
    bool bool_value;
}expr;

// Statement
typedef enum{
    STMT_DECL,
    STMT_EXPR,
    STMT_IF_ELSE,
    STMT_RETURN
}stmt_t;

struct stmt {
	stmt_t kind;
	struct decl *decl;
	struct expr *init_expr;
	struct expr *expr;
	struct expr *next_expr;
	struct stmt *body;
	struct stmt *else_body;
	struct stmt *next;
};

struct decl {
	char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct decl *next;
};

struct type* type_create(type_t kind, struct type* subtype, struct param_list* params);
struct param_list * param_list_create(char *name, struct type *type, struct param_list *next);
void param_list_print(struct param_list *p);
void type_print(struct type *t);
struct expr* expr_create(expr_t kind, struct expr *left, struct expr *right);
struct expr* expr_create_int_literal(int v);
struct expr* expr_create_char_literal(char c);
struct expr* expr_create_bool_literal(bool b);
struct expr* expr_create_name(char* name);
void expr_print(struct expr *e);
void stmt_print(struct stmt *s);
void decl_print(struct decl *e);

#endif