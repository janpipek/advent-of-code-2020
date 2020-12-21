// Originally from https://github.com/janpipek/g4multiarray/blob/master/multi_array.hh

#ifndef MultiArray_h
#define MultiArray_h

#include <valarray>
#include <array>
#include <ostream>
#include <functional>
#include <vector>

// Forward definition of types
template<typename T, size_t N>
class multi_array;

template<typename T, size_t N>
class multi_array_view;

template<typename T, size_t N>
class multi_array_view_const;

template<typename T, size_t N, template<typename, size_t> class data_policy>
class multi_array_base;

template<typename T>
multi_array<T, 1> asarray(const std::vector<T> &);

template<typename T, size_t N>
multi_array<T, 1> asarray(const std::array<T, N> &);

template<size_t N>
size_t get_product(const std::array<size_t, N> &arr) {
    size_t total = 1;
    for (size_t i = 0; i < N; i++) {
        total *= arr[i];
    }
    return total;
}

template<size_t N>
struct slicer_base {
public:
    static constexpr size_t new_dim(int M) { return (N == 1) ? (M - 1) : M; }

    // template<size_t M> using target_array_type = std::array<size_t, new_dim(M)>;

    // template<size_t M> using target_pair_type = std::pair<target_array_type<M>, target_array_type<M>>;

    template<class... Ts>
    constexpr slicer_base(Ts... args)
            : fNumbers({args...}) {
        static_assert(N <= 3, "Cannot have slicer with dim > 3");
    }

protected:
    std::array<int, N> fNumbers;
};

template<size_t N>
struct slicer : public slicer_base<N> {
    using slicer_base<N>::new_dim;

    static constexpr size_t deltaDim = 0;

    using slicer_base<N>::slicer_base;

    using slicer_base<N>::fNumbers;

    template<size_t M>
    std::tuple<size_t, std::array<size_t, new_dim(M)>, std::array<size_t, new_dim(M)>>
    apply(size_t offset, std::array<size_t, M> shape, std::array<size_t, M> strides, size_t I) const {
        if (N == 0) {
            return std::make_tuple(offset, shape, strides);     // All things equal :-)
        } else {
            if (fNumbers[0] > fNumbers[1]) {
                throw std::runtime_error("TODO: something intelligent");
            }
            if ((fNumbers[1] > shape[I]) || (fNumbers[0] >= shape[I])) {
                throw std::runtime_error("TODO: something intelligent");
            }

            size_t newOffset = offset + strides[I] * fNumbers[0];
            std::array<size_t, new_dim(M)> newShape;
            std::array<size_t, new_dim(M)> newStrides;

            size_t iStride = (N == 2) ? 1 : fNumbers[2];
            for (int i = 0; i < I; i++) {
                newShape[i] = shape[i];
                newStrides[i] = strides[i];
            }
            newShape[I] = (fNumbers[1] - fNumbers[0]);
            newStrides[I] = strides[I] * iStride;
            if (iStride > 1) {
                ((newShape[I] -= 1) /= iStride) += 1;
            }
            for (int j = I + 1; j < new_dim(M); j++) {
                newShape[j] = shape[j];
                newStrides[j] = strides[j];
            }

            return std::make_tuple(newOffset, newShape, newStrides);
        }
    }
};

/* template<class... Ts> multi_array_view<T, slicer_result<T, N, Ts...>
{
}

template<class T1, class... Ts> multi_array_view<T, slicer_result<T, N, T1, Ts...>
{
    static constexpr size_t
}*/

template<>
struct slicer<1> : public slicer_base<1> {
    using slicer_base<1>::new_dim;

    using slicer_base<1>::slicer_base;

    using slicer_base<1>::fNumbers;

    static constexpr size_t deltaDim = 1;

    template<size_t M>
    std::tuple<size_t, std::array<size_t, new_dim(M)>, std::array<size_t, new_dim(M)>>
    apply(size_t offset, std::array<size_t, M> shape, std::array<size_t, M> strides, size_t I) const {
        if (fNumbers[0] >= (int) shape[I]) {
            throw std::runtime_error("TODO: something intelligent");
        }
        size_t start;
        if (fNumbers[0] >= 0) {
            start = (size_t) fNumbers[0];
        } else {
            start = shape[I] + fNumbers[0];
        }

        size_t newOffset = offset + strides[I] * start;
        std::array<size_t, new_dim(M)> newShape;
        std::array<size_t, new_dim(M)> newStrides;
        for (int i = 0; i < I; i++) {
            newShape[i] = shape[i];
            newStrides[i] = strides[i];
        }
        for (int j = I; j < new_dim(M); j++) {
            newShape[j] = shape[j + 1];
            newStrides[j] = strides[j + 1];
        }
        return std::make_tuple(newOffset, newShape, newStrides);     // All things equal :-)
    }
};

