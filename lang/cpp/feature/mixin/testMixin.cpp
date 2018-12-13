#include <iostream>
using namespace std;

struct Number {
    typedef int value_type;
    int n;
    void set(int v) {
        n = v;
    }

    int get() const {
        return n;
    }

};

template < typename BASE, typename T = typename BASE::value_type >
struct Undoable : public BASE
{
    typedef T value_type;
    T before;
    void set(T v) {
        before = BASE::get(); BASE::set(v);
    }

    void undo() {
        BASE::set(before);
    }

};

template < typename BASE, typename T = typename BASE::value_type >
struct Redoable : public BASE
{
    typedef T value_type;
    T after;
    void set(T v) {
        after = v; BASE::set(v);
    }

    void redo() {
        BASE::set(after);
    }

};

typedef Redoable < Undoable < Number > > ReUndoableNumber;

int main()
{
    ReUndoableNumber mynum;
    mynum.set(42); mynum.set(84);
    cout << mynum.get() << '\n';// 84
    mynum.undo();
    cout << mynum.get() << '\n';// 42
    mynum.redo();
    cout << mynum.get() << '\n';// back to 84

}
