#include "MyString.h"

// in out substr

MyString::MyString()
    : size_(0),
    capacity_(default_size),
    c_str_(new char [default_size]),
    data_(nullptr)
{
    c_str_[0] = '\0';
}

MyString::MyString(const MyString& str)         // copy constructor
    : size_(str.size_),
    capacity_(str.capacity_),
    c_str_(new char [str.capacity_]),
    data_(nullptr)
{
    std::strcpy(this->c_str_, str.c_str_);
}

MyString::MyString(MyString&& str) noexcept          // move constructor
{
    *this = std::move(str);
}

MyString::MyString(const char* str)
{
    size_ = std::strlen(str);
    capacity_ = round_size(size_);
    c_str_ = new char[capacity_];
    std::strcpy(this->c_str_, str);

    data_ = nullptr;
}

MyString::MyString(std::initializer_list<char>& il)
    : size_(il.size()),
    capacity_(round_size(il.size())),
    c_str_(new char [round_size(il.size())]),
    data_(nullptr)
{
    std::strcpy(this->c_str_, il.begin());
}

MyString::MyString(const std::string& str)
    : size_(str.size()),
    capacity_(str.capacity()),
    c_str_(new char [str.capacity()]),
    data_(nullptr)
{
    std::memcpy(this->c_str_, str.c_str(), size_);
    c_str_[size_] = '\0';
}

MyString::MyString(const char* str, size_t count)
{
    size_ = count;
    capacity_ = round_size(size_);
    c_str_ = new char [capacity_];
    std::memcpy(this->c_str_, str, count);

    data_ = nullptr;
}

MyString::MyString(size_t count, char c)
{
    size_ = count;
    capacity_ = round_size(size_);
    c_str_ = new char [capacity_];
    std::memset(c_str_, c, count);

    data_ = nullptr;
}

MyString::MyString(unsigned int num)
{
    auto byte_size { [] (unsigned int num) -> int {
        return 1 + static_cast<int> (std::log2(num)) / 8;
    }};

    size_ = byte_size(num) * 2 + 2;        // 1 byte in hex = 2 symbols. + 2 symbols for '0', 'x'
    capacity_ = round_size(size());
    c_str_ = new char [capacity()];

    c_str_[size()] = '\0';

    char c;
    for (int i = size() - 1; i >= 2; --i) {
        c = (num & 0xf);
        if (c <= 9) {
            c_str_[i] = c + '0';
        } else {
            c_str_[i] = c - 10 + 'a';
        }
        num >>= 4;
    }

    c_str_[0] = '0';
    c_str_[1] = 'x';

    data_ = nullptr;
}


MyString::~MyString()
{
    delete [] c_str_;
    c_str_ = nullptr;

    if (data_ != nullptr) {
        delete [] data_;
        data_ = nullptr;
    }
}

MyString& MyString::operator= (const char* str)
{
    if (this->capacity_ < std::strlen(str)) {
        change_capacity(round_size(std::strlen(str)));
    }
    std::strcpy(this->c_str_, str);
    this->size_ = std::strlen(str);

    return* this;
}

MyString& MyString::operator= (const std::string& str)
{
    return (*this = str.c_str());
}

MyString& MyString::operator= (const MyString& str)     //  copy assigment
{
    if (this != &str) {
        return (*this = str.c_str());
    }
    else {
        return* this;
    }
}

MyString& MyString::operator= (MyString&& str) noexcept  //  move assigment
{
    if (this != &str) {
         delete[] this->c_str_;

        this->c_str_ = str.c_str_;
        this->size_ = str.size_;
        this->capacity_ = str.capacity_;

        str.c_str_ = nullptr;
        str.size_ = 0;
        str.capacity_ = 0;
    }

    return* this;
}

MyString& MyString::operator= (char c)
{
    this->c_str_[0] = c;
    this->size_ = 1;

    return* this;
}

MyString& MyString::operator+= (const char* str)
{
    if (this->capacity_ <= this->size_ + std::strlen(str)) {
        change_capacity(round_size(this->size_ + std::strlen(str)));
    }
    std::strcat(this->c_str_, str);
    this->size_ += std::strlen(str);

    return* this;
}

MyString& MyString::operator+= (const std::string& str)
{
    return (*this += str.c_str());
}

MyString& MyString::operator+= (const MyString& str)
{
    return (*this += str.c_str_);
}

void MyString::clear()
{
    *this = MyString();
}

