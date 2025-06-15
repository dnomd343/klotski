#pragma once

namespace klotski {

template <typename T>
requires std::is_trivial_v<T>
LayerQueuePro<T>::LayerQueuePro(std::initializer_list<T> first_layer, const size_t max_size)
        : layer_end_(first_layer.size()), queue_end_(0) {
    data_ = static_cast<T*>(std::malloc(sizeof(T) * max_size));
    for (const auto node : first_layer) {
        emplace(node);
    }
    layer_offset_.reserve(232); // TODO: confirm the max layer number
    layer_offset_.emplace_back(layer_end_);
}

template <typename T>
requires std::is_trivial_v<T>
LayerQueuePro<T>::~LayerQueuePro() {
    std::free(data_);
}

template <typename T>
requires std::is_trivial_v<T>
T LayerQueuePro<T>::current() const {
    return data_[queue_begin_];
}

template <typename T>
requires std::is_trivial_v<T>
void LayerQueuePro<T>::emplace(T node) {
    data_[queue_end_] = node;
    ++queue_end_;
}

template <typename T>
requires std::is_trivial_v<T>
void LayerQueuePro<T>::next() {
    ++queue_begin_;
    if (queue_begin_ == layer_end_ && !is_ending()) {
        layer_begin_ = layer_end_;
        layer_end_ = queue_end_;
        layer_offset_.emplace_back(layer_end_);
    }
}

template <typename T>
requires std::is_trivial_v<T>
[[nodiscard]] bool LayerQueuePro<T>::is_ending() const {
    return queue_begin_ == queue_end_;
}

// template <typename T>
// requires std::is_trivial_v<T>
// [[nodiscard]] bool LayerQueuePro<T>::is_new_layer() const {
//     return queue_begin_ == layer_begin_;
// }

// template <typename T>
// requires std::is_trivial_v<T>
// std::vector<T> LayerQueuePro<T>::last_layer() const {
//     return {data_ + layer_begin_, data_ + layer_end_};
// }

// template <typename T>
// requires std::is_trivial_v<T>
// std::vector<std::vector<T>> LayerQueuePro<T>::all_layers() const {
//     std::vector<std::vector<T>> result;
//     result.reserve(layer_offset_.size() - 1);
//     for (size_t i = 0; i < layer_offset_.size() - 1; ++i) {
//         result.emplace_back(std::vector<T> {
//             data_ + layer_offset_[i],
//             data_ + layer_offset_[i + 1]
//         });
//     }
//     return result;
// }

} // namespace klotski
