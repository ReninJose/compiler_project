#include <iostream>
#include <string>
#include <fstream> 
#include <FlexLexer.h>
#include "token.h"

extern std::string yylval;

int main(){
    int token;
    std::string testDirectory = "Scanner/test/";
    std::string testCases[5] = {testDirectory+"test1.c", 
        testDirectory+"test2.c", 
        testDirectory+"test3.c",
        testDirectory+"test4.c",
        testDirectory+"test5.c"
    
    };

    for(int i = 0; i < 5; i++){
        std::ifstream file(testCases[i]);
        printf("Opening file %s\n", testCases[i].c_str());
        if(!file.is_open()){
            std::cerr<<"Unable to open file";
            return 1;
        }

        yyFlexLexer lexer(&file);

        while(true){
            int tok = lexer.yylex();
            if(tok == 0){
                break;
            }
            printf("%s : %d\n", lexer.YYText(), tok);
        }
        printf("\n");
    }

    return 0;
}

