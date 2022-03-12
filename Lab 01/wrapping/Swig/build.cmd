@python setup.py build

@md tmp
@move MyString_wrap.cpp tmp\
@md out
@move build\lib.win32-3.7\_MyString.cp37-win32.pyd out\
@move MyString.py out\