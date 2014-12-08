// Cpsc 210: Lab 14: mynode.h (c) 2012 Joshua Hull

#ifndef MYNODE_H
#define MYNODE_H

using namespace std;

class mynode {
   public:
      // Constructors
      mynode();
      mynode(const string & word);

      // Destructor
      ~mynode();

      // Methods
      string getword();
      mynode * getnext();
      void setnext(mynode * ptr);
      mynode * getprev();
      void setprev(mynode * ptr);

   private:
      // Data associated with mynode object
      string nodeword;
      mynode *next;
      mynode *prev;
};

#endif
