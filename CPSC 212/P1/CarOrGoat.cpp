#include <iostream>
#include <stdlib.h>

using namespace std;

int main() {

    // The doors either have a car or they don't so bools made sense.
    bool doors[3];

    // Number of times we picked the car.
    int success = 0;

    // I couldn't figure out a good way of calculating the new pick after the 
    // reveal. The value is the new pick after the pick at i and reveal at j.
    // Note: i != j, or at least shouldn't.
    int lut [3][3] = {-1,2,1,2,-1,0,1,0,-1};

    srand(time(NULL));
    for (int i = 0; i < 1000; i++){
        doors[0] = false;
        doors[1] = false;
        doors[2] = false;
        // Pick a random door for the car, the contestant, and the presenter to
        // reveal.
        int car = (rand() % (3));
        int pick = (rand() % (3));
        int reveal = (rand() % (3));
        
        // The game wouldn't be very exciting if he didn't give us a chance to
        // switch.
        while (reveal == car || reveal == pick)
                reveal = (rand() % (3));
        doors[car] = true;

        for(int j = 0; j < 3; j++) {
            if(doors[j])
                cout << " C ";
            else
                cout << " G ";
        }
        cout << " Pick: Door " << pick +1; 
        cout << " Reveal: Door " << reveal +1;

        // We always switch since that's what we're calculating the probably of
        // success with.
        pick = lut[pick][reveal];
        cout << " New Pick: " << pick + 1;

        if(doors[pick]){
            success++;
            cout << " Won: Car";
        }
        else{
            cout << " Won: Goat";
        }
        cout << endl;
    }
    cout << "Success rate over 1,000 trials: " << success/1000.0 *100 << "%" << endl;
    return(0);
}
