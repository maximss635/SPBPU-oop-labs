#ifndef STRING_MYSTRING_H
#define STRING_MYSTRING_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>  // for size_t
#include <cstring>
#include <cmath>

template <typename T> class base_iterator;
// T=char => iterator; T=const char => const_iterator


template <typename T> class base_reverse_iterator;
// T=char => reverse_iterator; T=const char => const_reverse_iterator

class MyString {
public:
    typedef unsigned short int Index;
    typedef base_reverse_iterator<char> reverse_iterator;
    typedef base_reverse_iterator<const char> const_reverse_iterator;
    typedef base_iterator<char> iterator;
    typedef base_iterator<const char> const_iterator;

    MyString();
    MyString(const MyString& str);               // copy constructor
    MyString(MyString&& str) noexcept ;          // move constructor
    explicit MyString(const char* str);
    MyString(std::initializer_list<char>& il);
    explicit MyString(const std::string& str);

    [[noreturn]] MyString(const char* str, size_t count);
    MyString(size_t count, char c);
    explicit MyString(unsigned int num);

    ~MyString();

    MyString& operator= (const char* str);
    MyString& operator= (const std::string& str);
    MyString& operator= (const MyString& str);          // copy assigment
    MyString& operator= (MyString&& str);               // move assigment
    MyString& operator= (char c);

    MyString& operator+= (const char* str);
    MyString& operator+= (const std::string& str);
    MyString& operator+= (const MyString& str);

    bool operator== (const MyString& str) const;
    bool operator!= (const MyString& str) const;

    bool operator> (const MyString& str) const;
    bool operator< (const MyString& str) const;
    bool operator>= (const MyString& str) const;
    bool operator<= (const MyString& str) const;

    friend std::ostream& operator<< (std::ostream& os, MyString& str);
    friend std::istream& operator>> (std::istream& is, MyString& str);
    //Operator<<(std::basic_ofstream), Operator>>(std::basic_ifstream)

    void clear();
    void shrink_to_fit();

    char operator[] (Index i) const;
    char* c_str() const;
    char* data();
    size_t size() const;
    size_t capacity() const;
    bool empty() const;

    void insert(Index i, const char* mid_str);
    void insert(Index i, size_t count, char c);
    void insert(Index i, char* mid_str, size_t count);
    void insert(Index i, const std::string& str);
    void insert(Index i, const std::string& str, size_t count);
    //same with iterator:
    void insert(iterator i, const char* mid_str);
    void insert(iterator i, size_t count, char c);
    void insert(iterator i, char* mid_str, size_t count);
    void insert(iterator i, const std::string& str);
    void insert(iterator i, const std::string& str, size_t count);

    void append(size_t count, char c);
    void append(const char* str);
    void append(const char* str, Index i, size_t count);
    void append(const char* str, iterator i, size_t count);
    void append(const std::string& str);
    void append(const std::string& str, Index i, size_t count);

    Index find(const char* substr) const;
    Index find(const char* substr, Index i_) const;
    Index find(const std::string& substr) const;
    Index find(const std::string& substr, Index i_) const;

    void replace(Index i, size_t count, const char* str);
    void replace(Index i, size_t count, const std::string& str);
    // same with iterator:
    void replace(iterator i, size_t count, char* str);
    void replace(iterator i, size_t count, const std::string& str);

    MyString substr(Index i, size_t count) const;
    MyString substr(Index i) const;

    void erase(Index i, size_t count);
    void erase(iterator i, size_t count);
    int to_int() const;
    char at(int index) const;

    iterator begin();
    iterator end();
    const_iterator cbegin();
    const_iterator cend();
    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator crbegin();
    const_reverse_iterator crend();

private:
    void change_capacity(size_t new_capacity);
    size_t round_size(size_t size);

    char* c_str_;
    char* data_;
    size_t size_, capacity_;
    static const int default_size  = 8;
};

MyString operator+ (const MyString& left, const char* right);
MyString operator+ (const MyString& left, const MyString& right);
MyString operator+ (const MyString& left, const std::string& right);

std::ostream& operator<< (std::ostream& os, MyString& str);
std::istream& operator>> (std::istream& is, MyString& str);

// T=char => iterator; T=const char => const_iterator
template <typename T>
class base_iterator {
public:
    explicit base_iterator(T* cur = nullptr) : cur_(cur) { }

    bool operator== (base_iterator<T> other) { return other.cur_ == cur_; }

    bool operator!= (base_iterator<T> other) { return other.cur_ != cur_; }

    T& operator* () { return *cur_; }

    /*virtual*/ base_iterator<T> operator++ () {
        ++cur_;
        return* this;
    }

    /*virtual*/ base_iterator<T> operator++ (int) {
        base_iterator<T> tmp(cur_);
        ++cur_;
        return tmp;
    }

    /*virtual*/ base_iterator<T> operator-- () {
        --cur_;
        return* this;
    }

    /*virtual*/ base_iterator<T> operator-- (int) {
        base_iterator<T> tmp(cur_);
        --cur_;
        return tmp;
    }
//protected:
    T *cur_;
};


// T=char => reverse_iterator; T=const char => const_reverse_iterator
template <typename T>
class base_reverse_iterator : public base_iterator<T> {
public:
    base_reverse_iterator<T>(T* cur, T* ptr_begin) : base_iterator<T>(cur), ptr_begin_(ptr_begin) { }

    base_reverse_iterator<T> operator++ () {
        if (base_iterator<T>::cur_ != ptr_begin_) {
            --base_iterator<T>::cur_;
            return* this;
        }

        base_iterator<T>::cur_ = nullptr;
        return* this;
    }

    base_reverse_iterator<T> operator++ (int) {
        base_iterator<T> tmp(base_iterator<T>::cur_, ptr_begin_);
        ++*this;
        return tmp;
    }

    base_reverse_iterator<T> operator-- () {
        if (base_iterator<T>::cur_ != nullptr) {
            ++base_iterator<T>::cur_;
            return* this;
        }

        base_iterator<T>::cur_ = ptr_begin_;
        return* this;
    }

    base_reverse_iterator<T> operator-- (int) {
        base_iterator<T> tmp(base_iterator<T>::cur_, ptr_begin_);
        --*this;
        return tmp;
    }

protected:
    T* ptr_begin_;
};


#endif //STRING_MYSTRING_H
