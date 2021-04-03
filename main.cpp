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
#ifdef _debug_
        cout<< "Cmplx deleted " ;
#endif
    }

    cmplx& operator= ( const cmplx& ot){
        re = new double(*ot.re);
        im = new double(*ot.im);
        return *this;
    }
    cmplx& operator= ( cmplx&& ot){
        swap(re, ot.re);
        swap(im, ot.im);
        return *this;
    }
    friend ostream& operator<< (ostream& os, const cmplx& cm);
};

ostream& operator<< (ostream& os, const cmplx& cm){
    os << *cm.re << " + " << *cm.im << "i " << endl;
    return os;
}
struct a_str {
    char* c_str;
    a_str( char* p_str): c_str(p_str){}
};

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

    jbptr<double> g ( jbptr( new double(2.8) ));
    cout << *g << endl;

    jbptr<double> e;
    e = make_jbptr(new double(3.1415));
    cout << *e << endl;

    jbptr f(new int[3], jbptr_del<int[]>() );
    for(int i=0; i < 3; ++i)
        f[i]=i+1;

    for(int i=0; i < 3; ++i)
        cout << f[i] << " ";
    cout << endl;

    jbptr ca(new cmplx(1,2));
    cout << *ca << endl;

    jbptr cb(new cmplx[3], jbptr_del<cmplx[]>() );
    for(int i=0; i < 3; ++i){
        cb[i] = cmplx(i, i+2);
    }

    cout << endl;
    for(int i=0; i < 3; ++i)
        cout << cb[i];

    cout << *ca->re + *ca->im  << endl;

    char buf[] = "Hello World";
    jbptr pstr (new a_str(buf));
    cout << pstr->c_str << endl;


    cmplx *pcc = new cmplx[4];
    cb.reset(pcc, jbptr_del<cmplx[]>() );

    for(int i=0; i < 4; ++i){
        *cb[i].re = i+10;
        *cb[i].im = i+12;
    }
    cout << endl;
    for(int i=0; i < 4; ++i)
        cout << cb[i];

    cmplx *pcb0 = cb.get();
    cout << "\nget pointer by get() method: " << *pcb0 << endl;

    return 0;
}