void MyString::change_capacity(size_t new_capacity)
{
    if (capacity() == new_capacity) {
        return;
    }

    char * tmp = c_str_;

    c_str_ = nullptr;
    c_str_ = new char [new_capacity];

    if (new_capacity > capacity()) {
        std::strcpy(c_str_, tmp);
    } else {
        std::strncpy(c_str_, tmp, new_capacity - 1);
    }

    delete [] tmp;

    capacity_ = new_capacity;
    if (size() >= capacity()) {
        size_ = capacity() - 1;
    }
}

void MyString::shrink_to_fit()
{
    change_capacity(size() + 1);
}

char& MyString::operator[] (Index i) const
{
    return c_str_[i];
}

char* MyString::c_str() const
{
    return c_str_;
}

size_t MyString::size() const
{
    return size_;
}

size_t MyString::capacity() const
{
    return capacity_;
}

bool MyString::empty() const
{
    return (size() == 0);
}

MyString operator+ (const MyString& left, const char* right)
{
    MyString res = left;
    return (res += right);
}

MyString operator+ (const MyString& left, const MyString& right)
{
    return (left + right.c_str());
}

MyString operator+ (const MyString& left, const std::string& right)
{
    return (left + right.c_str());
}

bool MyString::operator== (const MyString& str) const
{
    if (this == &str) {
        return true;
    }

    return (std::strcmp(this->c_str_, str.c_str()) == 0);
}

bool MyString::operator!= (const MyString& str) const
{
    return !(*this == str);
}


std::ostream& operator<< (std::ostream& os, MyString& str)
{
    return (os << str.c_str_);
}

std::istream& operator>> (std::istream& is, MyString& str)
{
    str.clear();

    char c;
    do {
        c = is.get();
        if (c == '\n' || c == -1) {
            break;
        }
        str.append(1, c);
    } while (1);

    return is;
}


void MyString::insert(Index i_, const char* mid_str)
{
    if (i_ > size()) {
        throw "Out of range";
    }

    if (size_ + std::strlen(mid_str) >= capacity_) {
        change_capacity(round_size(size_ + std::strlen(mid_str) + 1));
    }


    for (int i = size_ + std::strlen(mid_str) - 1; i >= i_ + std::strlen(mid_str); --i) {
        c_str_[i] = c_str_[i - std::strlen(mid_str)];
    }

    for (int i = i_, j = 0; i < i_ + std::strlen(mid_str); ++i, ++j) {
        c_str_[i] = mid_str[j];
    }

    size_ += std::strlen(mid_str);
    c_str_[size_] = '\0';
}

void MyString::insert(Index i, size_t count, char c)
{
    char* mid_str = new char [count + 1];
    std::memset(mid_str, c, count);
    mid_str[count] = '\0';
    insert(i, mid_str);
    delete [] mid_str;
}

void MyString::insert(Index i, char* mid_str, size_t count)
{
    std::memcpy(mid_str, static_cast<const char*>(mid_str), count);
    mid_str[count] = '\0';
    insert(i, mid_str);
}

void MyString::insert(Index i, const std::string& str)
{
    insert(i, str.c_str());
}

void MyString::insert(Index i, const std::string& str, size_t count)
{
    insert(i, str.c_str(), count);
}

int MyString::to_int() const
{
    return std::atoi(c_str());
}

char MyString::at(int i) const
{
    if ((i < 0) || (i >= size_)) {
        throw "Out of range";
    }

    return c_str_[i];
}


bool MyString::operator> (const MyString& str) const
{
    return std::strcmp(this->c_str(), str.c_str_) > 0;
}

bool MyString::operator< (const MyString& str) const
{
    return std::strcmp(this->c_str(), str.c_str_) < 0;
}

bool MyString::operator>= (const MyString& str) const
{
    return std::strcmp(this->c_str(), str.c_str_) >= 0;
}

bool MyString::operator<= (const MyString& str) const
{
    return std::strcmp(this->c_str(), str.c_str_) <= 0;
}



void MyString::append(size_t count, char c)
{
    char* tmp_str = new char [count + 1];
    std::memset(tmp_str, c, count);
    tmp_str[count] = '\0';

    append(tmp_str, 0, count);

    delete [] tmp_str;
}

void MyString::append(const char* str)
{
    append(str, 0, std::strlen(str));
}

void MyString::append(const char* str, Index i, size_t count)
{
    if (size_ + count >= capacity_) {
        change_capacity(round_size(size_ + count + 1));
    }
    std::strncpy(this->c_str_ + size_, str + i, count);
    size_ += count;
    c_str_[size()] = '\0';
}

