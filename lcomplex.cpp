//------------------------------------------------------------
// a sample library to show, how to embed a C-struct into lua
//------------------------------------------------------------

//
// required include (includes lua.h,...)
//
#include "lua.hpp"

//
// the name of the metatable holding the interface to S_COMPLEX
// (NOTE: should be unique)
//
#define LUA_COMPLEX "COMPLEX-27A6AB09-6B67-4D08-BF4C-E054141C07DC"

//
// our C structure
//
struct S_COMPLEX
{
	double re;
	double im;
};


//
// verify, object idx on stack is a S_COMPLEX and return pointer to it
// NOTE: must be const* because value is "immutable"
// see https://en.wikipedia.org/wiki/Immutable_object
//
static const S_COMPLEX*lua_checkcomplex(lua_State*L,int idx)
{
	return (const S_COMPLEX*)luaL_checkudata(L,idx,LUA_COMPLEX);
}

//
// push a S_COMPLEX on stack
//
static int lua_pushcomplex(lua_State*L,const S_COMPLEX&complex)
{
	// allocate memory in lua state
	S_COMPLEX*pComplex=(S_COMPLEX*)lua_newuserdata(L,sizeof(S_COMPLEX));
	// assign metatable
	luaL_getmetatable(L,LUA_COMPLEX);
	lua_setmetatable(L,-2);
	// assign value
	*pComplex=complex;
	// return " 1 element push on stack"
	return 1;
}

//
// create a S_COMPLEX from two doubles
//
// Example:
// c1=COMPLEX.new(1.2,-3.4)
//
static int lua_complex_new(lua_State*L)
{
	double re=luaL_checknumber(L,1);	// first argument must be a number
	double im=luaL_checknumber(L,2);	// second argument must be a number
	S_COMPLEX complex={re,im};			// create a new complex with result
	return lua_pushcomplex(L,complex);	// push a new complex number on stack
}

//
// convert a complex to string
//
// Example:
// print(tostring(c1))
// > {1.2,-3.4}
//
static int lua_complex_tostring(lua_State*L)
{
	const S_COMPLEX*complex=lua_checkcomplex(L,1);	// first argument must be a S_COMPLEX
	lua_pushfstring(L,"{%f,%f}",complex->re,complex->im);	// push formatted string on stack
	return 1;
}

//
// add two complex values
//
// Example:
// c1=COMPLEX.new(1.2,3.4)
// c2=COMPLEX.new(5.6,7.8)
// c3=c1+c2
// print(c3)
// >{6.8,11.2}
//
static int lua_complex_add(lua_State*L)
{
	const S_COMPLEX*a=lua_checkcomplex(L,1);	// first argument must be a complex
	const S_COMPLEX*b=lua_checkcomplex(L,2);	// second argument must be a complex
	S_COMPLEX sum= {a->re+b->re,a->im+b->im};	// create a new complex with result
	return lua_pushcomplex(L,sum);				// push result on stack and return
}

//
// sub two complex values
//
// Example:
// c1=COMPLEX.new(1.2,3.4)
// c2=COMPLEX.new(5.6,7.8)
// c3=c1-c2
// print(c3)
// >{-4.4,-4.4}
//
static int lua_complex_sub(lua_State*L)
{
	const S_COMPLEX*a=lua_checkcomplex(L,1);	// first argument must be a complex
	const S_COMPLEX*b=lua_checkcomplex(L,2);	// second argument must be a complex
	S_COMPLEX dif= {a->re-b->re,a->im-b->im};	// create a new complex with result
	return lua_pushcomplex(L,dif);				// push result on stack and return
}

//
// multiply two complex values
//
// Example:
// c1=COMPLEX.new(1.2,3.4)
// c2=COMPLEX.new(5.6,7.8)
// c3=c1*c2
// print(c3)
// >{-19.8,28.4}
//
static int lua_complex_mul(lua_State*L)
{
	const S_COMPLEX*z1=lua_checkcomplex(L,1);	// first argument must be a complex
	const S_COMPLEX*z2=lua_checkcomplex(L,2);	// second argument must be a complex
	// get components
	double a=z1->re;
	double b=z1->im;
	double c=z2->re;
	double d=z2->im;
	// SEE https://de.wikipedia.org/wiki/Komplexe_Zahl
	// SEE https://en.wikipedia.org/wiki/Complex_number
	S_COMPLEX prod= {a*c-b*d,a*d+b*c};			// create a new complex with result
	return lua_pushcomplex(L,prod);				// push result on stack and return
}

