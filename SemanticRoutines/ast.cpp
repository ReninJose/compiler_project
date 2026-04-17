#include <iostream>
#include "ast.h"

using namespace std;

struct type* type_create(type_t kind, struct type* subtype, struct param_list* params){
    struct type *t = (type*) malloc(sizeof(*t));
    t->kind = kind;
    t->subtype = subtype;
    t->params = params;
    return t;
}

struct param_list * param_list_create(char *name, struct type *type, struct param_list *next)
{
    struct param_list *p = (param_list*) malloc(sizeof(*p));
    p->name = name;
    p->type = type;
    p->next = next;
    return p;
}

void param_list_print(struct param_list *p)
{
    if(!p) return;

    cout<<p->name;
    type_print(p->type);

    if(p->next) {
        cout<<", ";
        param_list_print(p->next);
    }
}

void type_print(struct type *t)
{
    if(!t) return;

    switch(t->kind) {
        case DECL_VOID:     cout<<"void"; break;
        case DECL_BOOL:     cout<<"bool"; break;
        case DECL_CHAR:     cout<<"char"; break;
        case DECL_INT:      cout<<"int"; break;
        case DECL_FLOAT:    cout<<"float"; break;
        case DECL_FUNCTION:
            cout<<"function ";
            type_print(t->subtype); // return type
            if(t->params) {
	            cout<<" (";
	            param_list_print(t->params);
	            cout<<")";
	        }
            break;
    }
}

struct expr* expr_create(expr_t kind, struct expr *left, struct expr *right){
    struct expr *buffer = (struct expr*) malloc(sizeof(struct expr));
    buffer->kind = kind;
    buffer->left = left;
    buffer->right = right;
    buffer->int_value = 0;
    buffer->bool_value = NULL;
    buffer->name = NULL;
    return buffer;
}

struct expr* expr_create_int_literal(int v){
    struct expr *buffer = expr_create(EXPR_INT_LITERAL,0,0);
    buffer->int_value=v;
    return buffer;
}

struct expr* expr_create_char_literal(char c){
    struct expr *buffer = expr_create(EXPR_CHAR_LITERAL,0,0);
    buffer->char_value=c;
    return buffer;
}

struct expr* expr_create_bool_literal(bool b){
    struct expr *buffer = expr_create(EXPR_BOOL_LITERAL,0,0);
    buffer->int_value=1;
    return buffer;
}

struct expr* expr_create_name(char* name){
    struct expr *buffer = expr_create(EXPR_NAME,0,0);
    buffer->name=name;
    return buffer;
}

void expr_print( struct expr *e )
{
	if(!e) return;
		cout<<"(";
	
	expr_print(e->left);    
	switch(e->kind) {
		case EXPR_INT_LITERAL: 	    cout<<e->int_value; break;
        case EXPR_CHAR_LITERAL: 	cout<<e->char_value; break;
        case EXPR_BOOL_LITERAL: 	cout<<e->bool_value; break;
        case EXPR_NAME: 	        cout<<e->name; break;
        case EXPR_ADD:              cout<<"+"; break;
        case EXPR_SUB:              cout<<"-"; break;
        case EXPR_MUL:              cout<<"*"; break;
        case EXPR_DIV:              cout<<"/"; break;
        case EXPR_AND:              cout<<"&&"; break;
        case EXPR_OR:               cout<<"||"; break;
        case EXPR_EQEQ:             cout<<"=="; break;
        case EXPR_NEQ:              cout<<"!="; break;
        case EXPR_LTE:              cout<<"<="; break;
        case EXPR_GTE:              cout<<">="; break;
        case EXPR_LT:               cout<<"<"; break;
        case EXPR_GT:               cout<<">"; break;
        case EXPR_NOT:              cout<<"!"; break;
        case EXPR_ASSIGN:           cout<<"="; break;
	}
	expr_print(e->right);
	cout<<")";
}

struct decl * decl_create(char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next)
{
	struct decl *d = (struct decl*)malloc(sizeof(*d));
	d->name = name;
	d->type = type;
	d->value = value;
	d->code = code;
	d->next = next;
	return d;
}

void stmt_print(struct stmt *s)
{
    if(!s) return;

    switch(s->kind) {

        case STMT_DECL:
            decl_print(s->decl);
            break;

        case STMT_EXPR:
            expr_print(s->expr);
            printf(";\n");
            break;

        // case STMT_PRINT:
        //     printf("print ");
        //     expr_print(s->expr);
        //     printf(";\n");
        //     break;

        case STMT_RETURN:
            printf("return ");
            expr_print(s->expr);
            printf(";\n");
            break;

        case STMT_IF_ELSE:
            printf("if (");
            expr_print(s->expr);
            printf(")\n");
            stmt_print(s->body);

            if(s->else_body) {
                printf("else\n");
                stmt_print(s->else_body);
            }
            break;
    }

    stmt_print(s->next);
}

void decl_print(struct decl *e)
{
	if(!e) return;
		
	std::cout<<"var ["; 
	cout<<e->name; 
	cout<<"] : "; 

        type_print(e->type);

	if(e->value) {
	    cout<<" = expr "; 
	    expr_print(e->value);
	}	
	 
	if(e->code) {
		cout<<" code  ";
	    stmt_print(e->code);
	} else {
	        cout<<";\n";
	}
	decl_print(e->next);
}