void MyString::append(const std::string& str)
{
    append(str.c_str(), 0, str.size());
}

void MyString::append(const std::string& str, Index i, size_t count)
{
    append(str.c_str(), i, count);
}

void MyString::erase(Index i_, size_t count)
{
    if ((i_ < 0) || (i_ > size_)) {
        throw "Out of range";
    }

    for (int i = i_; i < size_ - count; ++i) {
        c_str_[i] = c_str_[i + count];
    }

    for (int i = size_ - count; i < size_; ++i) {
        c_str_[i] = '\0';
    }

    size_ -= count;
}

MyString MyString::substr(Index i, size_t count) const
{
    if (i + count >= size_) {
        throw "Out of range";
    }

    MyString ret;
    ret.insert(0, this->c_str() + i, count);

    return ret;
}

MyString MyString::substr(Index i) const
{
    if (i >= size_) {
        throw "Out of range";
    }


    MyString ret;
    ret.insert(0, this->c_str() + i);

    return ret;
}


MyString::Index MyString::find(const char* substr) const
{
    return find(substr, 0);
}

MyString::Index MyString::find(const char* substr, Index i_) const
{
    char c;
    for (int i = i_; i <= size_ - std::strlen(substr); ++i) {
        c = *(c_str_ + i + std::strlen(substr));
        *(c_str_ + i + std::strlen(substr)) = 0;
        if (!std::strcmp(c_str_ + i, substr)) {
            *(c_str_ + i + std::strlen(substr)) = 0;
            return i;
        }
        *(c_str_ + i + std::strlen(substr)) = c;
    }

    return static_cast <Index> (-1);
}

MyString::Index MyString::find(const std::string& substr) const
{
    return find(substr.c_str(), 0);
}

MyString::Index MyString::find(const std::string& substr, Index i_) const
{
    return find(substr.c_str(), i_);
}

void MyString::replace(Index i, size_t count, const char* str)
{
    if (i + count > size_) {
        throw "Out of range";
    }

    erase(i, count);
    insert(i, str);
}

void MyString::replace(Index i, size_t count, const std::string& str)
{
    replace(i, count, str.c_str());
}


// begin(), end(), cbegin(), cend()

MyString::iterator MyString::begin()
{
    return iterator(&c_str_[0]);
}

MyString::iterator MyString::end()
{
    return iterator(&c_str_[size_]);
}

MyString::const_iterator MyString::cbegin()
{
    return const_iterator(&c_str_[0]);
}

MyString::const_iterator MyString::cend()
{
    return const_iterator(&c_str_[size_]);
}


// rbegin(), rend(), crbegin, crend()

MyString::reverse_iterator MyString::rbegin()
{
    return reverse_iterator(&c_str_[size_ - 1], c_str_);
}

MyString::reverse_iterator MyString::rend()
{
    return reverse_iterator(nullptr, c_str_);
}

MyString::const_reverse_iterator MyString::crbegin()
{
    return const_reverse_iterator(&c_str_[size_ - 1], c_str_);
}

MyString::const_reverse_iterator MyString::crend()
{
    return const_reverse_iterator(nullptr, c_str_);
}



//overrided methods with itterator:

void MyString::insert(iterator i, const char *mid_str)
{
    insert(i.cur_ - c_str_, mid_str);
}

void MyString::insert(iterator i, size_t count, char c)
{
    insert(i.cur_ - c_str_, count, c);
}

void MyString::insert(iterator i, char *mid_str, size_t count)
{
    insert(i.cur_ - c_str_, mid_str, count);
}

void MyString::insert(iterator i, const std::string &str, size_t count)
{

    insert(i.cur_ - c_str_, str, count);
}

void MyString::insert(iterator i, const std::string &str)
{
    insert(i.cur_ - c_str_, str);
}

void MyString::append(const char *str, iterator i, size_t count)
{
    append(str, i.cur_ - c_str_, count);
}

void MyString::replace(iterator i, size_t count, char *str)
{
    replace(i.cur_ - c_str_, count, str);
}

void MyString::replace(iterator i, size_t count, const std::string &str)
{
    replace(i.cur_ - c_str_, count, str);
}

void MyString::erase(iterator i, size_t count)
{
    erase(i.cur_ - c_str_, count);
}

size_t MyString::round_size(size_t size)
{
    return ((((size) / default_size) + 1) * default_size);
}

char* MyString::data()
{
    if (data_ != nullptr) {
        delete [] data_;
    }

    data_ = new char [size()];
    std::memcpy(data_, c_str_, size());

    return data_;
}
