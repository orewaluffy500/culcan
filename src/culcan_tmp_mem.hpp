#pragma once

#include <optional>

// COMPARISON BUFFER

struct cc_CompareBuffer {
    std::optional<int> a;
    std::optional<int> b;

    cc_CompareBuffer(int a, int b): a(a), b(b) {}
    cc_CompareBuffer(): a(std::nullopt), b(std::nullopt) {}

    void reset(){
        a = std::nullopt;
        b = std::nullopt;
    }

    bool hasValue() { return a.has_value() && b.has_value(); }
};