template<class... Ts>
constexpr slicer<(sizeof...(Ts))> make_slicer(Ts... others) {
    return slicer<(sizeof...(Ts))>(others...);
}

struct slice_helper {
    template<typename... Ts>
    slicer<sizeof...(Ts)> operator()(Ts... args) const {
        return make_slicer(args...);
    }
};

constexpr slice_helper _;

// template<typename T, size_t N, template<typename, size_t> class data_policy> class multi_array_base

/** Creates strides for a regular array. **/
template<size_t N>
std::array<size_t, N> get_strides(const std::array<size_t, N> &arr) {
    std::array<size_t, N> result;
    result[N - 1] = 1;
    for (size_t i = 1; i < N; i++) {
        result[N - 1 - i] = result[N - i] * arr[N - i];
    }
    return result;
}

/** Creates gslice for a view. **/
template<size_t N>
std::gslice get_gslice(size_t offset, std::array<size_t, N> shape, std::array<size_t, N> strides) {
    std::valarray<size_t> shape_(N);
    std::valarray<size_t> strides_(N);
    std::copy(shape.begin(), shape.end(), std::begin(shape_));
    std::copy(strides.begin(), strides.end(), std::begin(strides_));
    return std::gslice(offset, shape_, strides_);
}

template<size_t N>
class index_impl {
public:
    using index_type = std::array<size_t, N>;

    index_impl(const index_type &shape)
            : fShape(shape), fStrides(get_strides(shape)), fSize(get_product(shape)) {}

    index_impl(const index_type &shape, const index_type &strides, size_t offset)
            : fShape(shape), fStrides(strides), fSize(get_product(shape)), fOffset(offset) {}

    size_t size() const { return fSize; }

    const index_type &shape() const { return fShape; }

protected:
    size_t make_index(const index_type &arr, bool check_index = true) const {
        size_t index = fOffset;
        for (int i = 0; i < N; i++) {
            if (check_index && (arr[i] >= fShape[i])) {
                throw std::runtime_error("Index overflow.");
            }
            index += fStrides[i] * arr[i];
        }
        return index;
    }

    index_type fShape;

    index_type fStrides;

    size_t fSize;

    size_t fOffset{0};
};

template<typename T, size_t N>
class array_owner_impl : public index_impl<N> {
public:
    // Type aliases
    using data_type = std::valarray<T>;
    using base_type = index_impl<N>;
    using typename base_type::index_type;

    array_owner_impl(const data_type &data, const index_type &shape)
            : base_type(shape), fData(data) {}

    array_owner_impl(const data_type &&data, const index_type &shape)
            : base_type(shape), fData(data) {}

    constexpr static bool read_write_access = true;

    const std::valarray<T> &data() const { return fData; }

protected:
    std::valarray<T> &get_data_array() { return fData; }

    void set_data(const std::valarray<T> &other) {
        fData = other;
    }

    void set_data(const T &other) {
        fData = other;
    }

    data_type fData;
};

template<typename T, size_t N, bool is_const>
class t_array_view_impl : public index_impl<N> {
public:
    constexpr static bool read_write_access = is_const;

    // Type aliases
    using data_type = typename std::conditional<is_const, const std::valarray<T> &, std::valarray<T> &>::type;
    // using data_array_type = typename std::conditional<is_const, std::valarray<T>, std::gslice_array<T>>::type;
    using base_type = index_impl<N>;
    using typename base_type::index_type;

    // Import base members
    using index_impl<N>::fStrides;
    using index_impl<N>::fShape;
    using index_impl<N>::fOffset;

    t_array_view_impl(data_type data, const index_type &shape, const index_type &strides, size_t offset)
            : base_type(shape, strides, offset), fData(data) {}

    std::valarray<T> data() const {
        return fData[get_gslice_()];
    }

private:
    std::gslice get_gslice_() const {
        auto slice = get_gslice(fOffset, fShape, fStrides);
        return slice;
    }

protected:
    data_type fData;

    /*data_array_type get_data_array()
    // Does not work with gcc :-()
    //auto get_data_array() -> decltype(this->fData[std::gslice()])
    {
        auto slize = get_gslice_();
        return fData[slize];
    }*/

