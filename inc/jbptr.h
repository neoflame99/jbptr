/*
 * Copyright (C) 2021. Jong B. Choi
 * License : MIT License
 * contact : neoflame99@naver.com
 * reference: https://ms.sapientia.ro/~manyi/teaching/c++/CPP_v1.2.pdf, Margit ANTAL
 *            www.cplusplus.com, www.cppreferenc.com and so on.
 * remark :
 *         1. Memory leak has been checked by valgrind with qtcreator IDE
 *         2. use jbptr with jbptr_del functor
 *             jbptr_del<T> and jbptr_del<T[]> are ready.
 *         3. examples
 *              1) jbptr ptr(new int(0));
 *              2) jbptr ptr(new int(0), jbptr_del<int>() );
 *              3) jbptr ptr(new int[5], jbptr_del<int[]>() );  //-> int type array
 *              4) jbptr<int> a( new int(0) );
 *              5) jbptr<int> b = a;
 *              6) jbptr<int> c;
 *                 c = a;
 *
 *              7) jbptr<int> a( new int(0) );
 *                 std::cout << *(a) << endl;
 *
 *              8) jbptr ptr(new int[5], jbptr_del<int[]>() );
 *                 ptr[1]= 10;
 *
 *              9) struct a_str { char* c_str; a_str(char* p):c_str(p){} };
 *                 jbptr ptr_s ( new a_str("Hello World!") );
 *                 cout << ptr_s->c_str << endl;
 *
 */

#ifndef JBPTR_H
#define JBPTR_H
#include <stdio.h>
#include <iostream>

typedef unsigned int  uint;
typedef unsigned char uchar;

// deleter functor for jbptr<T>
template <typename T> struct jbptr_del{
    constexpr void operator() (T* p){
        delete p;
#ifdef _debug_
        std::cerr << "jbptr_del<T> called "<< std::endl;
#endif
    }
};
// deleter functor for jbptr<T[]>; partial specialization of jbptr_del
template <typename T> struct jbptr_del<T[]>{
    constexpr void operator() (T* p){
        delete [] p;
#ifdef _debug_
        std::cerr << "jbptr_del<T[]> called "<< std::endl;
#endif
    }
};

template <typename T, typename Del=jbptr_del<T> > class jbptr{
private:
    T   *ptr = nullptr;
    int *cnt = nullptr;
    Del  del ;
private:
    constexpr void dealloc(){
        if( *cnt <= 0){
            if( ptr!= nullptr ) del(ptr);
            delete cnt;
            ptr = nullptr;
            cnt = nullptr;
#ifdef _debug_
            std::cerr << "  ->ptr has been deleted\n";
#endif
        }
    }
    constexpr void finalize(){
        if( *cnt > 0)
            --(*cnt);
#ifdef _debug_
        std::cerr<< "  ->ptrcnt : " << *cnt << std::endl;
#endif
        dealloc();
    }

public:
    constexpr jbptr() : ptr(nullptr), cnt (new int(0)) {}
    constexpr jbptr(T* ptr) : ptr( ptr), cnt( new int(1)){}
    constexpr jbptr(T* ptr, Del del) : ptr( ptr), cnt( new int(1)), del(del){}

    // copy constructor
    jbptr(const jbptr<T>& tptr): ptr( tptr.ptr), cnt( tptr.cnt), del(tptr.del){
        ++(*cnt);
#ifdef _debug_
        std::cerr << "  ->copy constructor\n";
#endif
    }

    // move constructor
    jbptr( jbptr<T>&& tptr){
        std::swap(ptr, tptr.ptr);
        std::swap(cnt, tptr.cnt);
        std::swap(del, tptr.del);
#ifdef _debug_
        std::cerr << "  ->move constructor\n";
#endif
    }

    ~jbptr() {
        finalize();
    }

    // copy assignment
    jbptr<T, Del>& operator= ( const jbptr<T, Del>& tptr){
        if( this != &tptr ){
            finalize();
            ptr = tptr.ptr;
            cnt = tptr.cnt;
            del = tptr.del;
            ++(*cnt);
        }
#ifdef _debug_
        std::cerr << "  ->copy assignment\n";
#endif
        return *this;
    }

    // move assignment
    jbptr<T, Del>& operator= ( jbptr<T, Del>&& tptr) {
        std::swap(ptr, tptr.ptr);
        std::swap(cnt, tptr.cnt);
        std::swap(del, tptr.del);
#ifdef _debug_
        std::cerr << "  ->move assignment\n";
#endif
        return *this;
    }

    constexpr T& operator[] (uint i) const{
        return ptr[i];
    }
    constexpr T& operator[] (uint i) {
        return ptr[i];
    }

    constexpr T& operator* (){
        return *ptr;
    }

    constexpr T* operator-> (){
        return ptr;
    }

    constexpr T* get() const{
        return ptr;
    }

    constexpr void reset(T* ptr, Del del){
        if( this->ptr != ptr){
            // clear current state
            finalize();

            this->ptr = ptr;
            this->cnt = new int(1);
            this->del = del;
#ifdef _debug_
            std::cerr << "  ->reset called \n";
#endif
        }
    }
};


#endif // JBPTR_H
