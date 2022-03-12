#include "MyString.h"

#define ROUND_SIZE(size) ((((size) / DEFAULT_SIZE) + 1) * DEFAULT_SIZE)

// Base String functions
char* strcpy_(char* to, const char* from)
{
    char* p = to;
    while (*from != '\0') {
        *(p++) = *(from++);
    }
    *p = '\0';

    return to;
}

char* strncpy_(char* to, const char* from, size_t n)
{
    size_t i;
    for (i = 0; (i < n) && (from[i] != '\0'); ++i) {
        to[i] = from[i];
    }
    to[i] = '\0';

    for (; i < n; ++i) {
        to[i] = '\0';
    }

    return to;
}

char* strset_(char* str, char c, int count)
{
    for (int i = 0; i < count; ++i) {
        str[i] = c;
    }
    str[count] = 0;

    return str;
}

char* strcat_(char* to, const char* from)
{
    size_t i, j;
    for (i = 0; to[i] != '\0'; i++);
    for (j = 0; from[j] != '\0'; j++) {
        to[i + j] = from[j];
    }
    to[i + j] = '\0';

    return to;
}

size_t strlen_(const char* str)
{
    size_t i = 0;
    while (str[i] != '\0') {
        ++i;
    }
    return i;
}

int strcmp_(const char* s1, const char* s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++, s2++;
    }

    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int to_int_(const char* str)
{
    int res = 0;
    while ((*str != 0) && (*str >= '0') && (*str <= '9')) {
        res = res * 10 + static_cast <int> (*str - '0');
        ++str;
    }

    return res;
}



MyString::MyString()
    : size_(0), max_size_(DEFAULT_SIZE), c_str_(new char [DEFAULT_SIZE])
{
    clear();
}

MyString::MyString(const MyString& str)         // copy constructor
    : size_(str.size_), max_size_(str.max_size_), c_str_(new char [str.max_size_])
{
    strcpy_(this->c_str_, str.c_str_);
}

MyString::MyString(MyString&& str) noexcept          // move constructor
{
    *this = std::move(str);
}

MyString::MyString(const char* str)
    : size_(strlen_(str)), max_size_(ROUND_SIZE(strlen_(str))), c_str_(new char [ROUND_SIZE(strlen_(str))])
{
    strcpy_(this->c_str_, str);
}

MyString::MyString(std::initializer_list<char>& il)
    : size_(il.size()), max_size_(ROUND_SIZE(il.size())), c_str_(new char [ROUND_SIZE(il.size())])
{
    strcpy_(this->c_str_, il.begin());
}

MyString::MyString(const std::string& str)
    : size_(str.size()), max_size_(str.max_size()), c_str_(new char [str.max_size()])
{
    strcpy_(this->c_str_, str.c_str());
}

MyString::MyString(const char* str, size_t count)
    : size_(count), max_size_(ROUND_SIZE(size_)), c_str_(new char [count])
{
    strncpy_(this->c_str_, str, count);
}

MyString::MyString(size_t count, char c)
    : size_(count), max_size_(ROUND_SIZE(size_)), c_str_(new char [count])
{
    strset_(c_str_, c, count);
}

MyString::MyString(unsigned int num)
    : size_(2 + 2 * sizeof(unsigned int)), max_size_(2 * DEFAULT_SIZE), c_str_(new char [2 * DEFAULT_SIZE])
{
    char c;
    unsigned int mask = 0xf0000000;
    strset_(c_str_, '\0', max_size_);
    for (int i = 0; i < 2 * sizeof(unsigned int); ++i) {
        c = (num & mask) >> (8 * sizeof(unsigned int) - 4 - 4 * i);
        c_str_[2 + i] = c + '0';
        mask >>= 4;
    }

    c_str_[0] = '0';
    c_str_[1] = 'x';
}


MyString::~MyString()
{
    delete [] c_str_;
    c_str_ = nullptr;
}

