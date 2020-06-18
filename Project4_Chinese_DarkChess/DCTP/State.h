
#ifndef STATE_INCLUDED
#define STATE_INCLUDED

#include <stdio.h>
#include <stdlib.h>

class State {
   public:
    State();                           // Default Constructor
    State(const State& s);             // Copy Constructor
    ~State(void);                      // destructor
    

   private:
    int color;
    int board[32];
};

#endif