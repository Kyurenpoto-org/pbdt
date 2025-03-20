/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#include <print>

#include "pbdt/bdd.hpp"

#include "util.hpp"

template <typename, typename>
struct X;

template <typename>
struct Outer
{
    struct Inner
    {
    };
};

template <typename T>
struct X<typename Outer<T>::Inner, T>
{
};

int main()
{
    /*pbdt::bdd::builder.withGiven(pbdt::bdd::given("", natural_components::target::freeFuncTarget))
        .withWhen(pbdt::bdd::when("", natural_components::domain::containerDomain))
        .withThen(pbdt::bdd::then("", natural_components::prop::freeFuncProp))
        .build("");*/

    // <요건 bdd가 아님>
    // 기능/시나리오 리스트 얻어내기
    // 특정 기능/시나리오 실행하기
    // 실패시 기능/시나리오 설명 출력

    // <이쪽이 bdd>
    // 타겟이 자유함수/멤버함수/함수객체인 경우
    // 타겟이 호출 가능이 아닌 경우
    // 속성이 자유함수/멤버함수/함수객체인 경우
    // 속성이 호출 가능이 아닌 경우
    // 도메인이 컨테이너/생성기/뷰인 경우
    // 도메인이 범위가 아닌 경우
    // 타겟-도메인-속성의 타입이 맞물리는 경우
    // 타겟-도메인-속성의 타입이 맞물리지 않는 경우

    // <이건 테스트 통계 기능>
    // 조기 종료 테스트인 경우
    // 지연 종료 테스트인 경우
    // 도메인에서 샘플링할 최대/최소 샘플이 지정된 경우
    // 최대 지연 실패 수가 지정된 경우

    // scenario는 test, rule/feature는 suite, expect는 assert
    // formatter는 특정 포멧에 맞춰 결과물 출력
    // test는 중첩을 허용하지 않음
    // 모든건 test 중심으로

    // 각 컴포넌트 불변성 테스트에서 컴포넌트의 공역을 X라고 할 때, 정의역은 임의의 자연수 n에 대해 X^n
    // 따라서 컴포넌트의 타입은 union(X, X^2, X^3, ...) -> X in union(X, X^2, X^3, ...)
    // 컴포넌트는 임의의 차원의 점을 1차원으로 사영하는 함수
    // 일반적인 사영 함수는 멱등 함수
    // 이 사영 함수는 변형 수축 함수이므로 브라우어 고정점 정리에 의해 공역 위의 항등함수 = 멱등 함수

    // 각 컴포넌트에서 andComponent 계열의 함수는 직합
    // union(X, X^2, X^3, ...)는 직합에 대해 반군
    // 그런데 andComponent 계열의 함수는 하나씩만 직합이 가능하므로 결합 법칙을 테스트 할 수 없다
    // 억지로 한다면 사영 함수를 이용하면 된다
    // given은 집합 범주지만, 항등원에 대한 것은 구현할 필요가 없기 때문에 테스트하지 않는다
    // when은 임의의 두 대상에 대한 곱이 존재하는 집합 범주이며 교환 법칙이 성립한다
    // then은 끝이 TestContext이고 각 1차 사상 집합의 임의의 두 사상에 대해 곱이 존재하는 2차 범주이며, 교환 법칙이
    // 성립한다

    return EXIT_SUCCESS;
}