/// Klotski Engine by Dnomd343 @2024

#pragma once

#include <vector>

namespace klotski {

template <typename T>
requires std::is_trivial_v<T>
class LayerQueue final {
public:
    ~LayerQueue();

    /// Construct from first layer nodes and reserve size.
    LayerQueue(std::initializer_list<T> first_layer, size_t max_size);

    // ------------------------------------------------------------------------------------- //

    /// Pop the head of the queue.
    void next();

    /// Obtain the current working node.
    T current() const;

    /// Emplace new node at the end of the queue.
    void emplace(T node);

    // ------------------------------------------------------------------------------------- //

    /// Whether the queue is empty.
    [[nodiscard]] bool is_ending() const;

    /// Whether the queue front is on new layer.
    [[nodiscard]] bool is_new_layer() const;

    // ------------------------------------------------------------------------------------- //

    /// Get the nodes of the last layer.
    std::vector<T> last_layer() const;

    /// Get all the nodes of each layer.
    std::vector<std::vector<T>> all_layers() const;

    // ------------------------------------------------------------------------------------- //

private:
    T *data_ {nullptr};
    size_t layer_begin_ {0}, layer_end_;
    size_t queue_begin_ {0}, queue_end_;
    std::vector<size_t> layer_offset_ {0};
};

} // namespace klotski

#include "internal/layer_queue.inl"