    void set_data(const std::valarray<T> &other) {
        fData[get_gslice_()] = other;
    }

    void set_data(const T &other) {
        fData[get_gslice_()] = other;
    }
};

template<typename T, size_t N> using array_view_impl = t_array_view_impl<T, N, false>;

template<typename T, size_t N> using array_const_view_impl = t_array_view_impl<T, N, true>;

template<typename T, size_t N>
class array_accessor_impl {
public:
    using const_item_type = multi_array_view_const<T, N - 1>;
    using item_type = multi_array_view<T, N - 1>;

    template<template<typename, size_t> class data_policy>
    static const_item_type get_const_item(const multi_array_base<T, N, data_policy> &array, size_t i) {
        return const_item_type(array, i);
    }

    template<template<typename, size_t> class data_policy>
    static item_type get_item(multi_array_base<T, N, data_policy> &array, size_t i) {
        return item_type(array, i);
    }
};

template<typename T>
class array_accessor_impl<T, 1> {
public:
    using const_item_type = const T &;
    using item_type = T &;

    template<template<typename, size_t> class data_policy>
    static const_item_type get_const_item(const multi_array_base<T, 1, data_policy> &array, size_t i) {
        return array.at(std::array<size_t, 1>{i});
    }

    template<template<typename, size_t> class data_policy>
    static item_type get_item(multi_array_base<T, 1, data_policy> &array, size_t i) {
        return array.at(std::array<size_t, 1>{i});
    }
};

/**
  * @short multi_array-like object
  *
  * Can be either an array itself, or any of its views.
  */
