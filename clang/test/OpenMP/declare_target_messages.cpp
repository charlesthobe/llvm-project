// RUN: %clang_cc1 -triple x86_64-apple-macos10.7.0 -verify=expected,omp45 -fopenmp -fopenmp-version=45 -fnoopenmp-use-tls -ferror-limit 100 -o - %s
// RUN: %clang_cc1 -triple x86_64-apple-macos10.7.0 -verify=expected,omp5,host5 -fopenmp -fopenmp-targets=x86_64-apple-macos10.7.0 -fnoopenmp-use-tls -ferror-limit 100 -o - %s
// RUN: %clang_cc1 -triple x86_64-apple-macos10.7.0 -verify=expected,omp5,dev5 -fopenmp -fopenmp-is-device -fopenmp-targets=x86_64-apple-macos10.7.0 -aux-triple x86_64-apple-macos10.7.0 -fnoopenmp-use-tls -ferror-limit 100 -o - %s

// RUN: %clang_cc1 -triple x86_64-apple-macos10.7.0 -verify=expected,omp5,host5 -fopenmp-simd -fopenmp-targets=x86_64-apple-macos10.7.0 -fnoopenmp-use-tls -ferror-limit 100 -o - %s
// RUN: %clang_cc1 -triple x86_64-apple-macos10.7.0 -verify=expected,omp5,host5 -fopenmp-simd -fopenmp-is-device -fopenmp-targets=x86_64-apple-macos10.7.0 -fnoopenmp-use-tls -ferror-limit 100 -o - %s
// RUN: %clang_cc1 -triple x86_64-apple-macos10.7.0 -verify=expected,omp45 -fopenmp-version=45 -fopenmp-simd -fnoopenmp-use-tls -ferror-limit 100 -o - %s
// RUN: %clang_cc1 -triple x86_64-apple-macos10.7.0 -verify=expected,omp51 -fopenmp -fopenmp-version=51 -fnoopenmp-use-tls -ferror-limit 100 -o - %s
// RUN: %clang_cc1 -triple x86_64-apple-macos10.7.0 -verify=expected,omp51 -fopenmp -fopenmp-version=51 -fnoopenmp-use-tls -ferror-limit 100 -DTESTEND=1 -o - %s
// RUN: %clang_cc1 -triple x86_64-apple-macos10.7.0 -verify=expected,omp51 -fopenmp -fopenmp-version=51 -fnoopenmp-use-tls -ferror-limit 100 -I%S/Inputs -DTESTENDINC=1 -o - %s
// RUN: %clang_cc1 -triple x86_64-apple-macos10.7.0 -verify=expected,omp51 -fopenmp-version=51 -fopenmp-simd -fnoopenmp-use-tls -ferror-limit 100 -o - %s
// RUN: %clang_cc1 -triple x86_64-apple-macos10.7.0 -verify=expected,omp51 -fopenmp-version=51 -fopenmp-simd -fnoopenmp-use-tls -ferror-limit 100 -o - %s

// RUN: %clang_cc1 -triple x86_64-apple-macos10.7.0 -verify=expected,omp52 -fopenmp -fopenmp-version=52 -DVERBOSE_MODE=1 -fnoopenmp-use-tls -ferror-limit 100 -o - %s

// RUN: %clang_cc1 -triple x86_64-apple-macos10.7.0 -verify=expected,omp5 -fopenmp -fnoopenmp-use-tls -ferror-limit 100 -o - %s
#pragma omp end declare target // expected-error {{unexpected OpenMP directive '#pragma omp end declare target'}}

int a, b, z; // omp5-error {{variable captured in declare target region must appear in a to clause}} // omp51-error {{variable captured in declare target region must appear in a to clause}} omp52-error {{variable captured in declare target region must appear in a to clause}}
__thread int t; // expected-note {{defined as threadprivate or thread local}}

#pragma omp declare target . // expected-error {{expected '(' after 'declare target'}}

#pragma omp declare target
void f();
#pragma omp end declare target shared(a) // expected-warning {{extra tokens at the end of '#pragma omp end declare target' are ignored}}

