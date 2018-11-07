#!/usr/bin/lua
local COMPLEX=require"COMPLEX"
local a=COMPLEX.new(1,2)
local b=COMPLEX.new(3,-5)
print(a,b,a+b,a-b)

c1=COMPLEX.new(1.2,3.4)
c2=COMPLEX.new(5.6,7.8)
print("add",c1+c2)
print("sub",c1-c2)
print("mul",c1*c2)
print("div",c1/c2)
print("abs",c1:abs())
print("abs",c2:abs())
