/*
 * CP SC 212 Lab 8: NQueens
 * Joshua Hull (jhull@clemson.edu)
 */

#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

// Global variables of things that are accessed all over the place.
int numSolutions = 0;
int N;
int k;
int column = 0;

class Queen {
public:
    int m_row;
    int m_col;

    void place(int row, int col);
    bool isSafe(Queen* queens, int numQueens);
};


void Queen::place(int row, int col) {

    // Place this queen at the specified point on the board.
    m_row = row;
    m_col = col;
}

bool Queen::isSafe(Queen* queens,int numQueens) {

    // For every queen in the array except the last one. The last one is 'this'
    // queen.
    for(int i = 0; i < numQueens; i++) {
        Queen q = queens[i];
        int q_row(q.m_row),q_col(q.m_col);

        // Calcualte the slope of the line between this queen and the one we are
        // checking against.
        double slope = double(m_row - q_row)/double(m_col - q_col);

        // Same row, same column, or on a diagonal.
        if(m_row == q_row || m_col == q_col || slope == 1 || slope == -1) {
            return false;
        }
    }
    return true;
}

void printQueens(Queen* q) {

    // For ever column...
    for (int i = 0; i < N; i ++) {
        // For every row...
        for (int j = 0; j < N; j ++) {
            // Check to see if there is a queen in the spot.
            bool here = false;
            for (int l = 0; l < N; l ++) {
                if(q[l].m_row == i && q[l].m_col == j)
                    here = true;
            }
            if(here)
                cout << " Q";
            else
                cout << " _";
        }
        // Newline to the next row.
        cout << endl;
    }
    // Give some space between boards.
    cout << endl << endl;
}

bool placeQueen(Queen *q) {

    bool flag = true;

    // newq is the queen we will be placing.
    Queen newq;

    // Start on the top row.
    for(int row = 0; row < N; row++) {

        // Place the piece, add it to the array, and check to see if the
        // placement is valid.
        newq.place(row,column);
        q[column] = newq;
        flag = newq.isSafe(q,column);


        while(flag) {
            // If we are on the last column
            if(column == N-1) {
                // and still need to print solutions then print this one, add
                // to the total number of solution, and backstep to the
                // previous column.
                if(numSolutions < k) {
                    printQueens(q);
                    numSolutions++;
                    column--;
                    return false;
                } else
                    // Here we have printed all the solutions we were asked
                    // for so just exit.
                    exit(1);
            } else
                // Here we are not at the last column so recuse to the next
                // one.
                column++;
            flag = placeQueen(q);
        }
    }
    if(column == 0)
        // So we've done all the rows. If we are on the first column then there
        // are no more solutions and k was too big for N.
        exit(1);
    else {
        // Here we are at the last row of a middle column. Backtrace to the
        // previous column.
        column--;
        return false;
    }
}

int main(int argc, char* argv[]) {

    // Make sure we get what we need.
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " k N" << endl;
        return 0;
    }

    // Store mission critical variables.
    N = atoi(argv[1]);
    k = atoi(argv[2]);

    // Create an array of N queens.
    Queen royals[N];

    // Start the ball rolling.
    placeQueen(royals);
    return 1;
}
