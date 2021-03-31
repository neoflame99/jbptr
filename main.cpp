#include <iostream>
#include "inc/jbptr.h"

using namespace std;

template < typename T> jbptr<T> make_jbptr(T *p){
    return jbptr(p, jbptr_del<T>());
}
struct cmplx{
    double *re, *im;
    cmplx():re(new double(0)), im( new double(0)){}
    cmplx(double r, double i): re(new double(r)), im(new double(i)){}
    cmplx(const cmplx& ot): re(new double(*ot.re)), im(new double(*ot.im)){}
    ~cmplx(){
        delete re;
        delete im;
    }

    cmplx& operator= ( const cmplx& ot){
        re = new double(*ot.re);
        im = new double(*ot.im);
        return *this;
    }
    friend ostream& operator<< (ostream& os, const cmplx& cm);
};

ostream& operator<< (ostream& os, const cmplx& cm){
    os << *cm.re << " + " << *cm.im << "i " << endl;
    return os;
}

int main()
{

    jbptr a( new int, jbptr_del<int>() );
    (*a) = 10;
    cout << (*a) << endl;

    jbptr c = a;
    cout << (*c) << endl;

    jbptr b( new int, jbptr_del<int>());
    (*b) = 20;
    cout << (*b) << endl;

    jbptr<int> d;
    d = b;
    cout << (*d) << endl;

    jbptr<double> e;
    e = make_jbptr(new double(3.1415));

    jbptr f(new int[3], jbptr_del<int[]>() );
    for(int i=0; i < 3; ++i)
        f[i]=i+1;

    for(int i=0; i < 3; ++i)
        cout << f[i];

    jbptr ca(new cmplx(1,2));
    cout << *ca << endl;

    jbptr cb(new cmplx[3], jbptr_del<cmplx[]>() );
    for(int i=0; i < 3; ++i)
        cb[i] = cmplx(i, i+2);

    for(int i=0; i < 3; ++i)
        cout << cb[i];


    return 0;
}
