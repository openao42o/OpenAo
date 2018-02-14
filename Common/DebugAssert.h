///////////////////////////////////////////////////////////////////////////////
// DebugAssert.h :    debug assert를 위한 매크로 정의 파일
//                    Condition compilaion(#defien)에 따라 사용이 달라진다.
//
// Date     : 2004-03-25 by cmkwon
// //Copyright[2002] MasangSoft
///////////////////////////////////////////////////////////////////////////////

#ifndef COMMON_DEBUGASSERT_H_
#define COMMON_DEBUGASSERT_H_

#include <assert.h>

#ifndef _DEBUG

#define ASSERT_ASSERT                __noop
#define ASSERT_ASSERT                __noop
#define ASSERT_REQUIRE                __noop
#define ASSERT_REQUIRE                __noop
#define ASSERT_ENSURE                __noop
#define ASSERT_ENSURE                __noop
#define ASSERT_CHECK                __noop
#define ASSERT_CHECK                __noop
#define ASSERT_IMPLIES                __noop
#define ASSERT_NEVER_GET_HERE        __noop
#define ASSERT_NOT_IMPLEMENTED_YET    __noop

#else // _DEBUG

#define ALL_ASSERTIONS
///////////////////////////////////////////////////////////////////////////////
// #define ALL_ASSERTIONS        : 모든 Assert macro 체크함
// #define ASSERTIONS_ASSERT    : ASSERT macro 만 체크함
// #define ASSERTIONS_REQUIRE    : REQUIRE macro 만 체크함
// #define ASSERTIONS_ENSURE    : ENSURE macro 만 체크함
// #define ASSERTIONS_CHECK        : CHECK macro 만 체크함
// #define ASSERTIONS_IMPLIES    : IMPLIES macro 만 체크함
///////////////////////////////////////////////////////////////////////////////

typedef enum
{
    Assertion_ASSERT,
    Assertion_REQUIRE,
    Assertion_ENSURE,
    Assertion_CHECK,
    Assertion_IMPLIES,
    Assertion_NEVER_GET_HERE,
    Assertion_NOT_IMPLEMENTED_YET
} Assertion;


///////////////////////////////////////////////////////////////////////////////
// Assert 창에 보여질 내용을 설정한다.
///////////////////////////////////////////////////////////////////////////////
#define ASSERT_DEBUG(expr1, expr2)


///////////////////////////////////////////////////////////////////////////////
// ASSERT :    Expression이 true임을 확인한다
//            ASSERTION_ASSERT 가 define되어있지 않으면 input code는 삭제 처리 된다.
//
//        ex)
///////////////////////////////////////////////////////////////////////////////
#if defined(ALL_ASSERTIONS) || defined(ASSERTION_ASSERT)
    #define ASSERT_ASSERT(expr)    if (!(expr)) { ASSERT_DEBUG(Assertion_ASSERT, expr); } else {;}
#else
    #define ASSERT_ASSERT(expr)
#endif // ASSERTION_ASSERT_endif

///////////////////////////////////////////////////////////////////////////////
// REQUIRE : Function의 input parameter들의 값의 범위를 확인한다.
//             ASSERTION_REQUIRE 가 define되어있지 않으면 input code는 삭제 처리 된다.
//
//        ex)
///////////////////////////////////////////////////////////////////////////////
#if defined(ALL_ASSERTIONS) || defined(ASSERTION_REQUIRE)
    #define ASSERT_REQUIRE(expr)    if (!(expr)) { ASSERT_DEBUG(Assertion_REQUIRE, expr); } else {;}
#else
    #define ASSERT_REQUIRE(expr)
#endif // ASSERTION_REQUIRE_endif


///////////////////////////////////////////////////////////////////////////////
// ENSURE : Function의 수행이 끝나고 난 뒤, return value의 값의 범위와
//            다른 variable들의 값의 범위를 비교한다.
//            ASSERTION_ENSURE 가 define되어있지 않으면 input code는 삭제 처리 된다.
//
//        ex)
///////////////////////////////////////////////////////////////////////////////
#if defined(ALL_ASSERTIONS) || defined(ASSERTION_ENSURE)
    #define ASSERT_ENSURE(expr)        if (!(expr)) { ASSERT_DEBUG(Assertion_ENSURE, expr); } else {;}
#else
    #define ASSERT_ENSURE(expr)
#endif // ASSERTION_REQUIRE_endif


///////////////////////////////////////////////////////////////////////////////
// CHECK : Conditional compilation(define 조건에 따라)에 의해
//            REQUIRE(), ENSURE()의 input code는 debugging이 끝난 뒤 삭제되는데 반해,
//            CHECK()의 input code는 남겨진다. 즉, 다음과 같은 code가 가능하다.
//            ASSERTION_CHECK 가 define되어있지 않아도 input code는 삭제 처리 되지 않는다.
//
//        ex)    ASSERT_CHECK((pfl = fopen(PARAM_FILE, "r")) != NULL);
///////////////////////////////////////////////////////////////////////////////
#if defined(ALL_ASSERTIONS) || defined(ASSERTION_CHECK)
    #define ASSERT_CHECK(expr)        if (!(expr)) { ASSERT_DEBUG(Assertion_CHECK, expr); } else {;}
#else
    #define ASSERT_CHECK(expr)        if (!(expr)) {;} else {;}
#endif // ASSERTION_CHECK_endif


///////////////////////////////////////////////////////////////////////////////
// IMPLIES : 첫번째 expression이 true인 경우에만 두번째 expression을 Assertion를 체크한다.
//             ASSERTION_ASSERT 가 define되어있지 않으면 input code는 삭제 처리 된다.
///////////////////////////////////////////////////////////////////////////////
#if defined(ALL_ASSERTIONS) || defined(ASSERTION_IMPLIES)
    #define ASSERT_IMPLIES(expr1, expr2)                                            \
        if ((expr1))                                                                \
        {                                                                            \
            if (!(expr2)) { ASSERT_DEBUG(Assertion_IMPLIES, #expr1 "," #expr2); }    \
        }                                                                   \

#else
    #define ASSERT_IMPLIES(expr1, expr2)
#endif // ASSERTION_IMPLIES_endif


///////////////////////////////////////////////////////////////////////////////
// NEVER_GET_HERE : Conditional compilation 에 상관없이 사용가능하며
//                    Control flow가 도달할 수 없는 곳에 두어 체크한다.
//
//        ex)
///////////////////////////////////////////////////////////////////////////////
#define ASSERT_NEVER_GET_HERE()    ASSERT_DEBUG(Assertion_NEVER_GET_HERE, "NEVER_GET_HERE");

///////////////////////////////////////////////////////////////////////////////
// NOT_IMPLEMENTED_YET : 아직 구현되지 않은 함수의 사용을 막기 위해 사용한다.
//
//        ex)
///////////////////////////////////////////////////////////////////////////////
#define ASSERT_NOT_IMPLEMENTED_YET()    ASSERT_DEBUG(Assertion_NOT_IMPLEMENTED_YET, "NOT_IMPLEMENTED_YET");

#endif // _DEBUG

#endif // COMMON_DEBUGASSERT_H_
