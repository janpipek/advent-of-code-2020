#ifndef ADVENT_OF_CODE_NDARRAY_HH
#define ADVENT_OF_CODE_NDARRAY_HH

#include <numeric>
#include <vector>

#include "mathematics.hh"

using index_item_t = int64_t;
using index_t = std::vector<index_item_t>;

/**
 * Multidimensional (numeric) array.
 *
 * @tparam T Element type
 */
template<typename T>
class NDArray {
public:
    NDArray(const std::vector<size_t>& shape);

    NDArray(const std::vector<size_t>& shape, const index_t& offsets);

    NDArray(const std::vector<size_t>& shape, const index_t& offsets, const std::vector<T>& data);

    // NDArray(const std::vector<size_t>& shape, const T& value);

    auto& at(const index_t& index) {
        auto internal = internalIndex(index);
        return _data.at(internal);
    }

    const auto& at(const index_t& index) const {
        auto internal = internalIndex(index);
        return _data.at(internal);
    }

    void set(const index_t& index, const T& value) {
        if (!isValidIndex(index)) {
            throw std::runtime_error("Invalid index.");
        }
        size_t internal = 0;
        for (size_t i = 0; i < _shape.size(); i++) {
            internal += size_t((index[i] - _offsets[i]) * _strides[i]);
        }
        _data[internal] = value;
    }

    bool isValidIndex(const index_t& index) const {
        for (size_t i = 0; i < _shape.size(); i++) {
            if ((index[i] < _offsets[i]) || (index[i] >= _shape[i] + _offsets[i])) {
                return false;
            }
        }
        return true;
    }

    const std::vector<size_t>& shape() const {
        return _shape;
    }

    const index_t& offsets() const {
        return _offsets;
    }

    size_t size() const {
        return product(_shape);
    }

    size_t ndim() const {
        return _shape.size();
    }

    T sum() const {
        return std::accumulate(_data.cbegin(), _data.cend(), 0, std::plus<T>());
    }

    NDArray<T> swapAxes(size_t axis1, size_t axis2) const {
        if (axis1 >= ndim() || axis2 >= ndim()) {
            throw std::runtime_error("Invalid axes to swap.");
        }
        NDArray<T> result = *this;
        std::swap(result._shape[axis1], result._shape[axis2]);
        std::swap(result._strides[axis1], result._strides[axis2]);
        std::swap(result._offsets[axis1], result._offsets[axis2]);
        return result;
    }

    NDArray<T> flip(size_t axis) const {
        if (axis >= ndim()) {
            throw std::runtime_error("Invalid axis to flip.");
        }
        
        if (ndim() != 2) {
            // TODO: Make this general
            throw std::runtime_error("Not implemented.");
        } else {
            NDArray<T> result(_shape, _offsets);
            size_t other = 1 - axis;
            int minI = _offsets[axis];
            int maxI = _offsets[axis] + _shape[axis];
            for (int i = minI; i < maxI; i++) {
                int targetI = maxI - (i - minI) - 1;
                for (int j = _offsets[other]; j < _offsets[other] + _shape[other]; j++) {
                    index_t sourceIndex = (axis == 0) ? index_t({i, j}) : index_t({j, i});
                    index_t targetIndex = (axis == 0) ? index_t({targetI, j}) : index_t({j, targetI});
                    result.at(targetIndex) = at(sourceIndex);
                }
            }
            return result;
        }
    }

    NDArray<T>& operator=(const T& val) {
        _data = std::vector(size(), val);
    }

private:
    NDArray() { }

    std::vector<T> _data;

    std::vector<size_t> _shape;

    std::vector<size_t> _strides;

    index_t _offsets;

    void _calculateStrides();

    size_t internalIndex(const index_t& index) const {
        if (!isValidIndex(index)) {
            throw std::runtime_error("Invalid index.");
        }
        size_t internal = 0;
        for (size_t i = 0; i < _shape.size(); i++) {
            internal += size_t((index[i] - _offsets[i]) * _strides[i]);
        }
        return internal;
    }
};

template<typename T> NDArray<T>::NDArray(const std::vector<size_t>& shape) : _shape(shape), _data(product(shape)), _offsets(shape.size(), 0) {
    _calculateStrides();
}

template<typename T> NDArray<T>::NDArray(const std::vector<size_t>& shape, const index_t& offsets) {
    if (offsets.size() != shape.size()) {
        throw std::runtime_error("Invalid offset size.");
    }
    _shape = shape;
    _data = std::vector<T>(product(shape));
    _offsets = offsets;
    _calculateStrides();
}

template<typename T> NDArray<T>::NDArray(const std::vector<size_t>& shape, const index_t& offsets, const std::vector<T>& data) {
    if (data.size() != product(shape)) {
        throw std::runtime_error("Invalid shape.");
    }
    if (shape.size() != offsets.size()) {
        throw std::runtime_error("Invalid offsets.");
    }
    _shape = shape;
    _data = data;
    _offsets = offsets;
    _calculateStrides();
}

template<typename T> void NDArray<T>::_calculateStrides() {
    _strides = std::vector<size_t>(ndim());
    (*_strides.rbegin()) = 1;
    if (ndim() == 1) {
        return;
    }
    for (int i = _strides.size() - 2; i >= 0; i--) {
        _strides[i] = _strides[i + 1] * _shape[i + 1];
    }
}

#endif
