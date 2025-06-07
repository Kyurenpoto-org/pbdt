/*
 * SPDX - FileCopyrightText: © 2025 Kyurenpoto <heal9179@gmail.com>
 *
 * SPDX - License - Identifier: MIT
 */

#pragma once

#include <array>
#include <cstdlib>

template <size_t N>
struct SuiteRunner
{
    SuiteRunner(std::array<const char*, N> names, std::array<void (*)(), N> funcs) :
        names(std::move(names)),
        funcs(std::move(funcs))
    {
    }

    template <typename Runner>
    void run(Runner&& runner)
    {
        for (size_t i = 0; i < N; ++i)
        {
            runner(names[i], funcs[i]);
        }
    }

    void run(int argc, const char* const* argv)
    {
        if (argc == 1)
        {
            run(
                [](const auto& name, auto&& func)
                {
                    func();
                }
            );
        }
        else if (argc == 2)
        {
            run(
                [&](const auto& name, auto&& func)
                {
                    if (name == argv[1])
                    {
                        func();
                    }
                }
            );
        }
        else
        {
            exit(EXIT_FAILURE);
        }
    }

private:
    std::array<const char*, N> names;
    std::array<void (*)(), N> funcs;
};

template <size_t N>
auto suite(std::array<const char*, N> names, std::array<void (*)(), N> funcs)
{
    return SuiteRunner(names, funcs);
}
