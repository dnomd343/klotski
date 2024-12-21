/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

#include <fast_cal/fast_cal.h>

#include "py_ffi/layout.h"

namespace klotski::ffi {

using fast_cal::FastCal;

class PyFastCal {
public:
    explicit PyFastCal(PyLayout code)
        : fast_cal_(FastCal(std::bit_cast<codec::CommonCode>(code).to_raw_code())) {}

    // TODO: export solution path directly
    std::optional<PyLayout> solve() {
        auto tmp = fast_cal_.solve();
        if (tmp.has_value()) {
            return std::bit_cast<PyLayout>(tmp.value().to_common_code());
        }
        return std::nullopt;
    }

    // std::vector<PyCommonCode> solve_multi();

    void build_all() {
        fast_cal_.build_all();
    }

    [[nodiscard]] std::vector<PyLayout> backtrack(PyLayout code) const {
        std::vector<PyLayout> path;
        for (auto x : fast_cal_.backtrack(std::bit_cast<codec::CommonCode>(code).to_raw_code())) {
            path.emplace_back(std::bit_cast<PyLayout>(x.to_common_code()));
        }
        return path;
    }

    [[nodiscard]] std::vector<std::vector<PyLayout>> exports() const {
        std::vector<std::vector<PyLayout>> result;

        const auto data = fast_cal_.exports();
        result.reserve(data.size());
        for (const auto &layer_raw : data) {
            std::vector<PyLayout> layer;
            layer.reserve(layer_raw.size());
            for (auto x : layer_raw) {
                layer.emplace_back(std::bit_cast<PyLayout>(x.to_common_code()));
            }
            result.emplace_back(layer);
        }

        return result;
    }

private:
    // TODO: add double call protect
    FastCal fast_cal_;
};

} // namespace klotski::ffi
