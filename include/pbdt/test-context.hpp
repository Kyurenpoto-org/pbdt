/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#ifndef PBDT_MODULE

#include <algorithm>
#include <array>
#include <format>
#include <ranges>
#include <source_location>
#include <string>
#include <vector>

#include "exstd/functional.hpp"
#include "exstd/ranges.hpp"

#endif

namespace pbdt::test_context
{
    namespace detail
    {
        /**
         * @brief Event counting structure
         *
         * @details This structure is used to count the number of passed, failed, and skipped events.
         *
         */
        struct EventCountable
        {
            /**
             * @brief Construct a prototype EventCountable with zero counts.
             *
             * @return constexpr EventCountable
             */
            static constexpr EventCountable prototype()
            {
                return EventCountable{
                    0,
                    0,
                    0,
                };
            }

            /**
             * @brief Count a passed event.
             *
             * @return constexpr EventCountable
             */
            constexpr EventCountable pass() const
            {
                return EventCountable{
                    passed + 1,
                    failed,
                    skipped,
                };
            }

            /**
             * @brief Count a failed event.
             *
             * @return constexpr EventCountable
             */
            constexpr EventCountable fail() const
            {
                return EventCountable{
                    passed,
                    failed + 1,
                    skipped,
                };
            }

            /**
             * @brief Count a skipped event.
             *
             * @return constexpr EventCountable
             */
            constexpr EventCountable skip() const
            {
                return EventCountable{
                    passed,
                    failed,
                    skipped + 1,
                };
            }

            /**
             * @brief Accumulate another EventCountable into this one.
             *
             * @param other
             * @return constexpr EventCountable
             */
            constexpr EventCountable accumulate(const EventCountable other) const
            {
                return EventCountable{
                    passed + other.passed,
                    failed + other.failed,
                    skipped + other.skipped,
                };
            }

            /**
             * @brief Check if there are any failed events.
             *
             * @details This must be satisfied with homomorphic property.
             *
             * @return constexpr bool
             */
            constexpr bool someFailed() const
            {
                return failed != 0;
            }

            /**
             * @brief Compute the total number of events.
             *
             * @details This must be satisfied with homomorphic property.
             *
             * @return constexpr size_t
             */
            constexpr size_t sum() const
            {
                return passed + failed + skipped;
            }

            /**
             * @brief Format the event counts as a string.
             *
             * @details The format includes the number and percentage of each type of events.
             * This must be satisfied with inclusive property.
             *
             * @return std::string
             */
            operator std::string() const
            {
                return std::format(
                    "\033[32m{:>5} ({:>3}%) passed\033[0m, "
                    "\033[31m{:>5} ({:>3}%) failed\033[0m, "
                    "\033[33m{:>5} ({:>3}%) skipped\033[0m"
                    " | {}",
                    passed, rate(passed), failed, rate(failed), skipped, rate(skipped), sum()
                );
            }

        private:
            constexpr EventCountable(const size_t passed, const size_t failed, const size_t skipped) :
                passed(passed),
                failed(failed),
                skipped(skipped)
            {
            }

            constexpr size_t rate(const size_t amount) const
            {
                return sum() == 0 ? 0 : amount * 100 / sum();
            }

            size_t passed;
            size_t failed;
            size_t skipped;
        };

        constexpr EventCountable operator+(const EventCountable lhs, const EventCountable rhs)
        {
            return lhs.accumulate(rhs);
        }

        struct ExpectationOperand
        {
            enum class Operand
            {
                PASS,
                FAIL,
                SKIP,
            };

            constexpr ExpectationOperand(
                const bool expression, const std::string_view message, const std::source_location location
            ) :
                ExpectationOperand{
                    expression ? Operand::PASS : Operand::FAIL,
                    message,
                    location,
                }
            {
            }

            constexpr ExpectationOperand(
                const Operand operand, const std::string_view message, const std::source_location location
            ) :
                operand{
                    operand,
                },
                message{
                    message,
                },
                location{
                    location,
                }
            {
            }

            template <typename EventCountable>
            constexpr EventCountable countEvent(const EventCountable context) const
            {
                switch (operand)
                {
                case Operand::PASS:
                    return context.pass();
                case Operand::FAIL:
                    return context.fail();
                case Operand::SKIP:
                    return context.skip();
                default:
                    std::unreachable();
                }
            }

            struct FailureReason
            {
                constexpr FailureReason(
                    const std::string_view message,
                    const std::source_location location = std::source_location::current()
                ) :
                    message{
                        message,
                    },
                    location{
                        location,
                    }
                {
                }

                operator std::string() const
                {
                    return std::format(
                        "{}({},{})\n{}", location.file_name(), location.line(), location.column(), message
                    );
                }

