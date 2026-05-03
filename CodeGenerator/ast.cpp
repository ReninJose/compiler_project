#include <iostream>
#include <string>
#include "ast.h"

using namespace std;

// Types

struct type* type_create(type_t kind, struct type* subtype, struct param_list* params){
    struct type *t = (type*) malloc(sizeof(*t));
    t->kind = kind;
    t->subtype = subtype;
    t->params = params;
    return t;
}

void type_print(struct type *t)
{
    if(!t) return;

    switch(t->kind) {
        case DECL_VOID:     cout<<"void "; break;
        case DECL_BOOL:     cout<<"bool "; break;
        case DECL_CHAR:     cout<<"char "; break;
        case DECL_INT:      cout<<"int" ; break;
        case DECL_FLOAT:    cout<<"float "; break;
        case DECL_FUNCTION:
            cout<<"function ";
            type_print(t->subtype); // return type
            if(t->params) {
	            cout<<" (";
	            param_list_print(t->params);
	            cout<<") ";
	        }
            break;
    }
}

// Param list

struct param_list* param_list_create(char *name, struct type *type, struct param_list *next)
{
    struct param_list *p = (param_list*) malloc(sizeof(*p));
    p->name = name;
    p->type = type;
    p->next = next;
    p->symbol = NULL;
    return p;
}

void param_list_print(struct param_list *p)
{
    if(!p) return;

    cout<<p->name<<" ";
    type_print(p->type);

    if(p->next) {
        cout<<", ";
        param_list_print(p->next);
    }
}

// Expressions

struct expr* expr_create(expr_t kind, struct expr *left, struct expr *right){
    struct expr *buffer = (struct expr*) malloc(sizeof(struct expr));
    buffer->kind = kind;
    buffer->left = left;
    buffer->right = right;
    buffer->int_value = 0;
    buffer->float_value = 0.0;
    buffer->bool_value = false;
    buffer->name = NULL;
    buffer->symbol = NULL;
    return buffer;
}

struct expr* expr_create_int_literal(int v){
    struct expr *buffer = expr_create(EXPR_INT_LITERAL,0,0);
    buffer->int_value=v;
    return buffer;
}

struct expr* expr_create_float_literal(float v){
    struct expr *buffer = expr_create(EXPR_FLOAT_LITERAL,0,0);
    buffer->float_value=v;
    return buffer;
}

struct expr* expr_create_char_literal(char c){
    struct expr *buffer = expr_create(EXPR_CHAR_LITERAL,0,0);
    buffer->char_value=c;
    return buffer;
}

struct expr* expr_create_bool_literal(bool b){
    struct expr *buffer = expr_create(EXPR_BOOL_LITERAL,0,0);
    buffer->bool_value=b;
    return buffer;
}

struct expr* expr_create_name(char* name){
    struct expr *buffer = expr_create(EXPR_NAME,0,0);
    buffer->name=name;
    return buffer;
}

