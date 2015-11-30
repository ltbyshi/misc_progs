#include <iostream>
using namespace std;

class Point
{
public:
    Point(): x(0), y(0) {}
    Point(double x, double y): x(x), y(y) {}
    
    virtual void Print() const {
        cout << "(" << x << ", " << y << ")" << endl; 
    }

    void Add(const Point& p) {
        x += p.x; y += p.y; 
    }
public:
    double x, y;
};

class ScaledPoint: public Point
{
public:
    ScaledPoint(const Point& p, double factor = 2)
        : Point(p), factor(factor) {}
    virtual void Print() const {
        cout << "(" << factor*x << ", " << factor*y << ")" << endl;
    }
public:
    double factor;
};

int main()
{
    Point a(3, 4), b(1, 2);
    ScaledPoint c(a, 2);

    typedef void (*PF_Point)(Point*);
    typedef void (*PF_ScaledPoint)(ScaledPoint*);
    PF_Point p_Print = (PF_Point)(&Point::Print);
    PF_ScaledPoint p_PrintScaled = (PF_ScaledPoint)(&ScaledPoint::Print);
    p_Print(&a);
    p_Print(&b);
    p_PrintScaled(&c);

    return 0;
}