            private:
                std::string_view message;
                std::source_location location;
            };

            constexpr FailureReason reportFailure() const
            {
                return FailureReason{
                    message,
                    location,
                };
            }

        private:
            Operand operand;
            std::string_view message;
            std::source_location location;
        };

        /**
         * @brief Expectation context structure
         *
         * @details This structure is used to construct expectation context from expectation chain.
         *
         * @tparam N
         */
        template <size_t N>
        struct ExpectationContext
        {
            /**
             * @brief Construct expectation chain.
             *
             * @pre Only if the first expectation
             *
             * @param expression
             * @param message
             * @param location
             * @return constexpr ExpectationContext<1>
             */
            static constexpr ExpectationContext<1> expect(
                const bool expression, const std::string_view message,
                const std::source_location location = std::source_location::current()
            )
                requires(N == 0)
            {
                return {
                    std::array{
                        ExpectationOperand{
                            expression,
                            message,
                            location,
                        },
                    },
                };
            }

            /**
             * @brief Expand expectation chain.
             *
             * @pre Only if not the first expectation
             *
             * @param expression
             * @param message
             * @param location
             * @return constexpr ExpectationContext<N + 1>
             */
            constexpr ExpectationContext<N + 1> expect(
                const bool expression, const std::string_view message,
                const std::source_location location = std::source_location::current()
            ) const
                requires(N != 0)
            {
                return expand(
                    ExpectationOperand{
                        expression,
                        message,
                        location,
                    }
                );
            }

            /**
             * @brief Accumulate expectation chain.
             *
             * @pre Only if not the first expectation
             *
             * @param context
             * @return constexpr ExpectationContext<N + M>
             */
            template <size_t M>
            constexpr ExpectationContext<N + M> accumulate(const ExpectationContext<M> context) const
                requires(N + M != 0)
            {
                std::vector<ExpectationOperand> newOperands;
                newOperands.reserve(N + M);
                newOperands.insert(newOperands.end(), operands.begin(), operands.end());
                newOperands.insert(newOperands.end(), context.operands.begin(), context.operands.end());

                return {
                    exstd::vecToArr(newOperands, std::make_index_sequence<N + M>{}),
                };
            }

            /**
             * @brief Count the number of each event types.
             *
             * @details This must be satisfied with homomorphic, associative and commutative property.
             *
             * @pre Only if not the first expectation
             *
             * @tparam EventCountable
             * @return constexpr EventCountable
             */
            template <typename EventCountable>
            constexpr EventCountable countedEvents() const
                requires(N != 0)
            {
                EventCountable context = EventCountable::prototype();
                for (const auto& operand : operands)
                {
                    context = operand.countEvent(context);
                }

                return context;
            }

            /**
             * @brief Stringifiable Failure report structure
             *
             * @pre Only if not the first expectation
             *
             * @tparam EventCountable
             */
            template <typename EventCountable>
                requires(N != 0)
            struct FailureReport
            {
                constexpr FailureReport(const std::array<ExpectationOperand, N> operands) :
                    operands{
                        operands,
                    }
                {
                }

                /**
                 * @brief Format failure report as a string
                 *
                 * @return std::string
                 */
                operator std::string() const
                {
                    std::string result;
                    for (const auto& operand : operands)
                    {
                        if (!operand.countEvent(EventCountable::prototype()).someFailed())
                        {
                            continue;
                        }

                        result += static_cast<std::string>(operand.reportFailure()) + "\n";
                    }
                    return result;
                }

            private:
                std::array<ExpectationOperand, N> operands;
            };

            /**
             * @brief Construct failure report
             *
             * @details This must be satisfied with homomorphic and associative property.
             *
             * @pre Only if not the first expectation
             *
             * @tparam EventCountable
             * @return constexpr FailureReport<EventCountable>
             */
            template <typename EventCountable>
            constexpr FailureReport<EventCountable> failureReport() const
                requires(N != 0)
            {
                return FailureReport<EventCountable>{ operands };
            }

        private:
            template <size_t M>
            friend struct ExpectationContext;

            constexpr ExpectationContext(const std::array<ExpectationOperand, N> operands) :
                operands{
                    operands,
                }
            {
            }

            constexpr ExpectationContext<N + 1> expand(const ExpectationOperand operand) const
            {
                std::vector<ExpectationOperand> newOperands;
                newOperands.reserve(N + 1);
                newOperands.insert(newOperands.end(), operands.begin(), operands.end());
                newOperands.push_back(operand);

                return {
                    exstd::vecToArr(newOperands, std::make_index_sequence<N + 1>{}),
                };
            }

            std::array<ExpectationOperand, N> operands;
        };

        template <size_t N, size_t M>
        constexpr ExpectationContext<N + M> operator+(const ExpectationContext<N> lhs, const ExpectationContext<M> rhs)
        {
            return lhs.accumulate(rhs);
        }

