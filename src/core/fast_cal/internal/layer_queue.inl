#pragma once

namespace klotski {

template <typename T>
requires std::is_trivial_v<T>
LayerQueue<T>::LayerQueue(std::initializer_list<T> first_layer, const size_t max_size)
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
LayerQueue<T>::~LayerQueue() {
    std::free(data_);
}

template <typename T>
requires std::is_trivial_v<T>
T LayerQueue<T>::current() const {
    return data_[queue_begin_];
}

template <typename T>
requires std::is_trivial_v<T>
void LayerQueue<T>::emplace(T node) {
    data_[queue_end_] = node;
    ++queue_end_;
}

template <typename T>
requires std::is_trivial_v<T>
void LayerQueue<T>::next() {
    ++queue_begin_;
    if (queue_begin_ == layer_end_ && !is_ending()) {
        layer_begin_ = layer_end_;
        layer_end_ = queue_end_;
        layer_offset_.emplace_back(layer_end_);
    }
}

template <typename T>
requires std::is_trivial_v<T>
[[nodiscard]] bool LayerQueue<T>::is_ending() const {
    return queue_begin_ == queue_end_;
}

template <typename T>
requires std::is_trivial_v<T>
[[nodiscard]] bool LayerQueue<T>::is_new_layer() const {
    return queue_begin_ == layer_begin_;
}

template <typename T>
requires std::is_trivial_v<T>
std::vector<T> LayerQueue<T>::last_layer() const {
    return {data_ + layer_begin_, data_ + layer_end_};
}

template <typename T>
requires std::is_trivial_v<T>
std::vector<std::vector<T>> LayerQueue<T>::all_layers() const {
    std::vector<std::vector<T>> result;
    result.reserve(layer_offset_.size() - 1);
    for (size_t i = 0; i < layer_offset_.size() - 1; ++i) {
        result.emplace_back(std::vector<T> {
            data_ + layer_offset_[i],
            data_ + layer_offset_[i + 1]
        });
    }
    return result;
}

} // namespace klotski