#pragma omp declare target map(a) // omp45-error {{expected at least one 'to' or 'link' clause}} omp5-error {{expected at least one 'to' or 'link' clause}} omp51-error {{expected at least one 'to', 'link' or 'indirect' clause}} omp45-error {{unexpected 'map' clause, only 'to' or 'link' clauses expected}} omp5-error {{unexpected 'map' clause, only 'to', 'link' or 'device_type' clauses expected}} omp51-error {{unexpected 'map' clause, only 'to', 'link', 'device_type' or 'indirect' clauses expected}} omp52-error {{unexpected 'map' clause, only 'enter', 'link', 'device_type' or 'indirect' clauses expected}} omp52-error {{expected at least one 'enter', 'link' or 'indirect' clause}}

#pragma omp declare target to(foo1) // omp45-error {{use of undeclared identifier 'foo1'}} omp5-error {{use of undeclared identifier 'foo1'}} omp51-error {{use of undeclared identifier 'foo1'}} omp52-error {{unexpected 'to' clause, use 'enter' instead}} omp52-error {{expected at least one 'enter', 'link' or 'indirect' clause}}

#pragma omp declare target link(foo2) // expected-error {{use of undeclared identifier 'foo2'}}

#pragma omp declare target to(f) device_type(host) // omp45-error {{unexpected 'device_type' clause, only 'to' or 'link' clauses expected}} dev5-note {{marked as 'device_type(host)' here}} omp52-error {{unexpected 'to' clause, use 'enter' instead}} omp52-error {{expected at least one 'enter', 'link' or 'indirect' clause}}

void q();
#pragma omp declare target to(q) device_type(any) device_type(any) device_type(host) // omp45-error {{unexpected 'device_type' clause, only 'to' or 'link' clauses expected}} omp5-warning {{more than one 'device_type' clause is specified}} // omp51-warning {{more than one 'device_type' clause is specified}} omp52-error {{unexpected 'to' clause, use 'enter' instead}} omp52-error {{expected at least one 'enter', 'link' or 'indirect' clause}}

#if _OPENMP == 202011
// omp51-error@+1 {{directive '#pragma omp declare target' cannot contain more than one 'indirect' clause}}
#pragma omp declare target to(q) indirect(true) indirect(false)

int xxx; //expected-note {{declared here}}
// omp51-error@+2 {{expression is not an integral constant expression}}
// omp51-note@+1 {{read of non-const variable 'xxx' is not allowed in a constant expression}}
#pragma omp declare target to(q) indirect(xxx)

constexpr bool fz() {return true;}
// omp51-error@+1 {{unexpected 'to' clause, only 'device_type', 'indirect' clauses expected}}
#pragma omp begin declare target to(q) indirect(fz()) device_type(nohost)
#pragma omp end declare target

// omp51-error@+1 {{unexpected 'to' clause, only 'device_type', 'indirect' clauses expected}}
#pragma omp begin declare target indirect to(xxx)
void bar();
#pragma omp end declare target

// omp51-error@+2 {{unexpected 'tofrom' clause, only 'to', 'link', 'device_type' or 'indirect' clauses expected}}
// omp51-error@+1 {{expected at least one 'to', 'link' or 'indirect' clause}}
#pragma omp declare target tofrom(xxx)

// omp51-error@+1 {{only 'device_type(any)' clause is allowed with indirect clause}}
#pragma omp begin declare target device_type(host) indirect
void bar();
#pragma omp end declare target
#endif  // _OPENMP

void c();

void func() {} // expected-note {{'func' defined here}}

#pragma omp declare target link(func) allocate(a) // expected-error {{function name is not allowed in 'link' clause}} omp45-error {{unexpected 'allocate' clause, only 'to' or 'link' clauses expected}} omp5-error {{unexpected 'allocate' clause, only 'to', 'link' or 'device_type' clauses expected}} omp51-error {{unexpected 'allocate' clause, only 'to', 'link', 'device_type' or 'indirect' clauses expected}} omp52-error {{unexpected 'allocate' clause, only 'enter', 'link', 'device_type' or 'indirect' clauses expected}}

void bar();
void baz() {bar();}
#pragma omp declare target(bar) // omp5-warning {{declaration marked as declare target after first use, it may lead to incorrect results}} // omp51-warning {{declaration marked as declare target after first use, it may lead to incorrect results}} omp52-warning {{declaration marked as declare target after first use, it may lead to incorrect results}}

extern int b;

struct NonT {
  int a;
};

typedef int sint;

template <typename T>
T bla1() { return 0; }

#pragma omp declare target
template <typename T>
T bla2() { return 0; }
#pragma omp end declare target

template<>
float bla2() { return 1.0; }

#pragma omp declare target
void blub2() {
  bla2<float>();
  bla2<int>();
}
#pragma omp end declare target

