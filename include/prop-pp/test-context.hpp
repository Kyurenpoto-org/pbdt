/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <algorithm>
#include <source_location>
#include <string>

#ifndef PBDT_MODULE

#include "exstd/functional.hpp"

#endif

namespace prop_pp::test_context
{
    namespace detail
    {
        constexpr std::string stringify(size_t value)
        {
            if (value == 0)
            {
                return "0";
            }

            std::string result;
            constexpr std::string_view lookup[10] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
            while (value != 0)
            {
                result = std::string(lookup[value % 10]) + result;
                value /= 10;
            }

            return result;
        }

        struct StringifiedLocation
        {
            constexpr operator std::string() const
            {
                return std::string(location.file_name()) + "(" + stringify(location.line()) + ","
                     + stringify(location.column()) + ")\n";
            }

            const std::source_location location;
        };

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

            constexpr operator std::string() const
            {
                return "Passed: " + stringify(passed) + "\n" + "Failed: " + stringify(failed) + "\n"
                     + "Skipped: " + stringify(skipped) + "\n";
            }

        private:
            constexpr EventCountable(const size_t passed, const size_t failed, const size_t skipped) :
                passed(passed),
                failed(failed),
                skipped(skipped)
            {
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
                    {},
                };
            }

            constexpr TestContext
            expect(const bool expression, const std::source_location location = std::source_location::current()) const
            {
                if (expression)
                {
                    return TestContext{
                        assertions.pass(),
                        failures,
                    };
                }
                else
                {
                    return TestContext{
                        assertions.fail(),
                        failures + std::string(StringifiedLocation{ location }),
                    };
                }
            }

            constexpr TestContext accumulate(const TestContext context) const
            {
                return TestContext{
                    assertions + context.assertions,
                    failures + context.failures,
                };
            }

            constexpr bool passable() const
            {
                return !assertions.someFailed();
            }

            constexpr std::string description() const
            {
                if (passable())
                {
                    return "<Assertions>\n" + std::string(assertions);
                }
                else
                {
                    return failures + "\n\n<Assertions>\n" + std::string(assertions);
                }
            }

        private:
            constexpr TestContext(const EventCountable assertions, const std::string failures) :
                assertions(assertions),
                failures(failures)
            {
            }

            const EventCountable assertions;
            const std::string failures;
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
                    {},
                };
            }

            template <typename Test>
            constexpr SampledTestContext accumulate(const std::string sample, Test&& test) const
            {
                if (samples.someFailed())
                {
                    return SampledTestContext{
                        samples.skip(),
                        failures,
                    };
                }

                const TestContext context = test();
                if (context.passable())
                {
                    return SampledTestContext{
                        samples.pass(),
                        failures,
                    };
                }
                else
                {
                    return SampledTestContext{
                        samples.fail(),
                        failures + context.description(),
                    };
                }
            }

            constexpr bool passable() const
            {
                return !samples.someFailed();
            }

            constexpr std::string description() const
            {
                if (passable())
                {
                    return "<Samples>\n" + std::string(samples);
                }
                else
                {
                    return failures + "\n\n<Samples>\n" + std::string(samples);
                }
            }

        private:
            constexpr SampledTestContext(const EventCountable samples, const std::string failures) :
                samples(samples),
                failures(failures)
            {
            }

            EventCountable samples;
            std::string failures;
        };
    }

    template <typename Samples, typename Test>
    constexpr detail::SampledTestContext parameterizedContext(Samples&& samples, Test&& test)
    {
        return std::ranges::fold_left(
            std::forward<Samples>(samples), detail::SampledTestContext::prototype(),
            [&test](const detail::SampledTestContext context, const auto sample)
            {
                return context.accumulate(
                    "",
                    [&]()
                    {
                        return test(sample);
                    }
                );
            }
        );
    }

    template <typename Target, typename Prop, typename Domain>
    constexpr detail::SampledTestContext propertyContext(Target&& target, Prop&& prop, Domain&& domain)
    {
        return prop_pp::test_context::parameterizedContext(
            std::forward<Domain>(domain),
            [&target, &prop](const auto& sample)
            {
                return prop(sample, exstd::applyOrInvoke(target, sample));
            }
        );
    }
}
