/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

#include <fast_cal/fast_cal.h>

#include "py_ffi/common_code.h"

namespace klotski::ffi {

using fast_cal::FastCal;

class PyFastCal {
public:
    explicit PyFastCal(PyCommonCode code)
        : fast_cal_(FastCal(std::bit_cast<CommonCode>(code).to_raw_code())) {}

    // TODO: export solution path directly
    std::optional<PyCommonCode> solve() {
        auto tmp = fast_cal_.solve();
        if (tmp.has_value()) {
            return std::bit_cast<PyCommonCode>(tmp.value().to_common_code());
        }
        return std::nullopt;
    }

    // std::vector<PyCommonCode> solve_multi();

    void build_all() {
        fast_cal_.build_all();
    }

    [[nodiscard]] std::vector<PyCommonCode> backtrack(PyCommonCode code) const {
        std::vector<PyCommonCode> path;
        for (auto x : fast_cal_.backtrack(std::bit_cast<CommonCode>(code).to_raw_code())) {
            path.emplace_back(std::bit_cast<PyCommonCode>(x.to_common_code()));
        }
        return path;
    }

    [[nodiscard]] std::vector<std::vector<PyCommonCode>> exports() const {
        std::vector<std::vector<PyCommonCode>> result;

        const auto data = fast_cal_.exports();
        result.reserve(data.size());
        for (const auto &layer_raw : data) {
            std::vector<PyCommonCode> layer;
            layer.reserve(layer_raw.size());
            for (auto x : layer_raw) {
                layer.emplace_back(std::bit_cast<PyCommonCode>(x.to_common_code()));
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