//
// divide two complex values
//
// Example:
// c1=COMPLEX.new(1.2,3.4)
// c2=COMPLEX.new(5.6,7.8)
// c3=c1/c2
// print(c3)
// >{0.36052060737527,0.10498915401302}
//
static int lua_complex_div(lua_State*L)
{
	const S_COMPLEX*z1=lua_checkcomplex(L,1);	// first argument must be a complex
	const S_COMPLEX*z2=lua_checkcomplex(L,2);	// second argument must be a complex
	// get components
	double a=z1->re;
	double b=z1->im;
	double c=z2->re;
	double d=z2->im;
	double ccdd=c*c+d*d;
	S_COMPLEX quot= {(a*c+b*d)/ccdd,(b*c-a*d)/ccdd};		// create a new complex with result
	return lua_pushcomplex(L,quot);							// push result on stack and return
}

//
// absolute value of complex
//
// Example:
// c2=COMPLEX.new(5.6,7.8)
// print(c2:abs())
// >92.2
//
static int lua_complex_abs(lua_State*L)
{
	const S_COMPLEX*z1=lua_checkcomplex(L,1);	// first argument must be a complex
	// get components
	double a=z1->re;
	double b=z1->im;
	lua_pushnumber(L,a*a+b*b);	// push abs value
	return 1;
}

//
// this is to clone objects via lanes/linda
//
static int lua_complex_lanesclone( lua_State* L)
{
	switch( lua_gettop( L))
	{
		case 0:
		lua_pushinteger( L, sizeof( struct S_COMPLEX));
		return 1;

		case 2:
		{
			struct S_COMPLEX* self = (S_COMPLEX*)lua_touserdata( L, 1);
			struct S_COMPLEX* from = (S_COMPLEX*)lua_touserdata( L, 2);
			*self = *from;
			return 0;
		}

		default:
		(void) luaL_error( L, "Lanes called clonable_lanesclone with unexpected parameters");
	}
	return 0;
}


static int lua_complex_gc( lua_State* L)
{
	struct S_COMPLEX* self = (struct S_COMPLEX*) lua_touserdata( L, 1);
	return 0;
}

//
// list functions to be set into the metatable of S_COMPLEX
//
static const struct luaL_Reg complex_mt[]=
{
	{"tostring",lua_complex_tostring},
	{"__tostring",lua_complex_tostring},
	{"__add",lua_complex_add},
	{"__sub",lua_complex_sub},
	{"__mul",lua_complex_mul},
	{"__div",lua_complex_div},
	{"abs",lua_complex_abs},
	{"__lanesclone",lua_complex_lanesclone},
	{"__gc",lua_complex_gc},
	{NULL,NULL},
};

//
// list functions to be set into the module
//
static const struct luaL_Reg complexlib[]=
{
	{"new",lua_complex_new},
	{NULL,NULL},
};

//
// this is called from lua interpreter on loading the module
//

extern "C" int luaopen_COMPLEX(lua_State*L)
{
	//
	// register functions to be exported by the module
	//
	luaL_register(L,"COMPLEX",complexlib);	// register the module itself as global
	//
	// create a metatable to be used with S_COMPLEX objects
	//
	if(luaL_newmetatable(L,LUA_COMPLEX))	// -1=mt'COMPLEX' create a new metatable
	{
		luaL_register(L,NULL,complex_mt);		// register with complex_mt for this metatable
		lua_pushvalue(L,-1);				// -1=mt'COMPLEX' -2=mt'COMPLEX'
		lua_setfield(L,-2,"__index");		// set index to itself
	}
	lua_setfield(L, -2, "__" LUA_COMPLEX "MT");                    // M

	//
	// return 1 element on the stack
	//
	return 1;
}