        /*
         * @todo Non-param failures
         * 1. ~string concat~
         * 2. ~string array~
         * 3. failure class tuple
         * non-branch
         * affect to `then` clause
         */
        struct TestContext
        {
            static constexpr TestContext prototype()
            {
                return TestContext{
                    EventCountable::prototype(),
                    //{},
                };
            }

            constexpr TestContext
            expect(const bool expression, const std::source_location location = std::source_location::current()) const
            {
                if (expression)
                {
                    return TestContext{
                        assertionEvents.pass(),
                        // failures,
                    };
                }
                else
                {
                    return TestContext{
                        assertionEvents.fail(),
                        // failures + std::string(StringifiedLocation{ location }),
                    };
                }
            }

            constexpr TestContext accumulate(const TestContext context) const
            {
                return TestContext{
                    assertionEvents + context.assertionEvents,
                    // failures + context.failures,
                };
            }

            constexpr bool passable() const
            {
                return !assertionEvents.someFailed();
            }

            // constexpr std::string description() const
            //{
            //     if (passable())
            //     {
            //         return "<Assertions>\n" + std::string(assertionEvents);
            //     }
            //     else
            //     {
            //         return failures + "\n\n<Assertions>\n" + std::string(assertionEvents);
            //     }
            // }

        private:
            constexpr TestContext(
                const EventCountable assertionEvents
                //, const std::string failures
            ) :
                assertionEvents(assertionEvents) //,
            // failures(failures)
            {
            }

            EventCountable assertionEvents;
            // const std::string failures;
        };

        constexpr TestContext operator+(const TestContext lhs, const TestContext rhs)
        {
            return lhs.accumulate(rhs);
        }
    }

    constexpr auto expect(const bool expression, const std::source_location& location = std::source_location::current())
    {
        return detail::TestContext::prototype().expect(expression, location);
    }

    namespace detail
    {
        /*
         * @todo param failures
         * static/dynamic array of ExpectationContext<N>, samples, results
         */
        struct SampledTestContext
        {
            static constexpr SampledTestContext prototype()
            {
                return SampledTestContext{
                    EventCountable::prototype(),
                    TestContext::prototype(),
                };
            }

            template <typename Test>
            constexpr SampledTestContext accumulate(Test&& test) const
            {
                if (sampleTestEvents.someFailed())
                {
                    return SampledTestContext{
                        sampleTestEvents.skip(),
                        assertionContext,
                    };
                }
                else
                {
                    const TestContext context = test();
                    return SampledTestContext{
                        context.passable() ? sampleTestEvents.pass() : sampleTestEvents.fail(),
                        assertionContext.accumulate(context),
                    };
                }
            }

            constexpr bool passable() const
            {
                return !sampleTestEvents.someFailed();
            }

        private:
            constexpr SampledTestContext(const EventCountable sampleTestEvents, const TestContext assertionContext) :
                sampleTestEvents(sampleTestEvents),
                assertionContext(assertionContext)
            {
            }

            EventCountable sampleTestEvents;
            TestContext assertionContext;
        };
    }

    // test 함수의 리턴 타입은 (샘플, 결과, 판별)
    // 컨텍스트는 내부적으로 판별 값을 이벤트 카운팅으로 집계
    // 또한 판별 값이 실패인 경우 나머지 값을 배열에 누적
    // 샘플 배열이 정적 배열인지 동적 배열인지 정해지지 않음
    // 따라서 배열의 타입을 템플릿 인자로 전달
    // 배열의 원소 타입에서 최소한 판별 타입은 사용되지 않음
    // 따라서 test 함수의 리턴 타입을 이용해서 누적 배열의 원소의 타입을 얻어내야 함
    // 또한 컨텍스트 누적을 위해서 누적 변수의 타입을 varaint로 설정해야 함
    // 일단 샘플의 갯수를 컴파일 타임에 알 수 있다고 가정
    template <typename Samples, typename Test>
    constexpr detail::SampledTestContext parameterizedContext(Samples&& samples, Test&& test)
    {
#if __cpp_lib_ranges_fold >= 202207L
        return std::ranges::fold_left(
            std::forward<Samples>(samples), detail::SampledTestContext::prototype(),
            [&test](const detail::SampledTestContext context, const auto sample)
            {
                return context.accumulate(
                    //"",
                    [&]()
                    {
                        return test(sample);
                    }
                );
            }
        );
#else
        detail::SampledTestContext context = detail::SampledTestContext::prototype();
        for (const auto& sample : samples)
        {
            context = context.accumulate(
                //"",
                [&]()
                {
                    return test(sample);
                }
            );
        }

        return context;
#endif
    }
}
