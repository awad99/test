#pragma once

#ifndef Unroller_h
#define Unroller_h

// Unroll a loop

namespace andrivet {
    namespace ADVobfuscator {

        template <int N>
        struct Unroller
        {
            template<typename F, typename... Args>
            inline auto operator()(F&& f, Args&&... args) -> decltype(std::forward<F>(f)(std::forward<Args>(args)...))
            {
                Unroller<N - 1>{}(std::forward<F>(f), std::forward<Args>(args)...);
                return std::forward<F>(f)(std::forward<Args>(args)...);
            }
        };

        template <>
        struct Unroller<1>
        {
            template<typename F, typename... Args>
            inline auto operator()(F&& f, Args&&... args) -> decltype(std::forward<F>(f)(std::forward<Args>(args)...))
            {
                return std::forward<F>(f)(std::forward<Args>(args)...);
            }
        };

    }
}

#endif