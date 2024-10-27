#pragma once

namespace klotski {

template <typename T>
inline LayerQueue<T>::LayerQueue(size_t reserve, std::initializer_list<T> first_layer) {
    data_.resize(reserve);
    for (auto x : first_layer) {
        emplace(x);
    }
    layer_end_ = first_layer.size();
    layers_.reserve(139);
    layers_.emplace_back(0, layer_end_);
}

template <typename T>
void LayerQueue<T>::emplace(T item) {
    data_[queue_end_] = item;
    ++queue_end_;
}

template <typename T>
T LayerQueue<T>::current() const {
    return data_[queue_begin_];
}

template <typename T>
void LayerQueue<T>::next() {
    ++queue_begin_;
    if (queue_begin_ == layer_end_ && !is_ending()) {
        layer_begin_ = layer_end_;
        layer_end_ = queue_end_;
        layers_.emplace_back(layer_begin_, layer_end_);
    }
}

template <typename T>
[[nodiscard]] bool LayerQueue<T>::is_ending() const {
    return queue_begin_ == queue_end_;
}

template <typename T>
[[nodiscard]] bool LayerQueue<T>::is_new_layer() const {
    return queue_begin_ == layer_begin_;
}

template <typename T>
std::vector<T> LayerQueue<T>::layer_cases() const {
    std::vector<T> layer_cases;
    for (size_t offset = layer_begin_; offset < layer_end_; ++offset) {
        layer_cases.emplace_back(data_[offset]);
    }
    return layer_cases;
}

} // namespace klotski
