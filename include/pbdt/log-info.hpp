/*
 * SPDX - FileCopyrightText: © 2024 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <algorithm>
#include <ranges>
#include <source_location>
#include <string>

#ifndef PBDT_MODULE

#include "pbdt/test-context.hpp"

#endif

namespace pbdt::log_info
{
    namespace detail
    {
        struct EventCountLogInfo
        {
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
                                toRate(passed),
                                toRate(failed),
                                toRate(skipped),
                                total,
                            };
                        }
                    );
                }

                const test_context::detail::EventCountable aggregation;
            };

            constexpr EventCountLogInfo(
                const size_t passedRate, const size_t failedRate, const size_t skippedRate, const size_t total
            ) :
                passedRate(passedRate),
                failedRate(failedRate),
                skippedRate(skippedRate),
                total(total)
            {
            }

        private:
            size_t passedRate;
            size_t failedRate;
            size_t skippedRate;
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

        private:
            EventCountLogInfo assertions;
            EventCountLogInfo sampleTests;
        };
    }
}
