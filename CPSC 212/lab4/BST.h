/* Class: CPSC 212 
 * Assignment: Lab 4
 * Name: Joshua Hull (jhull@clemson.edu)
 *
 * Since a lot of the functions of a binary trees involve the nodes and not the
 * tree itself I decided to create a sub-class of Node inside the BST class.
 * The entire subclass is private so it doesn't affect the public interface of
 * the BST class.
 */

class BST{
        private:
                    class Node{
                        private: 
                                int value;
                                Node *l;
                                Node *r;
                        public:
                                Node(int value);
                                ~Node();

                                bool insert(int value);
                                bool find(int value);

                                void preorder();
                                void inorder();
                                void postorder();

                                bool remove(int value);
                    };
                Node *root;
        public:
                BST();
                ~BST();

                bool isEmpty();
                bool insert(int value);
                bool find(int value);

                void preorder();
                void inorder();
                void postorder();

                bool remove(int value);
};
