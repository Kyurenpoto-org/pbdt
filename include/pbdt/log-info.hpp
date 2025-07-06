/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#ifndef PBDT_MODULE

#include <format>
#include <string>
#include <string_view>

#include "pbdt/test-context.hpp"

#endif

namespace pbdt::log_info
{
    namespace detail
    {
        struct EventCountLogInfo
        {
            struct AmountWithRate
            {
                size_t amount;
                size_t rate;

                operator std::string() const
                {
                    return std::format("{:>5} ({:>3}%)", amount, rate);
                }
            };

            struct Factory
            {
                constexpr EventCountLogInfo create() const
                {
                    return aggregation.provideAggregations(
                        [](const size_t passed, const size_t failed, const size_t skipped)
                        {
                            const size_t total = passed + failed + skipped;
                            const auto toRate = [&total](const size_t amount)
                            {
                                return amount * 100 / total;
                            };

                            return EventCountLogInfo{
                                AmountWithRate{
                                    passed,
                                    toRate(passed),
                                },
                                AmountWithRate{
                                    failed,
                                    toRate(failed),
                                },
                                AmountWithRate{
                                    skipped,
                                    toRate(skipped),
                                },
                                total,
                            };
                        }
                    );
                }

                const test_context::detail::EventCountable aggregation;
            };

            constexpr EventCountLogInfo(
                const AmountWithRate passed, const AmountWithRate failed, const AmountWithRate skipped,
                const size_t total
            ) :
                passed(passed),
                failed(failed),
                skipped(skipped),
                total(total)
            {
            }

            struct Colors
            {
                static constexpr std::string_view NONE = "\033[0m";
                static constexpr std::string_view PASS = "\033[32m";
                static constexpr std::string_view FAIL = "\033[31m";
                static constexpr std::string_view SKIP = "\033[33m";
            };

            operator std::string() const
            {
                return std::format(
                    "{}{} passed{}, {}{} failed{}, {}{} skipped{} | {}", Colors::PASS, passed, Colors::NONE,
                    Colors::FAIL, failed, Colors::NONE, Colors::SKIP, skipped, Colors::NONE, total
                );
            }

        private:
            AmountWithRate passed;
            AmountWithRate failed;
            AmountWithRate skipped;
            size_t total;
        };

        template <typename Param, typename Result>
        struct FailedTestLogInfo
        {
            Param param;
            Result result;
        };

        template <typename... SampleLogs>
        struct FailedTestSamplesLogInfo
        {
            std::tuple<SampleLogs...> failedSamples;
        };

        struct SampledTestLogInfo
        {
            struct Factory
            {
                constexpr SampledTestLogInfo create() const
                {
                    return context.provideEventAggregation(
                        [](const test_context::detail::EventCountable sampleTests,
                           const test_context::detail::TestContext& assertionContext)
                        {
                            return SampledTestLogInfo{
                                assertionContext.provideEventAggregation(
                                    [](const test_context::detail::EventCountable assertions)
                                    {
                                        return EventCountLogInfo::Factory{ assertions }.create();
                                    }
                                ),
                                EventCountLogInfo::Factory{ sampleTests }.create(),
                            };
                        }
                    );
                }

                const test_context::detail::SampledTestContext context;
            };

            constexpr SampledTestLogInfo(const EventCountLogInfo assertions, const EventCountLogInfo sampleTests) :
                assertions(assertions),
                sampleTests(sampleTests)
            {
            }

            operator std::string() const
            {
                return std::format("Assertion: {}\nDomain: {}\n", assertions, sampleTests);
            }

        private:
            EventCountLogInfo assertions;
            EventCountLogInfo sampleTests;
        };
    }
}
