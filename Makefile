# NOTE: This Makefile is intended to be used with the minebea-intec build rules
# for a simple host compile use compile.sh

LUALIBS := COMPLEX.so

objects-COMPLEX.so := lcomplex.o

include ../../build.rules
