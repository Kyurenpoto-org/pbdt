/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#ifndef PBDT_MODULE

#include <algorithm>
#include <format>
#include <ranges>
#include <source_location>
#include <string>

#include "exstd/functional.hpp"

#endif

namespace pbdt::test_context
{
    namespace detail
    {
        struct EventCountable
        {
            static constexpr EventCountable prototype()
            {
                return EventCountable{
                    0,
                    0,
                    0,
                };
            }

            constexpr EventCountable pass() const
            {
                return EventCountable{
                    passed + 1,
                    failed,
                    skipped,
                };
            }

            constexpr EventCountable fail() const
            {
                return EventCountable{
                    passed,
                    failed + 1,
                    skipped,
                };
            }

            constexpr EventCountable skip() const
            {
                return EventCountable{
                    passed,
                    failed,
                    skipped + 1,
                };
            }

            constexpr EventCountable accumulate(const EventCountable other) const
            {
                return EventCountable{
                    passed + other.passed,
                    failed + other.failed,
                    skipped + other.skipped,
                };
            }

            constexpr bool someFailed() const
            {
                return failed != 0;
            }

            constexpr size_t sum() const
            {
                return passed + failed + skipped;
            }

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
