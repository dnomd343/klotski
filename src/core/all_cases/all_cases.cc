#include <future>
#include "all_cases.h"

namespace klotski {
namespace cases {

/// Calculate all possible klotski heads.
static consteval std::array<int, 12> case_heads() {
    std::array<int, 12> heads = {};
    for (int i = 0, head = 0; head < 15; ++head) {
        if (head % 4 != 3) {
            heads[i++] = head;
        }
    }
    return heads;
}

/// Check whether the combination of head and range is valid.
static int check_range(int head, uint32_t range) noexcept {
    constexpr uint32_t MASK_1x1 = 0b00000001;
    constexpr uint32_t MASK_1x2 = 0b00000011;
    constexpr uint32_t MASK_2x1 = 0b00010001;
    constexpr uint32_t MASK_2x2 = 0b00110011;

    uint32_t flags = MASK_2x2 << head; // fill 2x2 block
    for (int addr = 0, offset = 1; range; range >>= 2, ++offset) { // traverse every 2-bit
        auto num = low_zero_num(~flags);
        addr += num; // next unfilled block
        flags >>= num;
        switch (range & 0b11) {
            case 0b00: // space
            case 0b11: // 1x1 block
                flags |= MASK_1x1;
                continue;
            case 0b10: // 2x1 block
                if ((flags >> 4) & 0b1 || addr > 15) { // invalid case
                    return offset; // broken offset
                }
                flags |= MASK_2x1;
                continue;
            case 0b01: // 1x2 block
                if ((flags >> 1) & 0b1 || (addr & 0b11) == 0b11) { // invalid case
                    return offset; // broken offset
                }
                flags |= MASK_1x2;
                continue;
        }
    }
    return 0; // pass check
}

/// Build all valid ranges of the specified head.
void AllCases::BuildCases(int head, Ranges &release) noexcept {
    release.reserve(ALL_CASES_NUM[head]);
    auto &basic_ranges = BasicRanges::Instance().Fetch();
    for (uint32_t index = 0; index < basic_ranges.size(); ++index) {
        auto offset = check_range(head, basic_ranges[index]);
        if (offset) { // invalid case
            auto tmp = (uint32_t)0b1 << (32 - offset * 2); // distance to next possible range
            ///         !! <- broken
            /// ( xx xx xx ) xx xx xx ... [reversed range]
            ///         +1   00 00 00 ...     (delta)
            tmp += range_reverse(basic_ranges[index]) & ~(tmp - 1);
            while (range_reverse(basic_ranges[++index]) < tmp); // located next range
            --index;
            continue;
        }
        release.emplace_back(range_reverse(basic_ranges[index])); // release valid case
    }
}

/// Execute the build process with parallel support and ensure thread safety.
void AllCases::BuildParallel(Executor &&executor) noexcept {
    if (available_) {
        return; // reduce consumption of mutex
    }
    std::lock_guard<std::mutex> guard(building_);
    if (available_) {
        return; // data is already available
    }
    std::vector<std::future<void>> futures;
    for (auto head : case_heads()) {
        // TODO: using std::move_only_function in C++23
        //       -> avoid using std::shared_ptr<std::promise<void>>
        auto promise = std::make_shared<std::promise<void>>();
        futures.emplace_back(promise->get_future());
        executor([head, promise = std::move(promise)]() {
            BuildCases(head, GetCases()[head]);
            promise->set_value(); // subtask completed notification
        });
    }
    for (auto &x : futures) {
        x.get(); // wait until all subtasks completed
    }
    available_ = true;
}

/// Execute the build process and ensure thread safety.
void AllCases::Build() noexcept {
    BuildParallel([](auto &&func) {
        func();
    });
}

RangesUnion& AllCases::GetCases() noexcept {
    static RangesUnion cases;
    return cases;
}

AllCases& AllCases::Instance() noexcept {
    static AllCases instance;
    return instance;
}

const RangesUnion& AllCases::Fetch() noexcept {
    this->Build();
    return GetCases();
}

bool AllCases::IsAvailable() const noexcept {
    return available_;
}

} // namespace cases
} // namespace klotski
