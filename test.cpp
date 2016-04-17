
#include <stdlib.h>
#include <stdio.h>
#include <tuple>
#include <string>
#include <typeinfo>
#include <assert.h>
#include <iostream>

#if defined (__GNUC__)
#include <cxxabi.h>
#else
#include <stdint.h>
#endif

using namespace std;

//返回类型名称
template <typename T>
class check_typeName
{
public:
	static string check()
	{
		string ss = "";
#   if defined(__GNUC__)
		char* real_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
		std::cout << " "<< real_name<<endl;
		ss = real_name;
		if(real_name)
			free(real_name);
#   else
		std::cout << typeid(T).name();
		ss = typeid(T).name();
#   endif
		return ss;
	}
};

#define CHECK_TYPE__(OPT) \
	template <typename T> \
class check_typeName<T OPT> : check_typeName<T> \
{ \
public: \
	static string check() { std::cout << " "#OPT; return check_typeName<T>::check()+" "#OPT;} \
};

CHECK_TYPE__(const);
CHECK_TYPE__(volatile);
CHECK_TYPE__(const volatile);
CHECK_TYPE__(&);
CHECK_TYPE__(&&);
CHECK_TYPE__(*);


//泛型：操作元组中的不同类型的元素
//iIndex：元组中第几个元素
//t:此元素
//sProtoName:protobuf协议结构名
template<typename T>
void DealElemOfTuple( T& t, int iIndex=0, const char* sProtoName="")
{
	//获取类型名
	string sTypeName = check_typeName<T>().check();
	printf("I am template typename=[%s] sizeof=[%d] iIndex=[%d]\n", sTypeName.c_str(), sizeof(T), iIndex);

	//处理tuple复合类型
	if (string::npos != sTypeName.find("tuple"))
	{
		printf("i am tuple \n");
		//tuple元素深度遍历
		DepthTraversalTuple(t);
	}
	else
	{
		//不允许传void*,数组等
		std::get<0>(t); //编译期就把错误暴露
		printf("error:i am unkown T=[%s] \n",sTypeName.c_str());
		assert(0);
	}	

	return;
}

//特化版本：
//string
#define  PRINT_TYPENAME(OPT) \
	string sTypeName = check_typeName<decltype(t)>().check(); \
	printf("I am special typename=[%s] opt="#OPT" sizeof=[%d] iIndex=[%d]\n", sTypeName.c_str(), sizeof(t), iIndex); 

#define DealElemOfTuple_STRING__(OPT) \
	template<>	\
	void DealElemOfTuple<OPT string >(OPT string & t, int iIndex, const char* sProtoName)	\
{ \
	PRINT_TYPENAME(OPT);\
	printf("i am string [%s]\n",t.c_str()); \
	return; \
}; \

DealElemOfTuple_STRING__();
DealElemOfTuple_STRING__(const);

//char*
#define DealElemOfTuple_CHARPTR__(OPT,OPTPTR) \
	template<>	\
	void DealElemOfTuple< OPT char * OPTPTR>( OPT char * OPTPTR & t, int iIndex, const char* sProtoName)	\
{ \
	PRINT_TYPENAME(OPT);\
	printf("\toptptr="#OPTPTR"\n");\
	printf("i am char* [%s]\n",t); \
	return; \
}; \

DealElemOfTuple_CHARPTR__(,);
DealElemOfTuple_CHARPTR__(const,);
DealElemOfTuple_CHARPTR__(signed,);
DealElemOfTuple_CHARPTR__(unsigned,);
DealElemOfTuple_CHARPTR__(const signed,);
DealElemOfTuple_CHARPTR__(const unsigned,);
DealElemOfTuple_CHARPTR__(,const);
DealElemOfTuple_CHARPTR__(const,const);
DealElemOfTuple_CHARPTR__(signed,const);
DealElemOfTuple_CHARPTR__(unsigned,const);
DealElemOfTuple_CHARPTR__(const signed,const);
DealElemOfTuple_CHARPTR__(const unsigned,const);

//char
#define DealElemOfTuple_CHAR__(OPT) \
	template<>	\
	void DealElemOfTuple< OPT char>( OPT char & t, int iIndex, const char* sProtoName)	\
{ \
	PRINT_TYPENAME(OPT);\
	printf("i am char [%c]\n",t); \
	return; \
}; \

DealElemOfTuple_CHAR__();
DealElemOfTuple_CHAR__(const);
DealElemOfTuple_CHAR__(signed);
DealElemOfTuple_CHAR__(unsigned);
DealElemOfTuple_CHAR__(const signed);
DealElemOfTuple_CHAR__(const unsigned);

//double
#define DealElemOfTuple_DOUBLE__(OPT) \
	template<>	\
	void DealElemOfTuple< OPT double>( OPT double & t, int iIndex, const char* sProtoName)	\
{ \
	PRINT_TYPENAME(OPT);\
	if(sizeof(t)<=8)\
	printf("i am double [%lf]\n",t); \
	else\
	printf("i am double [%llf]\n",t); \
	return; \
}; \

DealElemOfTuple_DOUBLE__();
DealElemOfTuple_DOUBLE__(const);
DealElemOfTuple_DOUBLE__(long);
DealElemOfTuple_DOUBLE__(const long);

//float
#define DealElemOfTuple_FLOAT__(OPT) \
	template<>	\
	void DealElemOfTuple< OPT float>( OPT float & t, int iIndex, const char* sProtoName)	\
{ \
	PRINT_TYPENAME(OPT);\
	printf("i am float [%f]\n",t); \
	return; \
}; \

DealElemOfTuple_FLOAT__();
DealElemOfTuple_FLOAT__(const);

//long
#define DealElemOfTuple_LONG__(OPT) \
	template<>	\
	void DealElemOfTuple< OPT long>( OPT long & t, int iIndex, const char* sProtoName)	\
{ \
	PRINT_TYPENAME(OPT);\
	printf("i am long [%lld]\n",t); \
	return; \
}; \

DealElemOfTuple_LONG__();
DealElemOfTuple_LONG__(const);
DealElemOfTuple_LONG__(long);
//DealElemOfTuple_LONG__(signed);
DealElemOfTuple_LONG__(unsigned);
DealElemOfTuple_LONG__(const long);
//DealElemOfTuple_LONG__(const signed);
DealElemOfTuple_LONG__(const unsigned);
//DealElemOfTuple_LONG__(signed long);
DealElemOfTuple_LONG__(unsigned long);
//DealElemOfTuple_LONG__(const signed long);
DealElemOfTuple_LONG__(const unsigned long);

//int
#define DealElemOfTuple_INT__(OPT) \
	template<>	\
	void DealElemOfTuple< OPT int>( OPT int & t, int iIndex, const char* sProtoName)	\
{ \
	PRINT_TYPENAME(OPT);\
	printf("i am int [%d]\n",t); \
	return; \
}; \

DealElemOfTuple_INT__();
DealElemOfTuple_INT__(const);
//DealElemOfTuple_INT__(long);
//DealElemOfTuple_INT__(short);
//DealElemOfTuple_INT__(signed);
DealElemOfTuple_INT__(unsigned);
//DealElemOfTuple_INT__(const long);
//DealElemOfTuple_INT__(const short);
//DealElemOfTuple_INT__(const signed);
DealElemOfTuple_INT__(const unsigned);
//DealElemOfTuple_INT__(signed long);
//DealElemOfTuple_INT__(unsigned long);
//DealElemOfTuple_INT__(signed short);
//DealElemOfTuple_INT__(unsigned short);
//DealElemOfTuple_INT__(const signed long);
//DealElemOfTuple_INT__(const unsigned long);
//DealElemOfTuple_INT__(const signed short);
//DealElemOfTuple_INT__(const unsigned short);

//short
#define DealElemOfTuple_SHORT__(OPT) \
	template<>	\
	void DealElemOfTuple< OPT short>( OPT short & t, int iIndex, const char* sProtoName)	\
{ \
	PRINT_TYPENAME(OPT);\
	printf("i am short [%d]\n",t); \
	return; \
}; \

DealElemOfTuple_SHORT__();
DealElemOfTuple_SHORT__(const);
//DealElemOfTuple_SHORT__(signed);
DealElemOfTuple_SHORT__(unsigned);
//DealElemOfTuple_SHORT__(const signed);
DealElemOfTuple_SHORT__(const unsigned);

//bool
#define DealElemOfTuple_BOOL__(OPT) \
	template<>	\
	void DealElemOfTuple< OPT bool>( OPT bool & t, int iIndex, const char* sProtoName)	\
{ \
	PRINT_TYPENAME(OPT);\
	printf("i am bool [%d]\n",t); \
	return; \
}; \

DealElemOfTuple_BOOL__();
DealElemOfTuple_BOOL__(const);

//tuple元素广度遍历
//泛化：递归调用
template<class Tuple, std::size_t N>
class TupleBreadthTraversal {
public:
	static void breadthTraversal(const Tuple& t)
	{
		TupleBreadthTraversal<Tuple, N - 1>::breadthTraversal(t);
		DealElemOfTuple(std::get<N - 1>(t), N-1,"");
	}
};

//特例：递归出口
template<class Tuple>
class TupleBreadthTraversal<Tuple, 1>{
public:
	static void breadthTraversal(const Tuple& t)
	{
		DealElemOfTuple(std::get<0>(t), 0,"");
	}
};

// tuple之遍历Args
template<class... Args>
void DepthTraversalTuple(std::tuple<Args... >& t)
{
	std::cout << "tuple:(\n";
	TupleBreadthTraversal<decltype(t), sizeof...(Args)>::breadthTraversal(t);
	std::cout << ")\n";
}

// template<typename T>
// void DepthTraversalTuple(T& t)
// {
// 	std::cout << "error:" << check_typeName<T>::check() <<endl;
// 	assert(0);
// }


int testDealElemOfTuple()
{
	printf("~~~~~~~~~~~~~~~~~~~~~~~~\n");

	//bool
	bool b=false;
	DealElemOfTuple(b);
	const bool cb=false;
	DealElemOfTuple(cb);

	//char
	char c='a';
	DealElemOfTuple(c);

	const char cc='b';
	DealElemOfTuple(cc);

	signed char sc='c';
	DealElemOfTuple(sc);

	unsigned char uc='u';
	DealElemOfTuple(uc);

	const signed char csc='s';
	DealElemOfTuple(csc);

	const unsigned char cuc='u';
	DealElemOfTuple(cuc);

	//int
	short int shi = 123;
	DealElemOfTuple(shi);

	signed short int sshi = 123;
	DealElemOfTuple(sshi);
	const signed short int csshi = 123;
	DealElemOfTuple(csshi);

	unsigned short int ushi = 123;
	DealElemOfTuple(ushi);

	const short int cshi = 123;
	DealElemOfTuple(cshi);

	const unsigned short int cushi = 123;
	DealElemOfTuple(cushi);

	int i = 1;
	DealElemOfTuple(i);

	const int ci = 1;
	DealElemOfTuple(ci);
	signed int si = 1;
	DealElemOfTuple(si);
	const signed int csi = 1;
	DealElemOfTuple(csi);
	unsigned int ui = 1;
	DealElemOfTuple(ui);

	const unsigned int cui = 1;
	DealElemOfTuple(cui);

	long int li = 123;
	DealElemOfTuple(li);
	signed long int sli = 123;
	DealElemOfTuple(sli);
	unsigned long int uli = 123;
	DealElemOfTuple(uli);

	const long int cli = 123;
	DealElemOfTuple(cshi);
	const signed long int csli = 123;
	DealElemOfTuple(csli);
	const unsigned long int culi = 123;
	DealElemOfTuple(culi);

	//long
	long l = 123;
	DealElemOfTuple(l);
	const long cl = 123;
	DealElemOfTuple(cl);
	signed long sl=123;
	DealElemOfTuple(sl);
	unsigned long ul=123;
	DealElemOfTuple(ul);
	const signed long csl=123;
	DealElemOfTuple(csl);
	const unsigned long cul=123;
	DealElemOfTuple(cul);
	long long ll = 123; //__int64
	DealElemOfTuple(ll);
	const long long cll = 123; //__int64
	DealElemOfTuple(cll);
	signed long long sll=123; //unsigned __int64
	DealElemOfTuple(sll);
	unsigned long long ull=123; //unsigned __int64
	DealElemOfTuple(ull);
	const signed long long csll=123; //unsigned __int64
	DealElemOfTuple(csll);
	const unsigned long long cull=123; //unsigned __int64
	DealElemOfTuple(cull);

	//double
	double d = 123;
	DealElemOfTuple(d);

	long double ld = 123;
	DealElemOfTuple(ld);
	const double cd = 123;
	DealElemOfTuple(cd);

	const long double cld = 123;
	DealElemOfTuple(cld);

	//float
	float f = 123;
	DealElemOfTuple(f);

	const float cf = 123;
	DealElemOfTuple(cf);

	int32_t i32=32;	//int
	DealElemOfTuple(i32);

	uint32_t ui32=32; //unsigned int
	DealElemOfTuple(ui32);

	int64_t i64=64;
	DealElemOfTuple(i64);

	uint64_t ui64=64;
	DealElemOfTuple(ui64);


	string s = "iamstring";
	DealElemOfTuple(s);
	const string cs = "iamconststring";
	DealElemOfTuple(cs);

	char * sz = "__char*";
	DealElemOfTuple(sz);
	const char * ss = "const__char*";
	DealElemOfTuple(ss);
	signed char ssz[]="__signed char *";
	signed char* sssz = (signed char*)ssz;
	DealElemOfTuple(sssz);
	unsigned char * uss=(unsigned char *) ssz;
	DealElemOfTuple(uss);
	const signed char* csssz = (const signed char*)ssz;
	DealElemOfTuple(csssz);
	const unsigned char * cuss=(const unsigned char *) ssz;
	DealElemOfTuple(cuss);
	{
		char * const sz = "__char*const__";
		DealElemOfTuple(sz);
		const char * const  ss = "const__char*__const";
		DealElemOfTuple(ss);
		signed char ssz[]="__signed char *";
		signed char* const sssz = (signed char*)ssz;
		DealElemOfTuple(sssz);
		unsigned char * const uss=(unsigned char *) ssz;
		DealElemOfTuple(uss);
		const signed char* const csssz = (const signed char*)ssz;
		DealElemOfTuple(csssz);
		const unsigned char * const cuss=(const unsigned char *) ssz;
		DealElemOfTuple(cuss);
	}
	
	char szz[100]="11111ss1111";
	char* szzc=(char*)szz;
	DealElemOfTuple(szzc);
	const char csz[100]="11111ss1111";
	const char* cszc=(const char*)csz;
	DealElemOfTuple(cszc);

	auto t4 = make_tuple(1,2,3,4);
	DealElemOfTuple(t4);


	return 0;
}

void testTraversalTuple()
{
	auto t2 = make_tuple("ss..6_3_1",632,633,634);
	auto t1 = make_tuple("str_6_1",string("sstring_6_2"),t2);

	auto t3 = make_tuple(1,2,3,4,t1);
	DepthTraversalTuple(t3);
}

int main()
{
	testDealElemOfTuple();
	testTraversalTuple();

	return 0;
}