template<typename T, size_t N, template<typename, size_t> class data_policy>
class multi_array_base
        : public data_policy<T, N> {
public:
    // Type aliases
    constexpr static size_t Dim = N;
    using base_type = data_policy<T, N>;
    using typename base_type::index_type;
    using const_item_type = typename std::conditional<N == 1, const T &, multi_array_view_const<T, N - 1>>::type;
    using item_type = typename std::conditional<N == 1, T &, multi_array_view<T, N - 1>>::type;
    using accessor_type = array_accessor_impl<T, N>;

    // Friends
    template<typename, size_t> friend
    class array_accessor_impl;

    template<typename, size_t, template<typename, size_t> class> friend
    class multi_array_base;

    template<typename, size_t> friend
    class multi_array;

    template<typename, size_t> friend
    class multi_array_view;

    template<typename, size_t> friend
    class multi_array_view_const;
    // template<typename, size_t> friend std::ostream& operator << (std::ostream&, const multi_array_base&);

    // Import members
    using base_type::data;

protected:
    // Import members
    using index_impl<N>::fStrides;
    using index_impl<N>::fShape;
    using index_impl<N>::fSize;
    using index_impl<N>::fOffset;
    using base_type::fData;

    using index_impl<N>::make_index;
    // using base_type::get_data_array;
    using base_type::set_data;

    using base_type::base_type;

    multi_array_base &operator=(const T &value) {
        set_data(value);
        return *this;
    }

    /* multi_array_base& operator=(const multi_array_base&) = delete;
    multi_array_base& operator=(const multi_array_base&&) = delete;*/

    template<template<typename, size_t> class data_policy2>
    multi_array_base &operator=(const multi_array_base<T, N, data_policy2> &other) {
        if (other.fShape != fShape) {
            throw std::runtime_error("Cannot assign data of different shapes.");
        }
        set_data(other.data());
        return *this;
    }

public:
    template<size_t I, class... Ts>
    typename std::enable_if<
            (slicer<sizeof...(Ts)>::new_dim(N) != 0),
            multi_array_view<T, slicer<sizeof...(Ts)>::new_dim(N)>
    >::type
    slice(Ts... slicerArguments) {
        static_assert(I < N, "TODO: write something intelligent.");
        slicer<sizeof...(Ts)> theSlicer = make_slicer(slicerArguments...);
        auto triple = theSlicer.apply(fOffset, fShape, fStrides, I);
        return multi_array_view<T, slicer<sizeof...(Ts)>::new_dim(N)>(*this, std::get<1>(triple), std::get<2>(triple),
                                                                      std::get<0>(triple));
    }

    template<size_t I, class... Ts>
    typename std::enable_if<
            (slicer<sizeof...(Ts)>::new_dim(N) == 0),
            const T &
    >::type
    slice(Ts... slicerArguments) {
        static_assert(I < N, "TODO: write something intelligent.");
        std::array<size_t, 1> ind = {slicerArguments...};
        return (*this)[ind[0]];
    }

    template<size_t I, size_t M>
    typename std::enable_if<
            (slicer<M>::new_dim(N) != 0),
            multi_array_view<T, slicer<M>::new_dim(N)>
    >::type
    slice(const slicer<M> &theSlicer) {
        auto triple = theSlicer.apply(fOffset, fShape, fStrides, I);
        return multi_array_view<T, slicer<M>::new_dim(N)>(*this, std::get<1>(triple), std::get<2>(triple),
                                                          std::get<0>(triple));
    }

    template<size_t I, size_t M>
    typename std::enable_if<
            (slicer<M>::new_dim(N) == 0),
            const T &
    >::type
    slice(const slicer<M> &theSlicer) {
        std::array<size_t, 1> ind = {theSlicer.fNumbers[0]};
        return (*this)[ind];
    }

    template<size_t I>
    multi_array_view<T, N> slice(const slice_helper &) {
        return multi_array_view<T, N>(*this, fShape, fStrides, fOffset);
    }

protected:
    template<int I, typename T1>
    auto _apply_indices(const T1 &t)
    -> decltype(slice<I>(t)) {
        return slice<I>(t);
    }

    template<int I, typename T1, typename... Ts>
    auto _apply_indices(const T1 &t, Ts... indices)
    -> decltype(slice<I>(t)._apply_indices<I, Ts...>(indices...)) {
        // auto intermediate = apply_index<I>(t);
        constexpr int M = decltype(slice<I>(t))::Dim;
        constexpr int J = I + 1 + M - N;
        return slice<I>(t)._apply_indices<J, Ts...>(indices...);
    }

public:
    template<typename... Ts>
    auto operator()(Ts... indices)
    -> decltype(_apply_indices<0, Ts...>(indices...)) {
        return _apply_indices<0, Ts...>(indices...); // Works for 1!
    }

    // template<size_t I, class... Ts> const T&

    /* template<class... Ts> multi_array_view<T, slicer_result<T, N, Ts...>::type apply_indices(Ts... indices)
    {

    }*/

    template<typename U>
    multi_array<T, N> operator*(const U &other) const {
        multi_array<T, N> result = copy();
        result *= other;
        return result;
    }

    template<typename U>
    multi_array<T, N> operator/(const U &other) const {
        multi_array<T, N> result = copy();
        result /= other;
        return result;
    }

    template<typename U>
    multi_array<T, N> operator+(const U &other) const {
        multi_array<T, N> result = copy();
        result += other;
        return result;
    }

    template<typename U>
    multi_array<T, N> operator-(const U &other) const {
        multi_array<T, N> result = copy();
        result -= other;
        return result;
    }

public:
    T &at(const index_type &i) { return fData[make_index(i)]; }

    const T &at(const index_type &i) const { return fData[make_index(i)]; }

    const_item_type operator[](size_t i) const { return accessor_type::get_const_item(*this, i); }

    item_type operator[](size_t i) { return accessor_type::get_item(*this, i); }

    multi_array<T, N> copy() const { return multi_array<T, N>(*this); }

    template<size_t M>
    multi_array<T, M> resize(const std::array<size_t, M> &newShape) const {
        if (!get_product(fShape) == get_product(newShape)) {
            throw std::runtime_error("Total size of the new array must equal to the old one.");
        }
        auto aCopy = copy();
        return multi_array<T, M>(newShape, aCopy.data());
    }

    template<class... Ts>
    multi_array<T, sizeof...(Ts)> resize(Ts... dims) const {
        std::array<size_t, sizeof...(Ts)> newShape{dims...};
        if (!get_product(fShape) == get_product(newShape)) {
            throw std::runtime_error("Total size of the new array must equal to the old one.");
        }
        auto aCopy = copy();
        return multi_array<T, sizeof...(Ts)>(newShape, aCopy.data());
    }

    // Conversion
    template<typename U>
    multi_array<U, N> as() const {
        std::valarray<U> result;
        const auto &theData = data();
        result.resize(fSize);
        for (size_t i = 0; i < fSize; i++) {
            result[i] = U(theData[i]);
        }
        return multi_array<U, N>(fShape, std::move(result));
    }

    multi_array_view_const<T, N> readOnly() const {
        return multi_array_view_const<T, N>(*this);
    }

    multi_array<T, N> apply(std::function<T(T)> f) const {
        std::valarray<T> result;
        const auto &theData = data();
        result.resize(fSize);
        for (size_t i = 0; i < fSize; i++) {
            result[i] = f(theData[i]);
        }
        return multi_array<T, N>(fShape, std::move(result));
    }

    template<typename U>
    multi_array<U, N> apply(std::function<U(const T &)> f) const {
        std::valarray<U> result;
        const auto &theData = data();
        result.resize(fSize);
        for (size_t i = 0; i < fSize; i++) {
            result[i] = f(theData[i]);
        }
        return multi_array<U, N>(fShape, std::move(result));
    }
};

