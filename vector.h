#include <initializer_list>
#include <type_traits>
#include <stdexcept>
#include <iterator>
#include <cstddef>
#include <utility>
#include <ostream>
#include <memory>

namespace coolstd {
template <class T, class Allocator = std::allocator<T>>
class vector {
public:
    class ConstIterator;

    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator() : ptr_(nullptr){};
        explicit Iterator(pointer ptr) : ptr_(ptr){};
        Iterator(const Iterator& a) = default;
        Iterator(Iterator&& a) noexcept = default;
        ~Iterator() = default;

        Iterator& operator=(Iterator&& a) noexcept = default;
        Iterator& operator=(const Iterator& a) = default;
        friend class vector;

        reference operator*() const {
            return *ptr_;
        }

        pointer operator->() const {
            return ptr_;
        }

        reference operator[](difference_type n) const {
            return *(std::next(ptr_, n));
        }

        Iterator& operator++() {
            ptr_ = std::next(ptr_, 1);
            return *this;
        }

        Iterator operator++(int) {
            Iterator it(*this);
            ptr_ = std::next(ptr_, 1);
            return it;
        }

        Iterator& operator--() {
            ptr_ = std::next(ptr_, -1);
            return *this;
        }

        Iterator operator--(int) {
            Iterator it(*this);
            ptr_ = std::next(ptr_, -1);
            return it;
        }

        Iterator& operator+=(difference_type n) {
            ptr_ = std::next(ptr_, n);
            return *this;
        }

        Iterator& operator-=(difference_type n) {
            ptr_ = std::next(ptr_, -n);
            return *this;
        }

        Iterator operator+(difference_type n) const {
            return Iterator(std::next(ptr_, n));
        }

        friend Iterator operator+(difference_type n, const Iterator& it) {
            return it + n;
        }

        Iterator operator-(difference_type n) const {
            return Iterator(std::next(ptr_, -n));
        }

        difference_type operator-(const Iterator& it) const {
            return std::distance(it.ptr_, ptr_);
        }

        auto operator<=>(const Iterator& rhs) const = default;

        operator ConstIterator() const {
            return ConstIterator(ptr_);
        }

    private:
        pointer ptr_;
    };

    class ConstIterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = const T;
        using difference_type = ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        ConstIterator() : ptr_(nullptr){};
        explicit ConstIterator(pointer ptr) : ptr_(ptr){};
        ConstIterator(const ConstIterator& a) = default;
        ConstIterator(ConstIterator&& a) noexcept = default;
        ~ConstIterator() = default;

        ConstIterator& operator=(ConstIterator&& a) noexcept = default;
        ConstIterator& operator=(const ConstIterator& a) = default;
        friend class vector;

        reference operator*() const {
            return *ptr_;
        }

        pointer operator->() const {
            return ptr_;
        }

        reference operator[](difference_type n) const {
            return *(std::next(ptr_, n));
        }

        ConstIterator& operator++() {
            ptr_ = std::next(ptr_, 1);
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator it(*this);
            ptr_ = std::next(ptr_, 1);
            return it;
        }

        ConstIterator& operator--() {
            ptr_ = std::next(ptr_, -1);
            return *this;
        }

        ConstIterator operator--(int) {
            ConstIterator it(*this);
            ptr_ = std::next(ptr_, -1);
            return it;
        }

        ConstIterator& operator+=(difference_type n) {
            ptr_ = std::next(ptr_, n);
            return *this;
        }

        ConstIterator& operator-=(difference_type n) {
            ptr_ = std::next(ptr_, -n);
            return *this;
        }

        ConstIterator operator+(difference_type n) const {
            return ConstIterator(std::next(ptr_, n));
        }

        friend ConstIterator operator+(difference_type n, const ConstIterator& it) {
            return it + n;
        }

        ConstIterator operator-(difference_type n) const {
            return ConstIterator(std::next(ptr_, -n));
        }

        difference_type operator-(const ConstIterator& it) const {
            return std::distance(it.ptr_, ptr_);
        }

        auto operator<=>(const ConstIterator& rhs) const = default;

