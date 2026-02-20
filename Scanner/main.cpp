#include <iostream>
#include <string>
#include <fstream> 
#include <FlexLexer.h>
#include "token.h"

extern std::string yylval;

int main(){
    int token;
    std::ifstream file("Scanner/input.c");
    if(!file.is_open()){
        std::cerr<<"Unable to open file";
        return 1;
    }

    yyFlexLexer lexer(&file);
    

    while ((token = lexer.yylex()) != 0){
        switch (token)
        {
        case static_cast<int>(Token::KEYWORD):
            std::cout << "Keyword: ";
            break;
        case static_cast<int>(Token::IDENTIFIERS):
            std::cout << "identifier: ";
            break;
        case static_cast<int>(Token::OPERATORS):
            std::cout << "operator: ";
            break;
        case static_cast<int>(Token::FLOAT_LITERAL):
            std::cout << "float_literal: ";
            break;
        case static_cast<int>(Token::INT_LITERAL):
            std::cout << "int_literal: ";
            break;
        case static_cast<int>(Token::CHAR_LITERAL):
            std::cout << "char_literal: ";
            break;
        case static_cast<int>(Token::STRING_LITERAL):
            std::cout << "string_literal: ";
            break;
        case static_cast<int>(Token::BOOLEANS):
            std::cout << "boolean: ";
            break;
        case static_cast<int>(Token::TOKEN_ERROR):
            std::cout << "Unknown token: ";
            break;
        }
        std::cout<< yylval << "\n";
    };
    
    file.close();
    return 0;
}


