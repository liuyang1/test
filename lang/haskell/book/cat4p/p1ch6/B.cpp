#include <iostream>

static const double kPi = 3.1415926;

class Shape {
    public:
        virtual double area() = 0;
};

class Circle : public Shape
{
    public:
        double _r;
        Circle(double r) :_r(r) {}
        double area() {
            return kPi * _r * _r;
        }
};

class Rect : public Shape
{
    public:
        double _w, _h;
        Rect(double w, double h) : _w(w), _h(h) {}
        double area() {
            return _w * _h;
        }
};

int main() {
    Shape *c = new Circle(1);
    Shape *r = new Rect(3, 4);
    std::cout << "circle area=" << c->area() << std::endl;
    std::cout << "rect area=" << r->area() << std::endl;
    delete c;
    delete r;
    return 0;
}
