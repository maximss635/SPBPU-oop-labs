#include "MyString.h"

class PyMyString : public MyString {
public:

#ifdef SWIG 
%extend 
{ 
    char __getitem__(int index) { return $self->at(index); } 
//    void __setitem__(int index, char val) { c_str_[index] = val; } 
    char* __str__() { return (char*)$self->c_str(); } 
} 
#endif 

}