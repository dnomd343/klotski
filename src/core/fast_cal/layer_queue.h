/// Klotski Engine by Dnomd343 @2024

#pragma once

#include <vector>
#include <cstdint>

namespace klotski {

template <typename T>
class LayerQueue {
public:
    LayerQueue(size_t reserve, std::initializer_list<T> first_layer);

    void emplace(T item);

    T current() const;

    void next();

    [[nodiscard]] bool is_ending() const;

    [[nodiscard]] bool is_new_layer() const;

    std::vector<T> layer_cases() const;

    // TODO: allow export all layers

private:
    size_t queue_begin_ {0};
    size_t queue_end_ {0};

    size_t layer_begin_ {0};
    size_t layer_end_ {0};

    std::vector<T> data_ {};

    std::vector<std::pair<size_t, size_t>> layers_ {};
};

} // namespace klotski

#include "internal/layer_queue.inl"
