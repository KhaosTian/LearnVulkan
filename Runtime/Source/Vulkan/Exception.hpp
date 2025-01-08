#pragma once
#include <source_location>
#include <stacktrace>
#include <stdexcept>

template<typename Exception>
struct ExceptionWithTrace: public Exception {
    std::stacktrace trace { std::stacktrace::current() };

    template<typename... Args>
    ExceptionWithTrace(Args&&... args): Exception(std::forward<Args>(args)...) {}
};

template<typename E>
[[noreturn]] inline void Throw(const E& e) {
    throw ExceptionWithTrace<E>(e);
}