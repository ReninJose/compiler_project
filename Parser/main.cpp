// #include <stdio.h>
// #include "expr.h"

// extern int yyparse();
// extern struct expr *parser_result;

// int main() {
//     printf("Enter expression: ");
    
//     if (yyparse() == 0) {
//         printf("Parsed expression: ");
//         expr_print(parser_result);
//         printf("\n");
//     } else {
//         printf("Parse failed.\n");
//     }

//     return 0;
// }

// #include <iostream>
// #include <string>
// #include <fstream>
// #include "expr.h"

// extern int yyparse();
// extern struct expr *parser_result;
// extern FILE *yyin;

// using namespace std;

// int main() {

//     string testDirectory = "test/";

//     string testCases[1] = {
//         testDirectory + "test1.cpp"
//         // testDirectory + "test2.c",
//         // testDirectory + "test3.c",
//         // testDirectory + "test4.c",
//         // testDirectory + "test5.c"
//     };

//     for (int i = 0; i < 1; i++) {
//         yyin = fopen(testCases[i].c_str(), "r");

//         if (!yyin) {
//             cerr << "Unable to open file: " << testCases[i] << endl;
//             return 1;
//         }

//         // Print file contents for debugging
//         char buffer[256];
//         while (fgets(buffer, sizeof(buffer), yyin)) {
//             cout << buffer;
//         }

//         rewind(yyin);

//         // parser_result = nullptr;
//         int result = yyparse();

//         if (result == 0) {
//             cout << "Parse success: ";
//             expr_print(parser_result);
//             cout << endl;
//         } else {
//             cout << "Parse failed." << endl;
//         }

//         fclose(yyin);
//     }

//     return 0;
// }

#include <iostream>
#include <string>
#include <fstream>
#include "expr.h"

extern int yyparse();
extern struct expr *parser_result;
extern FILE *yyin;

using namespace std;

int main() {

    string testDirectory = "test/";

    string testCases[4] = {
        testDirectory + "test1.cpp",
        testDirectory + "test2.cpp",
        testDirectory + "test3.cpp",
        testDirectory + "test4.cpp"
    };

    for (int i = 0; i < 4; i++) {
        FILE *file = fopen(testCases[i].c_str(), "r");

        if (!file) {
            cerr << "Unable to open file: " << testCases[i] << endl;
            return 1;
        }

        cout << endl << "Opening file: " << testCases[i] << endl; 

        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file)) {
            if (buffer[0] == '\0' || buffer[0] == '\n') {
                continue;
            }

            string line(buffer);
            string trimmed = line.substr(line.find_first_not_of(" \t"));
            cout << "Parsing line: " << trimmed;

            // Create an in-memory FILE* from the line buffer
            yyin = fmemopen(buffer, strlen(buffer), "r");
            if (!yyin) {
                cerr << "fmemopen failed." << endl;
                fclose(file);
                return 1;
            }

            parser_result = nullptr;
            int result = yyparse();

            if (result == 0) {
                cout << "Parse success: ";
                expr_print(parser_result);
                cout << endl;
            } else {
                continue;
                // cout << "Parse failed." << endl;
            }

            fclose(yyin);
            yyin = nullptr;
        }

        fclose(file);
    }

    return 0;
}