    private:
        pointer ptr_;
    };

    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = std::allocator_traits<Allocator>::pointer;
    using const_pointer = std::allocator_traits<Allocator>::const_pointer;
    using iterator = Iterator;
    using const_iterator = ConstIterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // construct/copy/destroy
    constexpr vector() noexcept(noexcept(Allocator())) : vector(Allocator()) {
    }
    constexpr explicit vector(const Allocator& alloc) noexcept;

    constexpr explicit vector(size_type count, const Allocator& alloc = Allocator());
    constexpr vector(size_type count, const T& value, const Allocator& alloc = Allocator());

    template <class InputIterator>
    constexpr vector(
        InputIterator first, InputIterator last, const Allocator& = Allocator(),
        typename std::enable_if<std::is_base_of<
            std::input_iterator_tag,
            typename std::iterator_traits<InputIterator>::iterator_category>::value>::type* =
            nullptr);
    constexpr vector(const vector& x);
    constexpr vector(vector&&) noexcept;

    constexpr vector(const vector&, const Allocator&);
    constexpr vector(vector&&, const Allocator&);

    constexpr vector(std::initializer_list<T>, const Allocator& = Allocator());

    constexpr ~vector();

    constexpr vector& operator=(const vector& x);
    constexpr vector& operator=(vector&& x) noexcept(
        std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
        std::allocator_traits<Allocator>::is_always_equal::value);
    constexpr vector& operator=(std::initializer_list<T>);

    template <class InputIterator>
    constexpr void assign(
        InputIterator first, InputIterator last,
        typename std::enable_if<std::is_base_of<
            std::input_iterator_tag,
            typename std::iterator_traits<InputIterator>::iterator_category>::value>::type* =
            nullptr);
    constexpr void assign(size_type count, const T& value);
    constexpr void assign(std::initializer_list<T>);
    constexpr allocator_type get_allocator() const noexcept {
        return allocator;
    }

    // iterators
    constexpr iterator begin() noexcept {
        return iterator(data_);
    }
    constexpr iterator end() noexcept {
        return iterator(data_ + sz_);
    }
    constexpr const_iterator begin() const noexcept {
        return const_iterator(data_);
    }
    constexpr const_iterator end() const noexcept {
        return const_iterator(data_ + sz_);
    }
    constexpr const_iterator cbegin() const noexcept {
        return const_iterator(data_);
    }
    constexpr const_iterator cend() const noexcept {
        return const_iterator(data_ + sz_);
    }

    constexpr reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }
    constexpr const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }
    constexpr reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }
    constexpr const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }
    constexpr const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }
    constexpr const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    // capacity
    constexpr bool empty() const noexcept {
        return (sz_ == 0);
    }
    constexpr size_type size() const noexcept {
        return sz_;
    }
    constexpr size_type capacity() const noexcept {
        return cap_;
    }
    constexpr size_type max_size() const noexcept {
        return std::allocator_traits<Allocator>::max_size(allocator) /
               2;  // idk, looks like std::vector's max smaller as it could be
    }
    constexpr void resize(size_type count);
    constexpr void resize(size_type count, const T& value);
    constexpr void reserve(size_type count);
    constexpr void shrink_to_fit();

    // element access
    constexpr reference operator[](size_type n) {
        return data_[n];
    }
    constexpr const_reference operator[](size_type n) const {
        return data_[n];
    }
    constexpr reference at(size_type pos);
    constexpr const_reference at(size_type pos) const;

    constexpr reference front() {
        return data_[0];
    }
    constexpr const_reference front() const {
        return data_[0];
    }
    constexpr reference back() {
        return data_[sz_ - 1];
    }
    constexpr const_reference back() const {
        return data_[sz_ - 1];
    }

    // data access
    constexpr T* data() noexcept {
        return data_;
    }
    constexpr const T* data() const noexcept {
        return data_;
    }

    // modifiers
    template <class... Args>
    constexpr reference emplace_back(Args&&... args);
    constexpr void push_back(const T& value);
    constexpr void push_back(T&& value);
    constexpr void pop_back();

    template <class... Args>
    constexpr iterator emplace(const_iterator position, Args&&... args);
    constexpr iterator insert(const_iterator position, const T& value);
    constexpr iterator insert(const_iterator position, T&& x);
    constexpr iterator insert(const_iterator position, size_type count, const T& value);
    template <class InputIterator>
    constexpr iterator insert(
        const_iterator position, InputIterator first, InputIterator last,
        typename std::enable_if<std::is_base_of<
            std::input_iterator_tag,
            typename std::iterator_traits<InputIterator>::iterator_category>::value>::type* =
            nullptr);
    constexpr iterator insert(const_iterator position, std::initializer_list<T> il);

    constexpr iterator erase(const_iterator position);
    constexpr iterator erase(const_iterator first, const_iterator last);

    constexpr void swap(vector&) noexcept(
        std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
        std::allocator_traits<Allocator>::is_always_equal::value);
    constexpr void clear() noexcept {
        destroyRange(data_, data_ + sz_);
        sz_ = 0;
    }

