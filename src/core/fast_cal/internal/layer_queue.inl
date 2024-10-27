#pragma once

namespace klotski {

template <typename T>
LayerQueue<T>::LayerQueue(std::initializer_list<T> first_layer, const size_t reserve) {
    // data_.resize(reserve);

    static_assert(std::is_trivial_v<T>);

    // data_ = new T[reserve];
    data_ = (T*)std::malloc(sizeof(T) * reserve);

    queue_begin_ = 0;
    queue_end_ = 0;
    for (auto x : first_layer) {
        emplace(x);
    }

    layer_begin_ = 0;
    layer_end_ = first_layer.size();

    layers_.reserve(139);
//    layers_.emplace_back(0, layer_end_);
    layers_.emplace_back(0);
    layers_.emplace_back(layer_end_);
}

template <typename T>
LayerQueue<T>::~LayerQueue() {
    // delete []data_;
    std::free(data_);
}

template <typename T>
void LayerQueue<T>::emplace(T node) {
    data_[queue_end_] = node;
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
//        layers_.emplace_back(layer_begin_, layer_end_);
        layers_.emplace_back(layer_end_);
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

//template <typename T>
//std::vector<T> LayerQueue<T>::layer_cases() const {
//    std::vector<T> layer_cases;
//    for (size_t offset = layer_begin_; offset < layer_end_; ++offset) {
//        layer_cases.emplace_back(data_[offset]);
//    }
//    return layer_cases;
//}

template <typename T>
std::vector<T> LayerQueue<T>::last_layer() const {
    std::vector<T> layer;
    layer.reserve(layer_end_ - layer_begin_);
    for (auto offset = layer_begin_; offset < layer_end_; ++offset) {
        layer.emplace_back(data_[offset]); // TODO: copy directly
    }
    return layer;
}

template <typename T>
std::vector<std::vector<T>> LayerQueue<T>::all_layers() const {
    std::vector<std::vector<T>> result;
    for (size_t i = 0; i < layers_.size() - 1; ++i) {
        size_t begin = layers_[i];
        size_t end = layers_[i + 1];

        std::vector<T> layer {};
        layer.reserve(end - begin);
        for (auto offset = begin; offset < end; ++offset) {
            layer.emplace_back(data_[offset]); // TODO: copy directly
        }
        result.emplace_back(layer);
    }
    return result;
}

} // namespace klotski
