#include <iostream>
#include <string>

using namespace std;

static const double kPi = 3.1415926;

class Shape {
    public:
        void show() {
            cout << name() << " area=" << area() << " circ=" << circ() << endl;
        }
        virtual string name() = 0;
        virtual double area() = 0;
        virtual double circ() = 0;
};

class Circle : public Shape
{
    public:
        double _r;
        Circle(double r) :_r(r) {}
        string name() {
            return string("circle");
        }
        double area() {
            return kPi * _r * _r;
        }
        double circ() {
            return 2 * kPi * _r;
        }
};

class Rect : public Shape
{
    public:
        double _w, _h;
        Rect(double w, double h) : _w(w), _h(h) {}
        string name() {
            return string("rect");
        }
        double area() {
            return _w * _h;
        }
        double circ() {
            return 2 * (_w + _h);
        }
};

class Square : public Shape
{
    public:
        double _w;
        Square(double w) : _w(w) {}
        string name() { return string("square"); }
        double area() { return _w * _w; }
        double circ() { return 4 * _w; }
};

int main() {
    Shape *c = new Circle(1);
    Shape *r = new Rect(3, 4);
    Shape *s = new Square(2);
    c->show();
    r->show();
    s->show();
    delete c;
    delete r;
    delete s;
    return 0;
}
