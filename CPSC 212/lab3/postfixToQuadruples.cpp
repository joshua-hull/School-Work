#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stack>
#include <sstream>
#include <iomanip>
using namespace std;

// Name:        Joshua Hull
//              Lab #3, postfixToQuadruple

// Header Comment:
//
//

// InfixToPostfix processes one line of input
void postfixToQuadruple(string oneLine) {

   cout << oneLine << endl;

   // convert a string to a char *
   char * cstr;
   cstr = new char [oneLine.size()+1];
   strcpy (cstr, oneLine.c_str());

   // Tokenize, i.e., break up one line into its component tokens
   char * token;                        // will hold one token from oneLine
   token = strtok(cstr, " \n");
   stack <string> operands;

   char tmpCounter = '0';               // Number of temporary variables weve created.
   string tmpString;                    /*
                                         *  A string for rach tmp variable we push to the
                                         *  stack.
                                         */
   string op1;                          // The operators of each quadruple.
   string op2;

   string tokenString = string(token);  // Create a string we can use with cout.

   operands.push("@");                  // Bottom of the stack marker.

   cout << setfill(' ');                // Used for nice formatting.

   while (token != NULL) {
        
        // Any alphanumaric character is treated as an operand.
        if(isalnum(*token)){
            operands.push(tokenString);
            token = strtok(NULL, " \n");
            tokenString = string(token);
        }

        // Any general binary operator.
        else if(*token == '+' || *token == '-' || *token == '*' || *token == '/' || *token == '^'){
            // If the stack is empty then there weren't enough operands.
            if(operands.top() == string("@")){
                cout << "     >>>Error, Invalid Postfix Expression!" << endl << endl;  
                return;
            }
            op2 = operands.top();
            operands.pop();

            if(operands.top() == string("@")){
                cout << "     >>>Error, Invalid Postfix Expression!" << endl << endl;
                return;
            }
            op1 = operands.top();
            operands.pop();

            // Create a new tmp operand, make a string, and push it onto the
            // stack.
            tmpCounter++;
            stringstream tmpStream;
            tmpStream << "t" << tmpCounter;
            tmpString = tmpStream.str();
            operands.push(tmpString);

            // Print out the quadruple.
            cout << "     " << token << " " << setw(2) << op1 << " " << setw(2) << op2 << " " << setw(2) << tmpString << endl;
            token = strtok(NULL, " \n");
            tokenString = string(token);

        } // General binary

        // Uniary minus.
        else if(*token == '~'){
           // If the stack is empty then there weren't enough operands.
           if(operands.top() == string("@")){
                cout << "    >>>Error, Invalid Postfix Expression!" << endl << endl;         
                 return;
           }

            op1 = operands.top();
            operands.pop();

            op2 = " ";
            tmpCounter++;

            // create a new tmp variable, make a string, and push onto the
            // stack
            stringstream tmpStream;
            tmpStream << "t" << tmpCounter;
            tmpString = tmpStream.str();
            operands.push(tmpString);

            // Print out the quadruple.
            cout << "     " << token << " " << setw(2) << op1 << " " << setw(2) << op2 << " " << setw(2) << tmpString << endl;
            token = strtok(NULL, " \n");
            tokenString = string(token);

        } // Uniary minus

        // Equals
        else if(*token == '='){
        
            // Check the stack has enough operands.
            if(operands.top() == string("@")){
               cout << "      >>>Error, Invalid Postfix Expression!" << endl << endl;
               return; 
            }
            op1 = operands.top();
            operands.pop();
        
            if(operands.top() == string("@")){
                cout << "     >>>Error, Invalid Postfix Expression!" << endl << endl;
                return;
            }
            op2 = " ";

            // Pull the last operand off the stack and use it as a destination.
            string dest = operands.top();
            operands.pop();

            cout << "     " << token << " " << setw(2) << op1 << " " << setw(2) << op2 << " " << setw(2) << dest << endl;
            token = strtok(NULL, " \n");
        
            if(token != NULL){
                tokenString = string(token);
            }
            
            // At this point the stack SHOULD be empty. If not, there were too
            // many operands.
            if(operands.top() != "@"){
                cout << "     >>>Error, Invalid Postfix Experssion" << endl << endl;
                return;
            }

        }// Equals
        
        // If it's not an operator we know or an operand there there has been a
        // problem.
        else {
            cout << "     >>>Error, Invalid Postfix Expression!" << endl << endl;
            return;
        }

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
      postfixToQuadruple(oneLine);
      getline(infile, oneLine);

   } // while

   // output
   infile.close();
   cout << "All done!\n";
   return 0;

} // end maini