/**
  * @short Multi-dimensional array, owning its data.
  */
template<typename T, size_t N>
class multi_array : public multi_array_base<T, N, array_owner_impl> {
public:
    // Type aliases
    using base_type = multi_array_base; //<T, N, array_owner_impl>;
    using typename base_type::index_type;
    using typename base_type::data_type;    // std::valarray<T>

protected:
    // Import members
    using index_impl<N>::fStrides;
    using index_impl<N>::fShape;
    using index_impl<N>::fSize;
    using index_impl<N>::fOffset;
    using base_type::fData;
    using base_type::get_data_array;

public:
    using base_type::operator=;

    explicit multi_array(const index_type &shape)
            : base_type(
            data_type(get_product(shape)),
            shape) {}

    multi_array(const index_type &shape, const std::valarray<T> &data)
            : base_type(
            data,
            shape) {}

    multi_array(const index_type &shape, const T &value)
            : base_type(
            data_type(value, get_product(shape)),
            shape) {}

    multi_array(const index_type &shape, const T *data)
            : base_type(
            data_type(data, get_product(shape)),
            shape) {}

    template<template<typename, size_t> class data_policy>
    multi_array(const multi_array_base<T, N, data_policy> &other)
            : base_type(
            other.data(),
            other.shape()
    ) {}

    // Operators
    template<template<typename, size_t> class data_policy>
    multi_array &operator*=(const multi_array_base<T, N, data_policy> &other) {
        if (fShape != other.fShape) {
            throw std::runtime_error("Incompatible shapes for multiplication.");
        }
        get_data_array() *= other.data();
        return *this;
    }

    multi_array &operator*=(const T &other) {
        get_data_array() *= other;
        return *this;
    }

    template<template<typename, size_t> class data_policy>
    multi_array &operator/=(const multi_array_base<T, N, data_policy> &other) {
        if (fShape != other.fShape) {
            throw std::runtime_error("Incompatible shapes for division.");
        }
        get_data_array() /= other.data();
        return *this;
    }

    multi_array &operator/=(const T &other) {
        get_data_array() /= other;
        return *this;
    }

    template<template<typename, size_t> class data_policy>
    multi_array &operator+=(const multi_array_base<T, N, data_policy> &other) {
        if (fShape != other.fShape) {
            throw std::runtime_error("Incompatible shapes for addition.");
        }
        get_data_array() += other.data();
        return *this;
    }

    multi_array &operator+=(const T &other) {
        get_data_array() += other;
        return *this;
    }

    template<template<typename, size_t> class data_policy>
    multi_array &operator-=(const multi_array_base<T, N, data_policy> &other) {
        if (fShape != other.fShape) {
            throw std::runtime_error("Incompatible shapes for subtraction.");
        }
        get_data_array() -= other.data();
        return *this;
    }

    multi_array &operator-=(const T &other) {
        get_data_array() -= other;
        return *this;
    }


    void write(std::ostream &os) const; // TODO: Generalize
};

template<typename T, size_t N>
class multi_array_view : public multi_array_base<T, N, array_view_impl> {
public:
    // Type aliases
    using base_type = multi_array_base<T, N, array_view_impl>;
    using typename base_type::index_type;
    using typename base_type::data_type;    // std::valarray<T>

    template<typename, size_t, template<typename, size_t> typename> friend
    class multi_array_base;

    // Import members
    using base_type::operator=;

    // Constructor for selecting items
    template<template<typename, size_t> class data_policy>
    multi_array_view(const multi_array_base<T, N + 1, data_policy> &upper, size_t i)
            : base_type(
            upper.fData,
            get_shape(upper, i),
            get_strides(upper, i),
            get_offset(upper, i)
    ) {}

protected:
    // Import members
    using base_type::fShape;
    using base_type::fSize;
    // using base_type::get_data_array;
    using base_type::data;
    using base_type::set_data;

    // TODO: Add constructor for selecting items in any axis
    // TODO: Add constructor for selecting slices...

