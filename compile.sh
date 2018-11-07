gcc \
	--shared \
	-fpic \
	-I/usr/include/lua5.1 \
	-llua5.1 \
	-o COMPLEX.so \
	lcomplex.cpp
