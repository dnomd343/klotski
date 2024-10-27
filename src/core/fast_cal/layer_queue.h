/// Klotski Engine by Dnomd343 @2024

#pragma once

#include <vector>

namespace klotski {

template <typename T>
class LayerQueue final {
public:
    ~LayerQueue();

    /// Construct from first layer cases and reserve size.
    LayerQueue(std::initializer_list<T> first_layer, size_t reserve);

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
    size_t layer_begin_, layer_end_;
    size_t queue_begin_, queue_end_;

    // std::vector<T> data_ {};
    T *data_ {nullptr};

    std::vector<size_t> layers_ {};
    // std::vector<std::pair<size_t, size_t>> layers_ {};
};

} // namespace klotski

#include "internal/layer_queue.inl"