void t2() {
#pragma omp target
  {
    bla2<float>();
    bla2<long>();
  }
}

#pragma omp declare target
  void abc();
#pragma omp end declare target
void cba();
#pragma omp end declare target // expected-error {{unexpected OpenMP directive '#pragma omp end declare target'}}

#pragma omp declare target
#pragma omp declare target
void def();
#pragma omp end declare target
void fed();

#pragma omp declare target
#pragma omp threadprivate(a) // expected-note {{defined as threadprivate or thread local}}
extern int b;
int g;

struct T {
  int a;
  virtual int method();
};

class VC {
  T member;
  NonT member1;
  public:
    virtual int method() { T a; return 0; }
};

struct C {
  NonT a;
  sint b;
  int method();
  int method1();
};

int C::method1() {
  return 0;
}

void foo(int p) {
  a = 0; // expected-error {{threadprivate variables cannot be used in target constructs}}
  b = 0;
  t = 1; // expected-error {{threadprivate variables cannot be used in target constructs}}
  C object;
  VC object1;
  g = object.method();
  g += object.method1();
  g += object1.method() + p;
  f(); // dev5-error {{function with 'device_type(host)' is not available on device}}
  q();
  c();
}
#pragma omp declare target
void foo1() {
  [&](){ (void)(b+z);}(); // omp5-note {{variable 'z' is captured here}} //omp51-note {{variable 'z' is captured here}} omp52-note {{variable 'z' is captured here}}
}
#pragma omp end declare target

#pragma omp end declare target
#pragma omp end declare target
#pragma omp end declare target // expected-error {{unexpected OpenMP directive '#pragma omp end declare target'}}

int C::method() {
  return 0;
}

struct S {
#pragma omp declare target
  int v;
#pragma omp end declare target
};

int main (int argc, char **argv) {
#pragma omp declare target // expected-error {{unexpected OpenMP directive '#pragma omp declare target'}}
  int v;
#pragma omp end declare target // expected-error {{unexpected OpenMP directive '#pragma omp end declare target'}}
  foo(v);
  return (0);
}

namespace {
#pragma omp declare target
  int x;
}
#pragma omp end declare target

#pragma omp declare target link(S) // expected-error {{'S' used in declare target directive is not a variable or a function name}}

#pragma omp declare target (x, x) // expected-error {{'x' appears multiple times in clauses on the same declare target directive}}
#pragma omp declare target to(x) to(x) // omp45-error {{'x' appears multiple times in clauses on the same declare target directive}} omp5-error {{'x' appears multiple times in clauses on the same declare target directive}} omp51-error {{'x' appears multiple times in clauses on the same declare target directive}} omp52-error {{unexpected 'to' clause, use 'enter' instead}} omp52-error {{expected at least one 'enter', 'link' or 'indirect' clause}}
#pragma omp declare target link(x) // expected-error {{'x' must not appear in both clauses 'to' and 'link'}}

void bazz() {}
#pragma omp declare target to(bazz) device_type(nohost) // omp45-error {{unexpected 'device_type' clause, only 'to' or 'link' clauses expected}} host5-note 3{{marked as 'device_type(nohost)' here}} omp52-error {{unexpected 'to' clause, use 'enter' instead}} omp52-error {{expected at least one 'enter', 'link' or 'indirect' clause}}
void bazzz() {bazz();}
#pragma omp declare target to(bazzz) device_type(nohost) // omp45-error {{unexpected 'device_type' clause, only 'to' or 'link' clauses expected}} omp52-error {{unexpected 'to' clause, use 'enter' instead}} omp52-error {{expected at least one 'enter', 'link' or 'indirect' clause}}
void any() {bazz();} // host5-error {{function with 'device_type(nohost)' is not available on host}}
void host1() {bazz();} // host5-error {{function with 'device_type(nohost)' is not available on host}}
#pragma omp declare target to(host1) device_type(host) // omp45-error {{unexpected 'device_type' clause, only 'to' or 'link' clauses expected}} dev5-note 3 {{marked as 'device_type(host)' here}} omp52-error {{unexpected 'to' clause, use 'enter' instead}} omp52-error {{expected at least one 'enter', 'link' or 'indirect' clause}}
void host2() {bazz();} //host5-error {{function with 'device_type(nohost)' is not available on host}}
#pragma omp declare target to(host2) // omp52-error {{unexpected 'to' clause, use 'enter' instead}} omp52-error {{expected at least one 'enter', 'link' or 'indirect' clause}}
void device() {host1();} // dev5-error {{function with 'device_type(host)' is not available on device}}
#pragma omp declare target to(device) device_type(nohost) // omp45-error {{unexpected 'device_type' clause, only 'to' or 'link' clauses expected}} host5-note 2 {{marked as 'device_type(nohost)' here}} omp52-error {{unexpected 'to' clause, use 'enter' instead}} omp52-error {{expected at least one 'enter', 'link' or 'indirect' clause}}
void host3() {host1();} // dev5-error {{function with 'device_type(host)' is not available on device}}
#pragma omp declare target to(host3) // omp52-error {{unexpected 'to' clause, use 'enter' instead}} omp52-error {{expected at least one 'enter', 'link' or 'indirect' clause}}

