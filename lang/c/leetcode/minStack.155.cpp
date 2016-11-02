/**
 * A write stack as linkedlist style
 * It don't have limit of capblity, also don't need call realloc function
 *
 * compile with command
 *      g++ -std=gnu++0x minStack.155.cpp -g -Og
 */

#include <stdexcept>
#include <iostream>

class Node {
    public:
        int mMin;
        int mX;
        Node *mLink;
        Node(int x, int min) {
            mX = x;
            mMin = min;
            mLink = nullptr;
        }
        Node(int x, int min, Node * link) {
            mX = x;
            mMin = min;
            mLink = link;
        }
        int getX() {
            return mX;
        }

        int getMin() {
            return mMin;
        }

        Node *getLink() {
            return mLink;
        }

};

class MinStack {
    private:
        void chkEmpty() {
            if (mHead == nullptr) {
                throw std::invalid_argument("cannot pop from empty stack");
            }
        }
    public:
        Node *mHead;
        /** initialize your data structure here. */
        MinStack() {
            mHead = nullptr;
        }
        void push(int x) {
            int min;
            if (mHead == nullptr) {
                min = x;
            } else {
                min = x < mHead->getMin() ? x : mHead->getMin();
            }
            Node *n = new Node(x, min, mHead);
            mHead = n;
        }

        void pop() {
            chkEmpty();
            mHead = mHead->getLink();
        }

        int top() {
            chkEmpty();
            return mHead->getX();
        }

        int getMin() {
            chkEmpty();
            return mHead->getMin();
        }

};

int tryMinStack() {
    MinStack *stk = new MinStack();
    stk->push(4);
    stk->push(2);
    stk->push(3);
    stk->push(1);
    stk->push(5);

    do {
        int x = stk->top();
        int min = stk->getMin();
        std::cout << x << "\t" << min << std::endl;
        stk->pop();
    } while (true); // continue to pop until empty exception
    return 0;
}

int main() {
    tryMinStack();
    return 0;
}