void expr_print(struct expr *e){
	if(!e) return;
		cout<<"(";

	expr_print(e->left);    
	switch(e->kind) {
		case EXPR_INT_LITERAL: 	    cout<<e->int_value; break;
        case EXPR_FLOAT_LITERAL:    cout<<e->float_value; break;
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

// Declaration

struct decl* decl_create(char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next){
	struct decl *d = (struct decl*)malloc(sizeof(*d));
	d->name = name;
	d->type = type;
	d->value = value;
	d->code = code;
	d->next = next;
	d->symbol = NULL;
	return d;
}

void decl_print(struct decl *e){
	if(!e) return;
		
	cout<<"var ["; 
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
	}

	decl_print(e->next);
}


// Statement
struct stmt* stmt_create(stmt_t kind, decl *decl, expr *init_expr, expr *expr_b, expr *next_expr, stmt *body, stmt *else_body, stmt *next){
    struct stmt *s = (struct stmt*)malloc(sizeof(*s));
    s->kind = kind;
    s->decl = decl;
    s->init_expr = init_expr;
    s->expr = expr_b;
    s->next_expr = next_expr;
    s->body = body;
    s->else_body = else_body;
    s->next = next;
    return s;
}

void stmt_print(struct stmt *s)
{
    if(!s) return;

    switch(s->kind) {

        case STMT_DECL:
            decl_print(s->decl);
            cout<<";\n";
            break;

        case STMT_EXPR:
            expr_print(s->expr);
            cout<<";\n";
            break;

        case STMT_RETURN:
            cout<<"return ";
            expr_print(s->expr);
            cout<<";\n";
            break;
        
        case STMT_FUNCTION:
            cout << s->decl->name << "(";
            if(s->decl->type->kind == DECL_FUNCTION && s->decl->type->params)
                param_list_print(s->decl->type->params);
            cout<<")\n{\n";
            stmt_print(s->body);
            cout<<"}";
            break;

        case STMT_IF_ELSE:
            cout<<"if (";
            expr_print(s->init_expr);
            cout<<")\n";
            stmt_print(s->body);

            if(s->else_body) {
                cout<<"else\n";
                stmt_print(s->else_body);
            }
            break;
    }

    stmt_print(s->next);
}


// Note: I used the help of AI to aid me draw the ast tree properly. The code below was written with the help of AI.

static void expr_tree(struct expr *e, const string &pfx, bool last)
{
    if (!e) return;
    string branch   = pfx + (last ? "└── " : "├── ");
    string childPfx = pfx + (last ? "    " : "│   ");

    switch (e->kind) {
        case EXPR_INT_LITERAL:
            cout << branch << "INT(" << e->int_value << ")\n"; return;
        case EXPR_FLOAT_LITERAL:
            cout << branch << "FLOAT(" << e->float_value << ")\n"; return;
        case EXPR_CHAR_LITERAL:
            cout << branch << "CHAR('" << e->char_value << "')\n"; return;
        case EXPR_BOOL_LITERAL:
            cout << branch << "BOOL(" << (e->bool_value ? "true" : "false") << ")\n"; return;
        case EXPR_NAME:
            cout << branch << "NAME(" << e->name << ")\n"; return;
        default: break;
    }

    const char *op = "OP";
    switch (e->kind) {
        case EXPR_ADD:    op = "ADD(+)";    break;
        case EXPR_SUB:    op = "SUB(-)";    break;
        case EXPR_MUL:    op = "MUL(*)";    break;
        case EXPR_DIV:    op = "DIV(/)";    break;
        case EXPR_AND:    op = "AND(&&)";   break;
        case EXPR_OR:     op = "OR(||)";    break;
        case EXPR_EQEQ:   op = "EQEQ(==)";  break;
        case EXPR_NEQ:    op = "NEQ(!=)";   break;
        case EXPR_LT:     op = "LT(<)";     break;
        case EXPR_GT:     op = "GT(>)";     break;
        case EXPR_LTE:    op = "LTE(<=)";   break;
        case EXPR_GTE:    op = "GTE(>=)";   break;
        case EXPR_NOT:    op = "NOT(!)";    break;
        case EXPR_ASSIGN: op = "ASSIGN(=)"; break;
        default: break;
    }
    expr_tree(e->left,  childPfx, e->right == nullptr);
    expr_tree(e->right, childPfx, true);
    cout << branch << op << "\n";
}

static void stmt_tree(struct stmt *s, const string &pfx, bool last)
{
    if (!s) return;
    bool hasNext = s->next != nullptr;
    string branch = pfx + ((!hasNext && last) ? "└── " : "├── ");
    string childPfx = pfx + ((!hasNext && last) ? "    " : "│   ");

    switch (s->kind) {
        case STMT_DECL:
            cout << branch << "DECL: " << s->decl->name << " : ";
            type_print(s->decl->type);
            cout << "\n";
            if (s->decl->value) {
                cout << childPfx << "└── VALUE:\n";
                expr_tree(s->decl->value, childPfx + "    ", true);
            }
            break;

        case STMT_FUNCTION:
            cout << branch << "FUNCTION: " << s->decl->name << "(";
            param_list_print(s->decl->type->params);
            cout << ") -> ";
            type_print(s->decl->type->subtype);
            cout << "\n";
            if (s->body) {
                cout << childPfx << "└── BODY:\n";
                stmt_tree(s->body, childPfx + "    ", true);
            }
            break;

        case STMT_EXPR:
            cout << branch << "EXPR:\n";
            expr_tree(s->expr, childPfx, true);
            break;

        case STMT_RETURN:
            cout << branch << "RETURN:\n";
            expr_tree(s->expr, childPfx, true);
            break;

        case STMT_IF_ELSE:
            cout << branch << "IF:\n";
            cout << childPfx << "├── COND:\n";
            expr_tree(s->init_expr, childPfx + "│   ", true);
            cout << childPfx << (s->else_body ? "├── " : "└── ") << "THEN:\n";
            stmt_tree(s->body, childPfx + (s->else_body ? "│   " : "    "), true);
            if (s->else_body) {
                cout << childPfx << "└── ELSE:\n";
                stmt_tree(s->else_body, childPfx + "    ", true);
            }
            break;
    }

    if (hasNext)
        stmt_tree(s->next, pfx, last);
}

void ast_print_tree(struct stmt *root)
{
    cout << "\n=== AST ===\n";
    cout << "PROGRAM\n";
    stmt_tree(root, "", true);
    cout << "===========\n";
}