#include <iostream>
#include <fstream>
#include <string.h>
#include <stack>
using namespace std;

// Name:        Joshua Hull
//              Lab #2, InfixToPostfix

// Header Comment:
//
//

int ISP (char symbol) {

    int rtrn = -3;
    if(symbol == '@') rtrn = -2;
    if(symbol == '=') rtrn = -1;
    if(symbol == '^') rtrn = 3;
    if(symbol == '~') rtrn = 3;
    if(symbol == '*') rtrn = 2;
    if(symbol == '/') rtrn = 2;
    if(symbol == '+') rtrn = 1;
    if(symbol == '-') rtrn = 1;
    if(symbol == '(') rtrn = 0;
    return rtrn;
}

int ICP(char symbol) {

    int rtrn = -2;
    if(symbol == '=') rtrn = -1;
    if(symbol == '^') rtrn = 4;
    if(symbol == '~') rtrn = 4;
    if(symbol == '*') rtrn = 2;
    if(symbol == '/') rtrn = 2;
    if(symbol == '+') rtrn = 1;
    if(symbol == '-') rtrn = 1;
    if(symbol == '(') rtrn = 5;
    return rtrn;
}

// InfixToPostfix processes one line of input
void InfixToPostfix(string oneLine) {

   // declarations
   cout << oneLine << endl;

   // convert a string to a char *
   char * cstr;
   cstr = new char [oneLine.size()+1];
   strcpy (cstr, oneLine.c_str());

   // Tokenize, i.e., break up one line into its component tokens
   char * token;             // will hold one token from oneLine
   token = strtok(cstr, " \n");
   stack <char> operators;
   operators.push('@');
   bool lastOperator = false;


  while (token != NULL) {
        if(*token == '-' && lastOperator)
                *token = '~';
        if (*token == ';') {
            while (operators.top() != '@'){
                    cout << operators.top() << " ";
                    operators.pop();
            }
            operators.pop();          
            lastOperator = false;
        }
        else if (!(*token == '=' || *token == '^' || *token == '~' || *token == '*' || *token == '/' || *token == '+' || *token == '-' || *token == '('|| *token == ')')) {
        
        cout << token << " ";
        lastOperator = false;
        
        }
        else if(*token == ')') {
        
                while (operators.top() != '('){
                        cout << operators.top() << " ";
                        operators.pop();
                }
                operators.pop();
                lastOperator = false;    
        }

        else{
                while(ISP(operators.top()) >= ICP(*token)){
                        cout << operators.top() << " ";
                        operators.pop();
                }
                operators.push(*token);
                lastOperator = true;
        }
      token = strtok(NULL, " \n");
   } // while

   cout << endl;

} // InfixToPostfix

// ==========================================================

int main (int argc, char *argv[] ) {

   // declarations
   string oneLine; 

   // declare and initialize filename
   ifstream infile (argv[1]);

   // read and display lines
   getline(infile, oneLine);

   while (!infile.eof()) {
      // pass oneLine to InfixToPostfix for processing
      InfixToPostfix(oneLine);
      getline(infile, oneLine);

   } // while

   // output
   infile.close();
   cout << "All done!\n";
   return 0;

} // end maini