MyString& MyString::operator= (const char* str)
{
    if (this->max_size_ < strlen_(str)) {
        this->shrink_to_fit(ROUND_SIZE(strlen_(str)));
    }
    strcpy_(this->c_str_, str);
    this->size_ = strlen_(str);

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

MyString& MyString::operator= (MyString&& str)          //  move assigment
{
    if (this != &str) {
        delete [] this->c_str_;

        this->c_str_ = str.c_str_;
        this->size_ = str.size_;
        this->max_size_ = str.max_size_;

        str.c_str_ = nullptr;
        str.size_ = 0;
        str.max_size_ = 0;
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
    if (this->max_size_ <= this->size_ + strlen_(str)) {
        shrink_to_fit(ROUND_SIZE(this->size_ + strlen_(str)));
    }
    strcat_(this->c_str_, str);
    this->size_ += strlen_(str);

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
    strset_(c_str_, '\0', max_size_);
    size_ = 0;
}

void MyString::shrink_to_fit(size_t size)
{
    if (size <= this->size_) {
        return;
    }

    char * tmp = c_str_;
    c_str_ = nullptr;
    c_str_ = new char[ROUND_SIZE(size)];
    strcpy_(c_str_, tmp);
    delete [] tmp;
    tmp = nullptr;
    max_size_ = ROUND_SIZE(size);
}

char MyString::operator[] (Index i) const
{
    return (i < size_ ? c_str_[i] : static_cast<char>(-1));
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
    return max_size_;
}

bool MyString::empty() const
{
    return (size_ == 0);
}

MyString MyString::operator+ (const char* str_2)
{
    MyString res(*this);
    return (res += str_2);
}

MyString MyString::operator+ (const MyString& str_2)
{
    return (*this + str_2.c_str());
}

MyString MyString::operator+ (const std::string& str_2)
{
    return (*this + str_2.c_str());
}

bool MyString::operator== (const MyString& str) const
{
    if (this == &str) {
        return true;
    }

    return static_cast <bool> (strcmp_(this->c_str_, str.c_str()) == 0);
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
    return (is >> str.c_str_);
}


void MyString::insert(Index i_, const char* mid_str)
{
    if (i_ >= max_size_) {
        throw "Out of range";
    }

    if (size_ + strlen_(mid_str) >= max_size_) {
        shrink_to_fit(size_ + strlen_(mid_str) + 1);
    }


    for (int i = size_ + strlen_(mid_str) - 1; i >= i_ + strlen_(mid_str); --i) {
        c_str_[i] = c_str_[i - strlen_(mid_str)];
    }

    for (int i = i_, j = 0; i < i_ + strlen_(mid_str); ++i, ++j) {
        c_str_[i] = mid_str[j];
    }

    size_ += strlen_(mid_str);
    c_str_[size_] = '\0';
}

void MyString::insert(Index i, size_t count, char c)
{
    char* mid_str = new char [count + 1];
    insert(i, strset_(mid_str, c, count));
    delete [] mid_str;
}

void MyString::insert(Index i, char* mid_str, size_t count)
{
    strncpy_(mid_str, static_cast<const char*>(mid_str), count);
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
    return to_int_(c_str_);
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
    return strcmp_(this->c_str_, str.c_str_) > 0;
}

bool MyString::operator< (const MyString& str) const
{
    return strcmp_(this->c_str_, str.c_str_) < 0;
}

bool MyString::operator>= (const MyString& str) const
{
    return strcmp_(this->c_str_, str.c_str_) >= 0;
}

bool MyString::operator<= (const MyString& str) const
{
    return strcmp_(this->c_str_, str.c_str_) <= 0;
}



void MyString::append(size_t count, char c)
{
    char* tmp_str = new char [count + 1];
    strset_(tmp_str, c, count);
    tmp_str[count] = '\0';

    append(tmp_str, 0, count);

    delete [] tmp_str;
}

void MyString::append(const char* str)
{
    append(str, 0, strlen_(str));
}

void MyString::append(const char* str, Index i, size_t count)
{
    if (size_ + count > max_size_) {
        shrink_to_fit(size_ + count + 1);
    }
    strncpy_(this->c_str_ + size_, str + i, count);
    size_ += count;
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

void MyString::substr(char* result_buffer, Index i, size_t count) const
{
    if (i + count >= size_) {
        throw "Out of range";
    }

    strncpy_(result_buffer, c_str_ + i, count);
}

void MyString::substr(char* result_buffer, Index i) const
{
    if (i >= size_) {
        throw "Out of range";
    }

    strcpy_(result_buffer, c_str_ + i);
}


Index MyString::find(const char* substr) const
{
    return find(substr, 0);
}

Index MyString::find(const char* substr, Index i_) const
{
    char c;
    for (int i = i_; i <= size_ - strlen_(substr); ++i) {
        c = *(c_str_ + i + strlen_(substr));
        *(c_str_ + i + strlen_(substr)) = 0;
        if (!strcmp_(c_str_ + i, substr)) {
            *(c_str_ + i + strlen_(substr)) = 0;
            return i;
        }
        *(c_str_ + i + strlen_(substr)) = c;
    }

    return static_cast <Index> (-1);
}

Index MyString::find(const std::string& substr) const
{
    return find(substr.c_str(), 0);
}

Index MyString::find(const std::string& substr, Index i_) const
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

iterator MyString::begin()
{
    return iterator(&c_str_[0]);
}

iterator MyString::end()
{
    return iterator(&c_str_[size_]);
}

const_iterator MyString::cbegin()
{
    return const_iterator(&c_str_[0]);
}

const_iterator MyString::cend()
{
    return const_iterator(&c_str_[size_]);
}


// rbegin(), rend(), crbegin, crend()

reverse_iterator MyString::rbegin()
{
    return reverse_iterator(&c_str_[size_ - 1], c_str_);
}

reverse_iterator MyString::rend()
{
    return reverse_iterator(nullptr, c_str_);
}

const_reverse_iterator MyString::crbegin()
{
    return const_reverse_iterator(&c_str_[size_ - 1], c_str_);
}

const_reverse_iterator MyString::crend()
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