#pragma omp declare target
void any1() {any();}
void any2() {host1();} // dev5-error {{function with 'device_type(host)' is not available on device}}
void any3() {device();} // host5-error {{function with 'device_type(nohost)' is not available on host}}
void any4() {any2();}
#pragma omp end declare target

void any5() {any();}
void any6() {host1();}
void any7() {device();} // host5-error {{function with 'device_type(nohost)' is not available on host}}
void any8() {any2();}

int MultiDevTy;
#pragma omp declare target to(MultiDevTy) device_type(any)    // omp45-error {{unexpected 'device_type' clause, only 'to' or 'link' clauses expected}} omp52-error {{unexpected 'to' clause, use 'enter' instead}} omp52-error {{expected at least one 'enter', 'link' or 'indirect' clause}}
#pragma omp declare target to(MultiDevTy) device_type(host)   // omp45-error {{unexpected 'device_type' clause, only 'to' or 'link' clauses expected}} omp5-error {{'device_type(host)' does not match previously specified 'device_type(any)' for the same declaration}} omp51-error {{'device_type(host)' does not match previously specified 'device_type(any)' for the same declaration}} omp52-error {{unexpected 'to' clause, use 'enter' instead}} omp52-error {{expected at least one 'enter', 'link' or 'indirect' clause}}
#pragma omp declare target to(MultiDevTy) device_type(nohost) // omp45-error {{unexpected 'device_type' clause, only 'to' or 'link' clauses expected}} omp5-error {{'device_type(nohost)' does not match previously specified 'device_type(any)' for the same declaration}} // omp51-error {{'device_type(nohost)' does not match previously specified 'device_type(any)' for the same declaration}} omp52-error {{unexpected 'to' clause, use 'enter' instead}} omp52-error {{expected at least one 'enter', 'link' or 'indirect' clause}}

static int variable = 100;  //expected-warning {{declaration is not declared in any declare target region}}
static float variable1 = 200;
static float variable2 = variable1;  //expected-warning {{declaration is not declared in any declare target region}}

static int var = 1;  //expected-warning {{declaration is not declared in any declare target region}}

static int var1 = 10;
static int *var2 = &var1;
static int **ptr1 = &var2;  //expected-warning {{declaration is not declared in any declare target region}}

int arr[2] = {1,2};
int (*arrptr)[2] = &arr;  //expected-warning {{declaration is not declared in any declare target region}}

class declare{
  public: int x;
          void print();
};
declare obj1;
declare *obj2 = &obj1;  //expected-warning {{declaration is not declared in any declare target region}}

struct target{
  int x;
  void print();
};
static target S;  //expected-warning {{declaration is not declared in any declare target region}}

#pragma omp declare target
int target_var = variable;  //expected-note {{used here}}
float target_var1 = variable2;  //expected-note {{used here}}
int *ptr = &var;  //expected-note {{used here}}
int ***ptr2 = &ptr1;  //expected-note {{used here}}
int (**ptr3)[2] = &arrptr;  //expected-note {{used here}}
declare **obj3 = &obj2;  //expected-note {{used here}}
target *S1 = &S; //expected-note {{used here}}
#pragma omp end declare target

#if TESTENDINC
#include "unterminated_declare_target_include.h"
#elif TESTEND
// expected-warning@+1 {{expected '#pragma omp end declare target' at end of file to match '#pragma omp declare target'}}
#pragma omp declare target
#else
// expected-warning@+1 {{expected '#pragma omp end declare target' at end of file to match '#pragma omp begin declare target'}}
#pragma omp begin declare target
#endif