private:
    size_type sz_ = 0;
    size_type cap_ = 0;
    T* data_ = nullptr;
    Allocator allocator;

    // helpers
    template <class InputIterator>
    void assignRangeForward(InputIterator from, InputIterator to, pointer destination);

    template <class InputIterator>
    void assignRangeBackward(InputIterator from, InputIterator to, pointer destination);

    template <class InputIterator>
    void moveRangeForward(InputIterator from, InputIterator to, pointer destination);

    template <class InputIterator>
    void copyRangeForward(InputIterator from, InputIterator to, pointer destination);

    void copyRangeForward(pointer from, pointer to, const_reference value);

    template <class InputIterator>
    void copyRangeBackward(InputIterator from, InputIterator to, pointer destination);

    T* grow(size_type newCap, bool copy = false, size_type gapIndex = 0, size_type gapSize = 0);

    void destroyRange(pointer from, pointer to);
    void destroyPointer(pointer ptr);
};

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(const Allocator& alloc) noexcept
    : sz_(0), cap_(0), data_(nullptr), allocator(alloc) {
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(size_type count, const Allocator& alloc)
    : sz_(0), cap_(count), data_(nullptr), allocator(alloc) {

    data_ = std::allocator_traits<Allocator>::allocate(allocator, cap_);

    for (; sz_ < count; ++sz_) {
        std::allocator_traits<Allocator>::construct(allocator, data_ + sz_);
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(size_type count, const T& value, const Allocator& alloc)
    : sz_(0), cap_(count), data_(nullptr), allocator(alloc) {

    data_ = std::allocator_traits<Allocator>::allocate(allocator, cap_);

    for (; sz_ < count; ++sz_) {
        std::allocator_traits<Allocator>::construct(allocator, data_ + sz_, value);
    }
}

template <class T, class Allocator>
template <class InputIterator>
constexpr vector<T, Allocator>::vector(
    InputIterator first, InputIterator last, const Allocator& alloc,
    typename std::enable_if<std::is_base_of<
        std::input_iterator_tag,
        typename std::iterator_traits<InputIterator>::iterator_category>::value>::type*)
    : sz_(0), data_(nullptr), allocator(alloc) {
    const size_type count = size_type(last - first);

    cap_ = count;
    data_ = std::allocator_traits<Allocator>::allocate(allocator, cap_);

    for (; sz_ < count; ++sz_) {
        std::allocator_traits<Allocator>::construct(allocator, data_ + sz_, *(first + sz_));
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(const vector& copyVector)
    : sz_(0), cap_(copyVector.capacity()), data_(nullptr), allocator(copyVector.allocator) {

    if (cap_ > 0) {
        data_ = std::allocator_traits<Allocator>::allocate(allocator, cap_);
    }

    for (; sz_ < copyVector.size(); ++sz_) {
        std::allocator_traits<Allocator>::construct(allocator, data_ + sz_, copyVector[sz_]);
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(const vector& copyVector, const Allocator& alloc)
    : sz_(0), cap_(copyVector.capacity()), data_(nullptr), allocator(alloc) {

    if (cap_ > 0) {
        data_ = std::allocator_traits<Allocator>::allocate(allocator, cap_);
    }

    for (; sz_ < copyVector.size(); ++sz_) {
        std::allocator_traits<Allocator>::construct(allocator, data_ + sz_, copyVector[sz_]);
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(vector&& moveVector) noexcept
    : sz_(moveVector.size()), cap_(moveVector.capacity()), data_(moveVector.data_) {
    moveVector.sz_ = 0;
    moveVector.cap_ = 0;
    moveVector.data_ = nullptr;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(vector&& moveVector, const Allocator& alloc)
    : sz_(moveVector.size()),
      cap_(moveVector.capacity()),
      data_(moveVector.data_),
      allocator(alloc) {
    moveVector.sz_ = 0;
    moveVector.cap_ = 0;
    moveVector.data_ = nullptr;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(std::initializer_list<value_type> initializerList,
                                       const Allocator& alloc)
    : sz_(0), cap_(initializerList.size()), data_(nullptr), allocator(alloc) {
    if (cap_ > 0) {
        data_ = std::allocator_traits<Allocator>::allocate(allocator, cap_);
    }

    for (; sz_ < initializerList.size(); ++sz_) {
        std::allocator_traits<Allocator>::construct(allocator, data_ + sz_,
                                                    *(initializerList.begin() + sz_));
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::~vector() {
    destroyRange(data_, data_ + sz_);
    destroyPointer(data_);

    sz_ = 0;
    cap_ = 0;
    data_ = nullptr;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>& vector<T, Allocator>::operator=(const vector& copyVector) {
    if (this != &copyVector) {
        assign(copyVector.begin(), copyVector.end());
    }

    return *this;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>& vector<T, Allocator>::operator=(vector&& moveVector) noexcept(
    std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
    std::allocator_traits<Allocator>::is_always_equal::value) {
    if (this == &moveVector) {
        return *this;
    }

    clear();
    swap(moveVector);

    return *this;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>& vector<T, Allocator>::operator=(
    std::initializer_list<value_type> initializerList) {
    assign(initializerList);

    return *this;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::reference vector<T, Allocator>::at(size_type pos) {
    if (pos < sz_) {
        return data_[pos];
    }

    throw(std::out_of_range("Pos is out-of-range!"));
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type pos) const {
    if (pos < sz_) {
        return data_[pos];
    }

    throw(std::out_of_range("Pos is out-of-range!"));
}

template <class T, class Allocator>
template <class InputIterator>
constexpr void vector<T, Allocator>::assign(
    InputIterator first, InputIterator last,
    typename std::enable_if<std::is_base_of<
        std::input_iterator_tag,
        typename std::iterator_traits<InputIterator>::iterator_category>::value>::type*) {
    const size_type count = size_type(last - first);

    size_type copied = 0;

    if (count > capacity()) {
        value_type* newData = std::allocator_traits<Allocator>::allocate(allocator, count);
        ;

        for (; (copied < count) && (first != last); ++copied) {
            std::allocator_traits<Allocator>::construct(allocator, newData + copied, *(first++));
        }

        destroyRange(data_, data_ + sz_);
        destroyPointer(data_);

        data_ = newData;
        cap_ = count;
    } else {

        for (; (copied < size()) && (copied < count); ++copied) {
            *(data_ + copied) = *(first++);
        }

        for (; copied < count; ++copied) {
            std::allocator_traits<Allocator>::construct(allocator, data_ + copied, *(first++));
        }

        for (size_type index = size(); index > count; --index) {
            std::allocator_traits<Allocator>::destroy(allocator, data_ + index - 1);
        }
    }

    sz_ = count;
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::assign(size_type count, const T& value) {
    size_type copied = 0;

    if (count > capacity()) {
        value_type* newData = std::allocator_traits<Allocator>::allocate(allocator, count);

        for (; copied < count; ++copied) {
            std::allocator_traits<Allocator>::construct(allocator, newData + copied, value);
        }

        destroyRange(data_, data_ + sz_);
        destroyPointer(data_);

        data_ = newData;
        cap_ = count;
    } else {
        for (; (copied < size()) && (copied < count); ++copied) {
            *(data_ + copied) = value;
        }

        for (; copied < count; ++copied) {
            std::allocator_traits<Allocator>::construct(allocator, data_ + copied, value);
        }

        for (size_type index = size(); index > count; --index) {
            std::allocator_traits<Allocator>::destroy(allocator, data_ + index - 1);
        }
    }

    sz_ = count;
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::assign(std::initializer_list<T> initializerList) {
    size_type copied = 0;

    if (initializerList.size() > capacity()) {
        value_type* newData =
            std::allocator_traits<Allocator>::allocate(allocator, initializerList.size());

        for (; copied < initializerList.size(); ++copied) {
            std::allocator_traits<Allocator>::construct(allocator, newData + copied,
                                                        *(initializerList.begin() + copied));
        }

        destroyRange(data_, data_ + sz_);
        destroyPointer(data_);

        data_ = newData;
        cap_ = initializerList.size();
    } else {

        for (; (copied < size()) && (copied < initializerList.size()); ++copied) {
            *(data_ + copied) = *(initializerList.begin() + copied);
        }

        for (; copied < initializerList.size(); ++copied) {
            std::allocator_traits<Allocator>::construct(allocator, data_ + copied,
                                                        *(initializerList.begin() + copied));
        }

        for (size_type index = size(); index > initializerList.size(); --index) {
            std::allocator_traits<Allocator>::destroy(allocator, data_ + index - 1);
        }
    }

    sz_ = initializerList.size();
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::push_back(const T& value) {
    if (size() == capacity()) {
        value_type* newData = grow(2 * cap_, true);

        destroyRange(data_, data_ + sz_);
        destroyPointer(data_);

        data_ = newData;
        cap_ = 2 * cap_;
    }

    std::allocator_traits<Allocator>::construct(allocator, data_ + sz_, value);

    sz_++;
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::push_back(T&& value) {
    emplace_back(std::move(value));
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::pop_back() {
    --sz_;
    std::allocator_traits<Allocator>::destroy(allocator, data_ + sz_);
}

template <class T, class Allocator>
template <class InputIterator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::insert(
    const_iterator position, InputIterator first, InputIterator last,
    typename std::enable_if<std::is_base_of<
        std::input_iterator_tag,
        typename std::iterator_traits<InputIterator>::iterator_category>::value>::type*) {
    const size_type numberOfElements = size_type(std::distance(first, last));
    const size_type positionAsIndex = size_type(position - begin());

    if (size() + numberOfElements > capacity()) {
        value_type* newData = nullptr;
        size_type copied = 0, constructed = 0;

        newData = std::allocator_traits<Allocator>::allocate(allocator, size() + numberOfElements);

        for (; (constructed < numberOfElements) && (first != last); ++constructed) {
            std::allocator_traits<Allocator>::construct(
                allocator, newData + positionAsIndex + constructed, *(first++));
        }

        for (; copied < positionAsIndex; ++copied) {
            std::allocator_traits<Allocator>::construct(allocator, newData + copied,
                                                        *(data_ + copied));
        }

        for (; copied < sz_; ++copied) {
            std::allocator_traits<Allocator>::construct(
                allocator, newData + copied + numberOfElements, *(data_ + copied));
        }

        destroyRange(data_, data_ + sz_);
        destroyPointer(data_);

        data_ = newData;
        cap_ = size() + numberOfElements;
        sz_ += numberOfElements;
    } else {
        if (numberOfElements <= (size() - positionAsIndex)) {
            moveRangeForward(end() - numberOfElements, end(), data_ + sz_);

            assignRangeForward(begin() + positionAsIndex, end() - numberOfElements,
                               data_ + positionAsIndex + numberOfElements);
        } else {
            moveRangeForward(begin() + positionAsIndex, end(),
                             data_ + (numberOfElements + positionAsIndex));
        }
        sz_ += numberOfElements;

        size_type constructed = 0;
        for (; (constructed < numberOfElements) && (first != last); ++constructed) {
            *(data_ + positionAsIndex + constructed) = *(first++);
        }
    }

    return iterator(data_ + positionAsIndex);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position,
                                                                      size_type count,
                                                                      const T& value) {
    const size_type positionAsIndex = size_type(position - begin());

    if (size() + count > capacity()) {
        value_type* newData = nullptr;
        size_type copied = 0, constructed = 0;

        newData = std::allocator_traits<Allocator>::allocate(allocator, size() + count);

        for (; constructed < count; ++constructed) {
            std::allocator_traits<Allocator>::construct(
                allocator, newData + positionAsIndex + constructed, value);
        }

        for (; copied < positionAsIndex; ++copied) {
            std::allocator_traits<Allocator>::construct(allocator, newData + copied,
                                                        *(data_ + copied));
        }

        for (; copied < sz_; ++copied) {
            std::allocator_traits<Allocator>::construct(allocator, newData + copied + count,
                                                        *(data_ + copied));
        }

        destroyRange(data_, data_ + sz_);
        destroyPointer(data_);

        data_ = newData;
        cap_ = size() + count;
        sz_ += count;
    } else {
        T temp(value);

        if (count <= (size() - positionAsIndex)) {
            moveRangeForward(end() - count, end(), data_ + sz_);
            assignRangeForward(begin() + positionAsIndex, begin() + sz_ - count,
                               data_ + positionAsIndex + count);

        } else {
            moveRangeForward(begin() + positionAsIndex, end(), data_ + (count + positionAsIndex));
        }
        sz_ += count;

        size_type constructed = 0;
        for (; constructed < count; ++constructed) {
            *(data_ + positionAsIndex + constructed) = temp;
        }
    }

    return iterator(data_ + positionAsIndex);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position,
                                                                      const T& value) {
    if (position == end()) {
        push_back(value);

        return (end() - 1);
    }

    const size_type positionAsIndex = size_type(position - begin());

    if (size() == capacity()) {
        value_type* newData = nullptr;
        size_type numOfCopied = 0;

        newData = std::allocator_traits<Allocator>::allocate(allocator, 2 * cap_);

        std::allocator_traits<Allocator>::construct(allocator, newData + positionAsIndex, value);

        for (; numOfCopied < positionAsIndex; ++numOfCopied) {
            std::allocator_traits<Allocator>::construct(allocator, newData + numOfCopied,
                                                        *(data_ + numOfCopied));
        }

        for (; numOfCopied < sz_; ++numOfCopied) {
            std::allocator_traits<Allocator>::construct(allocator, newData + numOfCopied + 1,
                                                        *(data_ + numOfCopied));
        }

        destroyRange(data_, data_ + sz_);
        destroyPointer(data_);

        data_ = newData;
        cap_ = 2 * cap_;
        ++sz_;

    } else {
        T temp(value);

        assignRangeBackward(begin() + positionAsIndex, end(), data_ + positionAsIndex + 1);
        ++sz_;

        *(data_ + positionAsIndex) = std::move(temp);
    }

    return iterator(data_ + positionAsIndex);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position,
                                                                      T&& value) {
    if (position == end()) {
        push_back(std::move(value));

        return (end() - 1);
    }

    const size_type positionAsIndex = size_type(position - begin());

    if (size() == capacity()) {
        value_type* newData = nullptr;
        size_type numOfCopied = 0;

        newData = std::allocator_traits<Allocator>::allocate(allocator, 2 * cap_);

        std::allocator_traits<Allocator>::construct(allocator, newData + positionAsIndex,
                                                    std::move(value));

        for (; numOfCopied < positionAsIndex; ++numOfCopied) {
            std::allocator_traits<Allocator>::construct(allocator, newData + numOfCopied,
                                                        *(data_ + numOfCopied));
        }

        for (; numOfCopied < sz_; ++numOfCopied) {
            std::allocator_traits<Allocator>::construct(allocator, newData + numOfCopied + 1,
                                                        *(data_ + numOfCopied));
        }

        destroyRange(data_, data_ + sz_);
        destroyPointer(data_);

        data_ = newData;
        cap_ = 2 * cap_;
        ++sz_;

    } else {
        T temp(std::move(value));

        assignRangeBackward(begin() + positionAsIndex, end(), data_ + positionAsIndex + 1);
        ++sz_;

        *(data_ + positionAsIndex) = std::move(temp);
    }

    return iterator(data_ + positionAsIndex);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::insert(
    const_iterator position, std::initializer_list<value_type> initializerList) {
    return insert(position, initializerList.begin(), initializerList.end());
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator position) {
    if (position == (end() - 1)) {
        pop_back();

        return end();
    }
    const size_type positionAsIndex = size_type(position - begin());
    assignRangeForward(begin() + 1 + positionAsIndex, end(), data_ + positionAsIndex);
    destroyRange(data_ + sz_ - 1, data_ + sz_);
    --sz_;

    return iterator(data_ + (position - begin()));
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator first,
                                                                     const_iterator last) {
    const difference_type distance = std::distance(first, last);
    const size_type positionAsIndex = size_type(first - begin());

    assignRangeForward(begin() + positionAsIndex + distance, end(), data_ + positionAsIndex);
    destroyRange(data_ + sz_ - distance, data_ + sz_);
    sz_ -= distance;

    return iterator(data_ + positionAsIndex);
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::swap(vector& swapVector) noexcept(
    std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
    std::allocator_traits<Allocator>::is_always_equal::value) {
    if (this == &swapVector) {
        return;
    }

    std::swap(data_, swapVector.data_);
    std::swap(cap_, swapVector.cap_);
    std::swap(sz_, swapVector.sz_);
}

template <class T, class Allocator>
template <class... Args>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::emplace(const_iterator position,
                                                                       Args&&... args) {
    const size_type positionAsIndex = size_type(position - begin());

    if (size() == capacity()) {
        value_type* newData = nullptr;
        size_type numOfCopied = 0;

        newData = std::allocator_traits<Allocator>::allocate(allocator, 2 * cap_);

        std::allocator_traits<Allocator>::construct(allocator, newData + positionAsIndex,
                                                    std::forward<Args>(args)...);

        for (; numOfCopied < positionAsIndex; ++numOfCopied) {
            std::allocator_traits<Allocator>::construct(allocator, newData + numOfCopied,
                                                        *(data_ + numOfCopied));
        }

        for (; numOfCopied < sz_; ++numOfCopied) {
            std::allocator_traits<Allocator>::construct(allocator, newData + numOfCopied + 1,
                                                        *(data_ + numOfCopied));
        }

        destroyRange(data_, data_ + sz_);
        destroyPointer(data_);

        data_ = newData;
        cap_ = 2 * cap_;
        ++sz_;
    } else {
        assignRangeBackward(begin() + positionAsIndex, end(), data_ + positionAsIndex + 1);
        ++sz_;

        std::allocator_traits<Allocator>::construct(allocator, position.ptr_,
                                                    std::forward<Args>(args)...);
    }

    return iterator(data_ + positionAsIndex);
}

template <class T, class Allocator>
template <class... Args>
constexpr vector<T, Allocator>::reference vector<T, Allocator>::emplace_back(Args&&... args) {
    if (size() == capacity()) {
        value_type* newData = grow(2 * cap_, true);

        destroyRange(data_, data_ + sz_);
        destroyPointer(data_);

        data_ = newData;
        cap_ = 2 * cap_;
    }

    std::allocator_traits<Allocator>::construct(allocator, data_ + sz_,
                                                std::forward<Args>(args)...);

    ++sz_;
    return data_[sz_ - 1];
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::resize(size_type count) {
    if (count == 0) {
        clear();
        return;
    } else if (count < size()) {
        destroyRange(data_ + count, data_ + sz_);
    } else if (size() == count) {
        return;
    } else {
        size_type constructed = 0;

        if (count < capacity()) {
            for (; constructed < (count - sz_); ++constructed) {
                std::allocator_traits<Allocator>::construct(allocator, data_ + sz_ + constructed);
            }

        } else {
            value_type* newData = grow(count, true);

            for (; constructed < (count - sz_); ++constructed) {
                std::allocator_traits<Allocator>::construct(allocator, newData + sz_ + constructed);
            }

            destroyRange(data_, data_ + sz_);
            destroyPointer(data_);

            data_ = newData;
            cap_ = count;
        }
    }

    sz_ = count;
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::resize(size_type count, const T& value) {
    if (count == 0) {
        return clear();
    } else if (count < size()) {
        destroyRange(data_ + count, data_ + sz_);
    } else if (count == size()) {
        return;
    } else {
        size_type constructed = 0;

        if (count < capacity()) {

            for (; constructed < (count - sz_); ++constructed) {
                std::allocator_traits<Allocator>::construct(allocator, data_ + sz_ + constructed,
                                                            value);
            }

        } else {
            value_type* newData = grow(count, true);

            for (; constructed < (count - sz_); ++constructed) {
                std::allocator_traits<Allocator>::construct(allocator, newData + sz_ + constructed,
                                                            value);
            }

            destroyRange(data_, data_ + sz_);
            destroyPointer(data_);

            data_ = newData;
            cap_ = count;
        }
    }

    sz_ = count;
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::reserve(size_type count) {
    if (count <= capacity()) {
        return;
    }

    value_type* newData = grow(count, true);

    destroyRange(data_, data_ + sz_);
    destroyPointer(data_);
    data_ = newData;
    cap_ = count;
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::shrink_to_fit() {
    if (size() == capacity()) {
        return;
    }

    value_type* newData = std::allocator_traits<Allocator>::allocate(allocator, size());
    size_type copied = 0;

    for (; copied < sz_; ++copied) {
        std::allocator_traits<Allocator>::construct(allocator, newData + copied, *(data_ + copied));
    }

    destroyRange(data_, data_ + sz_);
    destroyPointer(data_);

    data_ = newData;
    cap_ = size();
}

template <class T, class Allocator>
template <class InputIterator>
void vector<T, Allocator>::assignRangeForward(InputIterator from, InputIterator to,
                                              pointer destination) {
    for (; from != to; ++from, ++destination) {
        *destination = *from;
    }
}

template <class T, class Allocator>
template <class InputIterator>
void vector<T, Allocator>::assignRangeBackward(InputIterator from, InputIterator to,
                                               pointer destination) {
    destination += to - from - 1;

    for (; to != from; --to, --destination) {
        *destination = *(to - 1);
    }
}

template <class T, class Allocator>
template <class InputIterator>
void vector<T, Allocator>::moveRangeForward(InputIterator from, InputIterator to,
                                            pointer destination) {
    for (; from != to; ++from, ++destination) {
        std::allocator_traits<Allocator>::construct(allocator, destination, std::move(*from));
    }
}

template <class T, class Allocator>
template <class InputIterator>
void vector<T, Allocator>::copyRangeForward(InputIterator from, InputIterator to,
                                            pointer destination) {
    for (; from != to; ++from, ++destination) {
        std::allocator_traits<Allocator>::construct(allocator, destination, *from);
    }
}

template <class T, class Allocator>
void vector<T, Allocator>::copyRangeForward(pointer from, pointer to, const_reference value) {
    for (; from != to; ++from) {
        std::allocator_traits<Allocator>::construct(allocator, from, value);
    }
}

template <class T, class Allocator>
template <class InputIterator>
void vector<T, Allocator>::copyRangeBackward(InputIterator from, InputIterator to,
                                             pointer destination) {

    destination += to - from - 1;

    for (; to != from; --to, --destination) {
        std::allocator_traits<Allocator>::construct(allocator, destination, *(to - 1));
    }
}

template <class T, class Allocator>
T* vector<T, Allocator>::grow(size_type newCap, bool copy, size_type gapIndex, size_type gapSize) {
    value_type* newData = std::allocator_traits<Allocator>::allocate(allocator, newCap);

    if (copy) {
        size_type copied = 0;

        if (gapSize > 0) {

            for (; copied < gapIndex; ++copied) {
                std::allocator_traits<Allocator>::construct(allocator, newData + copied,
                                                            *(data_ + copied));
            }

            for (; copied < sz_; ++copied) {
                std::allocator_traits<Allocator>::construct(allocator, newData + copied + gapSize,
                                                            *(data_ + copied));
            }

        } else {

            for (; copied < sz_; ++copied) {
                std::allocator_traits<Allocator>::construct(allocator, newData + copied,
                                                            *(data_ + copied));
            }
        }
    }

    return newData;
}

template <class T, class Allocator>
void vector<T, Allocator>::destroyRange(pointer from, pointer to) {
    for (; from != to; ++from) {
        std::allocator_traits<Allocator>::destroy(allocator, from);
    }
}

template <class T, class Allocator>
void vector<T, Allocator>::destroyPointer(pointer ptr) {
    std::allocator_traits<Allocator>::deallocate(allocator, ptr, sz_);
}
}  // namespace coolstd