    // Constructor for reshaping of existing arrays
    template<size_t M, template<typename, size_t> class data_policy>
    multi_array_view(const multi_array_base<T, M, data_policy> &upper, const index_type &shape,
                     const index_type &strides, size_t offset = 0)
            : base_type(
            upper.fData,
            shape,
            strides,
            offset
    ) {}

public:
    template<template<typename, size_t> class data_policy>
    multi_array_view &operator*=(const multi_array_base<T, N, data_policy> &other) {
        if (fShape != other.fShape) {
            throw std::runtime_error("Incompatible shapes for multiplication.");
        }
        set_data(data() * other.data());
        // Note: for other compilers, this should work:
        //   get_data_array() *= other.data();
        //   See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=62119 - or other gcc bug related to gslice_array copy constructor
        return *this;
    }

    multi_array_view &operator*=(const T &other) {
        set_data(data() * other);
        return *this;
    }

    template<template<typename, size_t> class data_policy>
    multi_array_view &operator/=(const multi_array_base<T, N, data_policy> &other) {
        if (fShape != other.fShape) {
            throw std::runtime_error("Incompatible shapes for division.");
        }
        set_data(data() / other.data());
        return *this;
    }

    multi_array_view &operator/=(const T &other) {
        set_data(data() / other);
        return *this;
    }

    template<template<typename, size_t> class data_policy>
    multi_array_view &operator+=(const multi_array_base<T, N, data_policy> &other) {
        if (fShape != other.fShape) {
            throw std::runtime_error("Incompatible shapes for addition.");
        }
        set_data(data() + other.data());
        return *this;
    }

    multi_array_view &operator+=(const T &other) {
        set_data(data() + other);
        return *this;
    }

    template<template<typename, size_t> class data_policy>
    multi_array_view &operator-=(const multi_array_base<T, N, data_policy> &other) {
        if (fShape != other.fShape) {
            throw std::runtime_error("Incompatible shapes for subtraction.");
        }
        set_data(data() - other.data());
        return *this;
    }

    multi_array_view &operator-=(const T &other) {
        set_data(data() - other);
        return *this;
    }

private:
    template<template<typename, size_t> class data_policy>
    static index_type get_shape(const multi_array_base<T, N + 1, data_policy> &upper, size_t i) {
        index_type shape;
        for (size_t i = 0; i < N; i++) {
            shape[i] = upper.fShape[i + 1];
        }
        return shape;
    }

    template<template<typename, size_t> class data_policy>
    static index_type get_strides(const multi_array_base<T, N + 1, data_policy> &upper, size_t i) {
        index_type strides;
        for (size_t i = 0; i < N; i++) {
            strides[i] = upper.fStrides[i + 1];
        }
        return strides;
    }

    template<template<typename, size_t> class data_policy>
    static size_t get_offset(const multi_array_base<T, N + 1, data_policy> &upper, size_t i) {
        return upper.fOffset + i * upper.fStrides[0];
    }

};

template<typename T, size_t N>
class multi_array_view_const : public multi_array_base<T, N, array_const_view_impl> {
public:
    // Type aliases
    using base_type = multi_array_base<T, N, array_const_view_impl>;
    using typename base_type::index_type;
    using typename base_type::data_type;    // std::valarray<T>

    // Import members
    using index_impl<N>::fStrides;
    using index_impl<N>::fShape;
    using index_impl<N>::fSize;
    using index_impl<N>::fOffset;
    using base_type::fData;

    // Read-only view with the same properties
    template<template<typename, size_t> class data_policy>
    explicit multi_array_view_const(const multi_array_base<T, N, data_policy> &other)
            : base_type(
            other.fData,
            other.fShape,
            other.fStrides,
            other.fOffset
    ) {}

    template<size_t M, template<typename, size_t> class data_policy>
    multi_array_view_const(const multi_array_base<T, M, data_policy> &upper, const index_type &shape,
                           const index_type &strides, size_t offset = 0)
            : base_type(
            upper.fData,
            shape,
            strides,
            offset
    ) {}
};

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> operator*(const T &x, const multi_array_base<T, N, data_policy> &y) {
    return y * x;
}

