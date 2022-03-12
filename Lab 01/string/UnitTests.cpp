#include <iostream>
#include <cassert>

#include "MyString.h"

void unit_tests();
void test_1(), test_2(), test_comparation(), test_insert(), test_erase(), test_to_int();;
void test_append(), test_replace(), test_find(), test_itterator(), test_substr();

int main()
{
   // unit_tests();

    MyString a ("1213");
    a.clear();

    return 0;
}

void unit_tests()
{
    test_1();
    test_2();

    test_comparation();
    test_insert();
    test_erase();
    test_append();
    test_replace();
    test_find();

    test_substr();
    test_to_int();

    // dop:
    test_itterator();

    std::cout << "All test completed\n";
}



void test_1()
{
    MyString a, b, c, d;

    a = "123", b = "4567", c = "89";
    d = a + b + c + d;

    assert(!std::strcmp(a.c_str(), "123"));
    assert(!std::strcmp(b.c_str(), "4567"));
    assert(!std::strcmp(c.c_str(), "89"));
    assert(!std::strcmp(d.c_str(), "123456789"));
    assert(a.size() == 3);
    assert(b.size() == 4);
    assert(c.size() == 2);
    assert(d.size() == 9);
}

void test_2()
{
    MyString a("bbbbbbbbbbbbbbbbbbbbbbbb");
    MyString b(a);
    MyString c(""), d, e;

    e += "1";
    e += a;
    c += e;
    e = a;
    a = b;
    b += "213";

    d = a + b + c;

    assert(!std::strcmp(a.c_str(), "bbbbbbbbbbbbbbbbbbbbbbbb"));
    assert(!std::strcmp(b.c_str(), "bbbbbbbbbbbbbbbbbbbbbbbb213"));
    assert(!std::strcmp(c.c_str(), "1bbbbbbbbbbbbbbbbbbbbbbbb"));
    assert(!std::strcmp(d.c_str(), "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb2131bbbbbbbbbbbbbbbbbbbbbbbb"));
    assert(!std::strcmp(e.c_str(), "bbbbbbbbbbbbbbbbbbbbbbbb"));

}

void test_comparation()
{
    MyString a, b, c;

    a = "...............................................................123";
    b = "...............................................................12";
    c = "...............................................................12345";

    assert (a > b);
    assert (a < c);
    assert (b < a);
    assert (b < c);
    assert (c > a);
    assert (c > b);

    assert (a >= b);
    assert (a <= c);
    assert (b <= a);
    assert (b <= c);
    assert (c >= a);
    assert (c >= b);

    a = b = c = "";

    assert (a.empty());
    assert (b.empty());
    assert (c.empty());
}

void test_insert()
{
    MyString a         ("11111111111111111111111111111122222222222222222222"), b(a), c(a);
    MyString a_expected("111111111111111111111111111111oooooooooo22222222222222222222");
    MyString b_expected("oooooooooo11111111111111111111111111111122222222222222222222");
    MyString c_expected("11111111111111111111111111111122222222222222222222oooooooooo");

    a.insert(30, 10, 'o');
    b.insert(0, 10, 'o');
    c.insert(50, 10, 'o');

    assert(a == a_expected);
    assert(b == b_expected);
    assert(c == c_expected);
}

void test_erase()
{
    MyString a("0123456789..................................................");
    MyString b("....................0123456789..............................");
    MyString c("..................................................0123456789");
    MyString expected("..................................................");

    a.erase(0, 10);
    b.erase(20, 10);
    c.erase(50, 10);

    assert(a == expected);
    assert(b == expected);
    assert(c == expected);

    MyString Andrey_Yuryevich ("Laboratory work is not credited");
    MyString I_Expect         ("Laboratory work is credited");

    Andrey_Yuryevich.erase(19, 4);
    assert (Andrey_Yuryevich == I_Expect);
}

void test_append()
{
    MyString a("******************************");
    MyString a_expected("******************************..............................llllllllllllllllllllllllllllll");
    MyString empty;

    a.append("..............................");
    a.append("llllllllllllllllllllllllllllll");

    assert(a == a_expected);
    a.clear();
    assert(a == empty);
}

void test_replace()
{
    MyString a("..............................REPLACE_ME....................");
    MyString b("REPLACE_ME..................................................");
    MyString c("..................................................REPLACE_ME");

    MyString a_expected("..............................NEW_STRING....................");
    MyString b_expected("NEW_STRING..................................................");
    MyString c_expected("..................................................NEW_STRING");

    a.replace(30, 10, "NEW_STRING");
    b.replace(0, 10, "NEW_STRING");
    c.replace(50, 10, "NEW_STRING");

    assert(a == a_expected);
    assert(b == b_expected);
    assert(c == c_expected);
}

void test_find()
{
    MyString a("0123456789012345678901234567890123456789___FIND_ME________");

    assert(a.find("FIND_ME") == 43);
}

void test_itterator()
{
    MyString a("HELLO WORLD");
    MyString res_1, res_1_correct("H-E-L-L-O- -W-O-R-L-D-");
    MyString res_2, res_2_correct("D-L-R-O-W- -O-L-L-E-H-");

    for (auto i = a.begin(); i != a.end(); ++i) {
        res_1.append(1, *i);
        res_1 += "-";
    }

    for (auto i = a.rbegin(); i != a.rend(); ++i) {
        res_2.append(1, *i);
        res_2 += "-";
    }

    //std::cout << res_1 << std::endl;
    //std::cout << res_2;

    assert(res_1 == res_1_correct);
    assert(res_2 == res_2_correct);
}

void test_substr()
{
    MyString a ("______________________________0123456789__________________________");
    MyString substr, expected_substr ("0123456789");

    substr = a.substr(30, 10);
    assert (substr == expected_substr);
}

void test_to_int()
{
    MyString a ("987654321");
    assert (a.to_int() == 987654321);
}