#ifndef JBPTR_H
#define JBPTR_H
#include <stdio.h>
#include <iostream>

typedef unsigned int  uint;
typedef unsigned char uchar;

// deleter functor
template <typename T> struct jbptr_del{
    void operator() (T* p){
        delete p;
#ifdef _debug_
        std::cout << "jbptr_del<T> called "<< std::endl;
#endif
    }
};
// deleter functor; partial specialization of jbptr_del
template <typename T> struct jbptr_del<T[]>{
    void operator() (T* p){
        delete [] p;
#ifdef _debug_
        std::cout << "jbptr_del<T[]> called "<< std::endl;
#endif
    }
};

template <typename T, typename Del=jbptr_del<T> > class jbptr{
private:
    T   *ptr = nullptr;
    int *cnt = nullptr;
    Del  del ;
private:
    void dealloc(){
        if( *cnt <= 0){
            if( ptr!= nullptr ) del(ptr);
            delete cnt;
            ptr = nullptr;
            cnt = nullptr;
#ifdef _debug_
            fprintf(stderr, " ptr has been deleted\n");
#endif
        }
    }
public:
    jbptr() : ptr(nullptr), cnt (new int(0)) {}
    jbptr(T* ptr) : ptr( ptr), cnt( new int(1)){}
    jbptr(T* ptr, Del del) : ptr( ptr), cnt( new int(1)), del(del){}

    // copy constructor
    jbptr(const jbptr<T>& tptr): ptr( tptr.ptr), cnt( tptr.cnt), del(tptr.del){
        ++(*cnt);
#ifdef _debug_
        fprintf(stderr, "copy constructor\n");
#endif
    }

    // move constructor
    jbptr( jbptr<T>&& tptr){
        std::swap(ptr, tptr.ptr);
        std::swap(cnt, tptr.cnt);
        std::swap(del, tptr.del);
#ifdef _debug_
        fprintf(stderr, "move constructor");
#endif
    }

    ~jbptr() {
        if( *cnt > 0)
            --(*cnt);
#ifdef _debug_
        fprintf(stderr," ptrcnt : %d\n", *cnt);
#endif
        dealloc();
    }

    // copy assignment
    jbptr<T, Del>& operator= ( const jbptr<T, Del>& tptr){
        if( this != &tptr ){
            if(*cnt == 0){
                dealloc();
            }else if(*cnt == 1){
                --(*cnt);
                dealloc();
            }else{
                --(*cnt);
            }
            ptr = tptr.ptr;
            cnt = tptr.cnt;
            del = tptr.del;
            ++(*cnt);
        }
#ifdef _debug_
        fprintf(stderr,"copy assignment\n");
#endif
        return *this;
    }

    // move assignment
    jbptr<T, Del>& operator= ( jbptr<T, Del>&& tptr) {
        std::swap(ptr, tptr.ptr);
        std::swap(cnt, tptr.cnt);
        std::swap(del, tptr.del);
#ifdef _debug_
        fprintf(stderr,"move assignment\n");
#endif
        return *this;
    }

    const T& operator[] (uint i) const{
        return ptr[i];
    }
    T& operator[] (uint i) {
        return ptr[i];
    }

    T& operator* (){
        return *ptr;
    }

    T* operator-> (){
        return ptr;
    }

};


#endif // JBPTR_H