/*template<typename U, typename T, size_t N, template<typename, size_t> class data_policy> multi_array<T, N> operator/ (const U& x, const multi_array_base<T, N, data_policy>& y)
{
    return y * x;
}*/

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> operator+(const T &x, const multi_array_base<T, N, data_policy> &y) {
    return y + x;
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> operator-(const T &x, const multi_array_base<T, N, data_policy> &y) {
    return -y + x;
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> operator-(const multi_array_base<T, N, data_policy> &x) {
    return T(-1) * x;
}

template<typename T, size_t N>
void multi_array<T, N>::write(std::ostream &os) const {
    os << "{";
    size_t i = 0;
    while (i < fSize) {
        if (i == 0) {
            for (size_t j = 0; j < N; j++) {
                os << "[";
            }
        } else {
            for (size_t j = 0; j + 1 < N; j++) {
                if (i % fStrides[j] == 0) {
                    for (size_t k = j; k + 1 < N; k++) {
                        os << "\n";
                    }
                    os << "  ";
                    for (size_t k = 0; k < j; k++) {
                        os << " ";
                    }
                    for (size_t k = j; k + 1 < N; k++) {
                        os << "[";
                    }
                    break;
                }
            }
        }

        os << fData[fOffset + i];
        i++;

        size_t j = 0;
        // size_t k = 0;
        for (; j + 1 < N; j++) {
            if (i % fStrides[j] == 0) {
                break;
            }
        }
        for (size_t k = j; k + 1 < N; k++) {
            os << "]";
        }
        if (i != fSize) {
            os << ", ";
        }
    }
    os << "]}";
}

template<typename T, size_t N>
std::ostream &operator<<(std::ostream &os, const multi_array<T, N> &array) {
    array.write(os);
    return os;
}

template<typename T, size_t N>
std::ostream &operator<<(std::ostream &os, const multi_array_view<T, N> &array) {
    array.copy().write(os);
    return os;
}

template<typename T, size_t N>
std::ostream &operator<<(std::ostream &os, const multi_array_view_const<T, N> &array) {
    array.copy().write(os);
    return os;
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> abs(const multi_array_base<T, N, data_policy> &arr) {
    return arr.apply((double (*)(double)) &std::abs);
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> exp(const multi_array_base<T, N, data_policy> &arr) {
    return arr.apply((double (*)(double)) &std::exp);
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> log(const multi_array_base<T, N, data_policy> &arr) {
    return arr.apply((double (*)(double)) &std::log);
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> log10(const multi_array_base<T, N, data_policy> &arr) {
    return arr.apply((double (*)(double)) &std::log10);
}

template<typename T, size_t N, template<typename, size_t> class data_policy, typename U>
multi_array<T, N> pow(const multi_array_base<T, N, data_policy> &arr, const U &exponential) {
    return multi_array<T, N>(arr.shape(), std::move(std::pow(arr.data(), T(exponential))));
}

template<typename T, size_t N, template<typename, size_t> class data_policy, typename U>
multi_array<T, N> pow(const U &base, const multi_array_base<T, N, data_policy> &arr) {
    return multi_array<T, N>(arr.shape(), std::move(std::pow(T(base), arr.data())));
}

template<typename T, size_t N, template<typename, size_t> class data_policy1,
        template<typename, size_t> class data_policy2>
multi_array<T, N>
pow(const multi_array_base<T, N, data_policy1> &arr1, const multi_array_base<T, N, data_policy1> &arr2) {
    return multi_array<T, N>(arr1.shape(), std::move(std::pow(arr1.data(), arr2.data())));
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> sqrt(const multi_array_base<T, N, data_policy> &arr) {
    return arr.apply((double (*)(double)) &std::sqrt);
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> sin(const multi_array_base<T, N, data_policy> &arr) {
    return arr.apply((double (*)(double)) &std::sin);
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> cos(const multi_array_base<T, N, data_policy> &arr) {
    return arr.apply((double (*)(double)) &std::cos);
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> tan(const multi_array_base<T, N, data_policy> &arr) {
    return arr.apply((double (*)(double)) &std::tan);
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> asin(const multi_array_base<T, N, data_policy> &arr) {
    return arr.apply((double (*)(double)) &std::asin);
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> acos(const multi_array_base<T, N, data_policy> &arr) {
    return arr.apply((double (*)(double)) &std::acos);
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> atan(const multi_array_base<T, N, data_policy> &arr) {
    return arr.apply((double (*)(double)) &std::atan);
}

template<typename T, size_t N, template<typename, size_t> class data_policy1,
        template<typename, size_t> class data_policy2>
multi_array<T, N>
atan2(const multi_array_base<T, N, data_policy1> &arr1, const multi_array_base<T, N, data_policy2> &arr2) {
    return multi_array<T, N>(arr1.shape(), std::move(std::atan2(arr1.data(), arr2.data())));
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> sinh(const multi_array_base<T, N, data_policy> &arr) {
    return arr.apply((double (*)(double)) &std::sinh);
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> cosh(const multi_array_base<T, N, data_policy> &arr) {
    return arr.apply((double (*)(double)) &std::cosh);
}

template<typename T, size_t N, template<typename, size_t> class data_policy>
multi_array<T, N> tanh(const multi_array_base<T, N, data_policy> &arr) {
    return arr.apply((double (*)(double)) &std::tanh);
}

template<typename T>
multi_array<T, 1> linspace(const T &start, const T &stop, size_t num, bool endpoint = true) {
    std::valarray<T> data(num);
    T step = (stop - start) / (endpoint ? (num - 1) : num);
    for (size_t i = 0; i < num; i++) {
        data[i] = start + i * step;
    }
    if (endpoint) { data[num - 1] = stop; } // To make the endpoint precise
    return {{num}, std::move(data)};
}

template<typename T>
multi_array<T, 1> logspace(const T &start, const T &stop, size_t num, bool endpoint = true) {
    return exp(log(10) * linspace((double) start, (double) stop, num, endpoint)).as<T>();
}

template<typename T>
multi_array<T, 1> geomspace(const T &start, const T &stop, size_t num, bool endpoint = true) {
    return logspace(log10((double) start), log10((double) stop), num, endpoint).as<T>();
}

template<typename T>
multi_array<T, 1> arange(const T &start, const T &stop, const T &step) {
    int nSteps = (stop - start) / step;
    if ((nSteps * step + start) >= stop) {
        nSteps--;
    }
    return linspace(start, start + nSteps * step, nSteps + 1);
}

template<typename T>
multi_array<T, 1> arange(const T &start, const T &stop) {
    T one{1};
    return arange(start, stop, one);
}

template<typename T>
multi_array<T, 1> arange(const T &stop) {
    T zero{0};
    T one{1};
    return arange(zero, stop, one);
}

template<typename T>
multi_array<T, 1> asarray(const std::vector<T> &data) {
    std::valarray<T> d(data.data(), data.size());
    return multi_array<T, 1>({data.size()}, d);
}

template<typename T, size_t N>
multi_array<T, 1> asarray(const std::array<T, N> &data) {
    std::valarray<T> d(N);
    for (int i = 0; i < N; i++) {
        d[i] = data[i];
    }
    return multi_array<T, 1>({N}, d);
}

template<typename U, typename... Ts>
multi_array<U, sizeof...(Ts)> zeros(Ts... args) {
    std::array<size_t, sizeof...(Ts)> shape{args...};
    return multi_array<U, sizeof...(Ts)>(shape);
}

template<typename T, int N, template<typename, size_t> typename data_policy>
multi_array<T, N> zeroslike(const multi_array_base<T, N, data_policy> &other) {
    return multi_array<T, N>(other.shape());
}

template<typename U, typename... Ts>
multi_array<U, sizeof...(Ts)> ones(Ts... args) {
    return zeros<U>(args...) + U(1);
}

/**
  * @short Vectorized function.
  *
  * Best created using the vectorize(f) call.
  */
template<typename T, typename U>
class ufunc_type {
public:
    using function_type = std::function<T(const U &)>;

    explicit ufunc_type(function_type f) : fWrapped(f) {}

    template<size_t N, template<typename, size_t> typename data_policy>
    multi_array<T, N> operator()(const multi_array_base<U, N, data_policy> &other) {
        return other.apply(fWrapped);
    }

private:
    function_type fWrapped;
};

template<typename T, typename U> ufunc_type<T, U> _make_vectorized(std::function < T(
const U&)>& f)
{
return
ufunc_type<T, U>(f);
}

/**
  * @short Create a vectorized function, that is applicable to array of any shape.
  */
template<typename T, typename U> ufunc_type<T, U> vectorize(std::function < T(
const U&)>& f)
{
return
_make_vectorized(f);
}

template<typename T, typename U>
ufunc_type<T, U> vectorize(std::function < T(U) > &f) {
    std::function<T(const U &)> referenced = [f](const U &x) { return f(x); };
    return _make_vectorized(referenced);
}

template<typename T, typename U>
ufunc_type<T, U> vectorize(T (*f)(const U &)) {
    std::function<T(const U &)> referenced = f;
    return _make_vectorized(referenced);
}

template<typename T, typename U>
ufunc_type<T, U> vectorize(T (*f)(U)) {
    std::function<T(const U &)> referenced = [f](const U &x) { return f(x); };
    return _make_vectorized(referenced);
}

#endif
