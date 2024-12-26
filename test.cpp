#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>

#include <algorithm>

#include <vector>
#include "vector.h"

template <typename T>
std::vector<T> create_range(T start, T end) {
    std::vector<T> vec;
    for (T i = start; i <= end; ++i) {
        vec.push_back(i);
    }
    return vec;
}

TEST_CASE("Constructors", "[vector]") {
    using custom_vector = coolstd::vector<int>;
    using std_vector = std::vector<int>;

    SECTION("Default constructor") {
        custom_vector custom_vec;
        std_vector std_vec;

        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE(custom_vec.empty() == std_vec.empty());
    }

    SECTION("Allocator constructor") {
        std::allocator<int> alloc;
        custom_vector custom_vec(alloc);
        std_vector std_vec(alloc);

        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE(custom_vec.empty() == std_vec.empty());
    }

    SECTION("Count constructor") {
        custom_vector custom_vec(5);
        std_vector std_vec(5);

        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Count and value constructor") {
        custom_vector custom_vec(5, 42);
        std_vector std_vec(5, 42);

        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Range constructor") {
        auto range = create_range(1, 5);
        custom_vector custom_vec(range.begin(), range.end());
        std_vector std_vec(range.begin(), range.end());

        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Initializer list constructor") {
        custom_vector custom_vec = {1, 2, 3, 4, 5};
        std_vector std_vec = {1, 2, 3, 4, 5};

        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Copy constructor") {
        custom_vector custom_vec_source = {1, 2, 3, 4, 5};
        std_vector std_vec_source = {1, 2, 3, 4, 5};

        custom_vector custom_vec(custom_vec_source);
        std_vector std_vec(std_vec_source);

        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Move constructor") {
        custom_vector custom_vec_source = {1, 2, 3, 4, 5};
        std_vector std_vec_source = {1, 2, 3, 4, 5};

        custom_vector custom_vec(std::move(custom_vec_source));
        std_vector std_vec(std::move(std_vec_source));

        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE(custom_vec_source.empty());  // Moved-from state
    }

    SECTION("Copy assignment") {
        custom_vector custom_vec_source = {1, 2, 3, 4, 5};
        std_vector std_vec_source = {1, 2, 3, 4, 5};

        custom_vector custom_vec;
        std_vector std_vec;

        custom_vec = custom_vec_source;
        std_vec = std_vec_source;

        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Move assignment") {
        custom_vector custom_vec_source = {1, 2, 3, 4, 5};
        std_vector std_vec_source = {1, 2, 3, 4, 5};

        custom_vector custom_vec;
        std_vector std_vec;

        custom_vec = std::move(custom_vec_source);
        std_vec = std::move(std_vec_source);

        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE(custom_vec_source.empty());  // Moved-from state
    }

    SECTION("Assign with iterators") {
        auto range = create_range(1, 5);
        custom_vector custom_vec;
        std_vector std_vec;

        custom_vec.assign(range.begin(), range.end());
        std_vec.assign(range.begin(), range.end());

        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Assign with count and value") {
        custom_vector custom_vec;
        std_vector std_vec;

        custom_vec.assign(5, 42);
        std_vec.assign(5, 42);

        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Assign with initializer list") {
        custom_vector custom_vec;
        std_vector std_vec;

        custom_vec.assign({1, 2, 3, 4, 5});
        std_vec.assign({1, 2, 3, 4, 5});

        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Get allocator") {
        std::allocator<int> alloc;
        custom_vector custom_vec(alloc);
        std_vector std_vec(alloc);

        REQUIRE(custom_vec.get_allocator() == std_vec.get_allocator());
    }
}

TEST_CASE("Iterators", "[vector]") {
    using custom_vector = coolstd::vector<int>;
    using std_vector = std::vector<int>;

    custom_vector custom_vec = {1, 2, 3, 4, 5};
    std_vector std_vec = {1, 2, 3, 4, 5};

    SECTION("Iterator begin and end") {
        REQUIRE(std::distance(custom_vec.begin(), custom_vec.end()) ==
                std::distance(std_vec.begin(), std_vec.end()));
        REQUIRE(std::equal(custom_vec.begin(), custom_vec.end(), std_vec.begin()));
    }

    SECTION("Const iterator begin and end") {
        const custom_vector custom_const_vec = {1, 2, 3, 4, 5};
        const std_vector std_const_vec = {1, 2, 3, 4, 5};

        REQUIRE(std::distance(custom_const_vec.begin(), custom_const_vec.end()) ==
                std::distance(std_const_vec.begin(), std_const_vec.end()));
        REQUIRE(
            std::equal(custom_const_vec.begin(), custom_const_vec.end(), std_const_vec.begin()));
    }

    SECTION("Reverse iterator rbegin and rend") {
        REQUIRE(std::distance(custom_vec.rbegin(), custom_vec.rend()) ==
                std::distance(std_vec.rbegin(), std_vec.rend()));
        REQUIRE(std::equal(custom_vec.rbegin(), custom_vec.rend(), std_vec.rbegin()));
    }

    SECTION("Const reverse iterator rbegin and rend") {
        const custom_vector custom_const_vec = {1, 2, 3, 4, 5};
        const std_vector std_const_vec = {1, 2, 3, 4, 5};

        REQUIRE(std::distance(custom_const_vec.rbegin(), custom_const_vec.rend()) ==
                std::distance(std_const_vec.rbegin(), std_const_vec.rend()));
        REQUIRE(
            std::equal(custom_const_vec.rbegin(), custom_const_vec.rend(), std_const_vec.rbegin()));
    }

    SECTION("cbegin and cend") {
        const custom_vector custom_const_vec = {1, 2, 3, 4, 5};
        const std_vector std_const_vec = {1, 2, 3, 4, 5};

        REQUIRE(std::distance(custom_const_vec.cbegin(), custom_const_vec.cend()) ==
                std::distance(std_const_vec.cbegin(), std_const_vec.cend()));
        REQUIRE(
            std::equal(custom_const_vec.cbegin(), custom_const_vec.cend(), std_const_vec.cbegin()));
    }

    SECTION("crbegin and crend") {
        const custom_vector custom_const_vec = {1, 2, 3, 4, 5};
        const std_vector std_const_vec = {1, 2, 3, 4, 5};

        REQUIRE(std::distance(custom_const_vec.crbegin(), custom_const_vec.crend()) ==
                std::distance(std_const_vec.crbegin(), std_const_vec.crend()));
        REQUIRE(std::equal(custom_const_vec.crbegin(), custom_const_vec.crend(),
                           std_const_vec.crbegin()));
    }

    SECTION("Iterator increment and access") {
        auto custom_it = custom_vec.begin();
        auto std_it = std_vec.begin();

        while (custom_it != custom_vec.end() && std_it != std_vec.end()) {
            REQUIRE(*custom_it == *std_it);
            ++custom_it;
            ++std_it;
        }
    }

    SECTION("Reverse iterator increment and access") {
        auto custom_rit = custom_vec.rbegin();
        auto std_rit = std_vec.rbegin();

        while (custom_rit != custom_vec.rend() && std_rit != std_vec.rend()) {
            REQUIRE(*custom_rit == *std_rit);
            ++custom_rit;
            ++std_rit;
        }
    }
}

TEST_CASE("Capacity", "[vector]") {
    using custom_vector = coolstd::vector<int>;
    using std_vector = std::vector<int>;

    custom_vector custom_vec = {1, 2, 3, 4, 5};
    std_vector std_vec = {1, 2, 3, 4, 5};

    SECTION("Empty") {
        custom_vector custom_vec_empty;
        std_vector std_vec_empty;

        REQUIRE(custom_vec_empty.empty() == std_vec_empty.empty());
        REQUIRE(custom_vec.empty() == std_vec.empty());
    }

    SECTION("Size") {
        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE(custom_vec.size() == 5);
    }

    SECTION("Capacity") {
        REQUIRE(custom_vec.capacity() >= custom_vec.size());
        REQUIRE(std_vec.capacity() >= std_vec.size());
    }

    SECTION("Max size") {
        REQUIRE(custom_vec.max_size() == std_vec.max_size());  // LOOK AT SOURCE
    }

    SECTION("Resize larger") {
        custom_vec.resize(10, 42);
        std_vec.resize(10, 42);

        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE(custom_vec.capacity() == std_vec.capacity());
        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Resize smaller") {
        custom_vec.resize(3);
        std_vec.resize(3);

        REQUIRE(custom_vec.size() == std_vec.size());
        REQUIRE(custom_vec.capacity() == std_vec.capacity());
        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Reserve") {
        custom_vec.reserve(20);
        std_vec.reserve(20);

        REQUIRE(custom_vec.capacity() == std_vec.capacity());
    }

    SECTION("Shrink to fit") {
        custom_vec.reserve(20);
        std_vec.reserve(20);

        custom_vec.shrink_to_fit();
        std_vec.shrink_to_fit();

        REQUIRE(custom_vec.capacity() == std_vec.capacity());
        REQUIRE(custom_vec.capacity() == std_vec.size());
    }
}

TEST_CASE("Element access", "[vector]") {
    using custom_vector = coolstd::vector<int>;
    using std_vector = std::vector<int>;

    custom_vector custom_vec = {1, 2, 3, 4, 5};
    std_vector std_vec = {1, 2, 3, 4, 5};

    SECTION("Operator[] access") {
        for (size_t i = 0; i < custom_vec.size(); ++i) {
            REQUIRE(custom_vec[i] == std_vec[i]);
        }
    }

    SECTION("Const operator[] access") {
        const custom_vector custom_const_vec = {1, 2, 3, 4, 5};
        const std_vector std_const_vec = {1, 2, 3, 4, 5};

        for (size_t i = 0; i < custom_const_vec.size(); ++i) {
            REQUIRE(custom_const_vec[i] == std_const_vec[i]);
        }
    }

    SECTION("at() method") {
        for (size_t i = 0; i < custom_vec.size(); ++i) {
            REQUIRE(custom_vec.at(i) == std_vec.at(i));
        }
        REQUIRE_THROWS_AS(custom_vec.at(custom_vec.size()), std::out_of_range);
    }

    SECTION("Const at() method") {
        const custom_vector custom_const_vec = {1, 2, 3, 4, 5};
        const std_vector std_const_vec = {1, 2, 3, 4, 5};

        for (size_t i = 0; i < custom_const_vec.size(); ++i) {
            REQUIRE(custom_const_vec.at(i) == std_const_vec.at(i));
        }
        REQUIRE_THROWS_AS(custom_const_vec.at(custom_const_vec.size()), std::out_of_range);
    }

    SECTION("front() method") {
        REQUIRE(custom_vec.front() == std_vec.front());
    }

    SECTION("Const front() method") {
        const custom_vector custom_const_vec = {1, 2, 3, 4, 5};
        const std_vector std_const_vec = {1, 2, 3, 4, 5};

        REQUIRE(custom_const_vec.front() == std_const_vec.front());
    }

    SECTION("back() method") {
        REQUIRE(custom_vec.back() == std_vec.back());
    }

    SECTION("Const back() method") {
        const custom_vector custom_const_vec = {1, 2, 3, 4, 5};
        const std_vector std_const_vec = {1, 2, 3, 4, 5};

        REQUIRE(custom_const_vec.back() == std_const_vec.back());
    }
}

TEST_CASE("Data access", "[vector]") {
    using custom_vector = coolstd::vector<int>;
    using std_vector = std::vector<int>;

    custom_vector custom_vec = {1, 2, 3, 4, 5};
    std_vector std_vec = {1, 2, 3, 4, 5};

    SECTION("Non-const data() access") {
        int* custom_data_ptr = custom_vec.data();
        int* std_data_ptr = std_vec.data();

        REQUIRE(custom_data_ptr != nullptr);
        REQUIRE(std_data_ptr != nullptr);

        REQUIRE(custom_data_ptr == &custom_vec[0]);
        REQUIRE(std_data_ptr == &std_vec[0]);

        for (size_t i = 0; i < custom_vec.size(); ++i) {
            REQUIRE(custom_data_ptr[i] == std_data_ptr[i]);
        }
    }

    SECTION("Const data() access") {
        const custom_vector custom_const_vec = {1, 2, 3, 4, 5};
        const std_vector std_const_vec = {1, 2, 3, 4, 5};

        const int* custom_data_ptr = custom_const_vec.data();
        const int* std_data_ptr = std_const_vec.data();

        REQUIRE(custom_data_ptr != nullptr);
        REQUIRE(std_data_ptr != nullptr);

        REQUIRE(custom_data_ptr == &custom_const_vec[0]);
        REQUIRE(std_data_ptr == &std_const_vec[0]);

        for (size_t i = 0; i < custom_const_vec.size(); ++i) {
            REQUIRE(custom_data_ptr[i] == std_data_ptr[i]);
        }
    }
}

TEST_CASE("Modifiers", "[vector]") {
    using custom_vector = coolstd::vector<int>;
    using std_vector = std::vector<int>;

    SECTION("emplace_back") {
        custom_vector custom_vec;
        std_vector std_vec;

        custom_vec.emplace_back(1);
        std_vec.emplace_back(1);
        custom_vec.emplace_back(2);
        std_vec.emplace_back(2);
        custom_vec.emplace_back(3);
        std_vec.emplace_back(3);

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("push_back lvalue") {
        custom_vector custom_vec;
        std_vector std_vec;

        int value = 1;
        custom_vec.push_back(value);
        std_vec.push_back(value);
        value = 2;
        custom_vec.push_back(value);
        std_vec.push_back(value);

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("push_back rvalue") {
        custom_vector custom_vec;
        std_vector std_vec;

        custom_vec.push_back(1);
        std_vec.push_back(1);
        custom_vec.push_back(2);
        std_vec.push_back(2);

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("pop_back") {
        custom_vector custom_vec = {1, 2, 3, 4, 5};
        std_vector std_vec = {1, 2, 3, 4, 5};

        custom_vec.pop_back();
        std_vec.pop_back();

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("emplace") {
        custom_vector custom_vec = {1, 2, 3};
        std_vector std_vec = {1, 2, 3};

        custom_vec.emplace(custom_vec.begin() + 1, 4);
        std_vec.emplace(std_vec.begin() + 1, 4);

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("insert lvalue") {
        custom_vector custom_vec = {1, 2, 3};
        std_vector std_vec = {1, 2, 3};

        int value = 4;
        custom_vec.insert(custom_vec.begin() + 1, value);
        std_vec.insert(std_vec.begin() + 1, value);

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("insert rvalue") {
        custom_vector custom_vec = {1, 2, 3};
        std_vector std_vec = {1, 2, 3};

        custom_vec.insert(custom_vec.begin() + 1, 4);
        std_vec.insert(std_vec.begin() + 1, 4);

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("insert count and value") {
        custom_vector custom_vec = {1, 2, 3};
        std_vector std_vec = {1, 2, 3};

        custom_vec.insert(custom_vec.begin() + 1, 2, 4);
        std_vec.insert(std_vec.begin() + 1, 2, 4);

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("insert range") {
        custom_vector custom_vec = {1, 2, 3};
        std_vector std_vec = {1, 2, 3};

        std::vector<int> range = {4, 5, 6};
        custom_vec.insert(custom_vec.begin() + 1, range.begin(), range.end());
        std_vec.insert(std_vec.begin() + 1, range.begin(), range.end());

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("insert initializer list") {
        custom_vector custom_vec = {1, 2, 3};
        std_vector std_vec = {1, 2, 3};

        custom_vec.insert(custom_vec.begin() + 1, {4, 5, 6});
        std_vec.insert(std_vec.begin() + 1, {4, 5, 6});

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("erase single element") {
        custom_vector custom_vec = {1, 2, 3, 4, 5};
        std_vector std_vec = {1, 2, 3, 4, 5};

        custom_vec.erase(custom_vec.begin() + 1);
        std_vec.erase(std_vec.begin() + 1);

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("erase range") {
        custom_vector custom_vec = {1, 2, 3, 4, 5};
        std_vector std_vec = {1, 2, 3, 4, 5};

        custom_vec.erase(custom_vec.begin() + 1, custom_vec.begin() + 3);
        std_vec.erase(std_vec.begin() + 1, std_vec.begin() + 3);

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("swap") {
        custom_vector custom_vec1 = {1, 2, 3};
        custom_vector custom_vec2 = {4, 5, 6};

        std_vector std_vec1 = {1, 2, 3};
        std_vector std_vec2 = {4, 5, 6};

        custom_vec1.swap(custom_vec2);
        std_vec1.swap(std_vec2);

        REQUIRE_THAT(custom_vec1, Catch::Matchers::RangeEquals(std_vec1));
        REQUIRE_THAT(custom_vec2, Catch::Matchers::RangeEquals(std_vec2));
    }

    SECTION("clear") {
        custom_vector custom_vec = {1, 2, 3, 4, 5};
        std_vector std_vec = {1, 2, 3, 4, 5};

        custom_vec.clear();
        std_vec.clear();

        REQUIRE(custom_vec.empty() == std_vec.empty());
        REQUIRE(custom_vec.size() == std_vec.size());
    }
}

template <typename T>
struct CustomAllocator {
    using value_type = T;

    CustomAllocator() = default;

    template <typename U>
    constexpr CustomAllocator(const CustomAllocator<U>&) noexcept {
    }

    T* allocate(std::size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t) noexcept {
        ::operator delete(p);
    }

    bool operator==(const CustomAllocator&) const {
        return true;
    }
    bool operator!=(const CustomAllocator&) const {
        return false;
    }
};

template <typename T>
using custom_vector_with_alloc = coolstd::vector<T, CustomAllocator<T>>;

TEST_CASE("Types", "[vector]") {

    SECTION("Custom vector with int type") {
        custom_vector_with_alloc<int> custom_vec = {1, 2, 3, 4, 5};
        std::vector<int, CustomAllocator<int>> std_vec = {1, 2, 3, 4, 5};

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Custom vector with double type") {
        custom_vector_with_alloc<double> custom_vec = {1.1, 2.2, 3.3, 4.4, 5.5};
        std::vector<double, CustomAllocator<double>> std_vec = {1.1, 2.2, 3.3, 4.4, 5.5};

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Custom vector with std::string type") {
        custom_vector_with_alloc<std::string> custom_vec = {"one", "two", "three", "four", "five"};
        std::vector<std::string, CustomAllocator<std::string>> std_vec = {"one", "two", "three",
                                                                          "four", "five"};

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Custom vector with custom struct type") {
        struct MyStruct {
            int a;
            double b;
            bool operator==(const MyStruct& other) const {
                return a == other.a && b == other.b;
            }
        };

        custom_vector_with_alloc<MyStruct> custom_vec = {
            {1, 1.1}, {2, 2.2}, {3, 3.3}, {4, 4.4}, {5, 5.5}};
        std::vector<MyStruct, CustomAllocator<MyStruct>> std_vec = {
            {1, 1.1}, {2, 2.2}, {3, 3.3}, {4, 4.4}, {5, 5.5}};

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Custom vector with std::allocator for int type") {
        coolstd::vector<int> custom_vec = {1, 2, 3, 4, 5};
        std::vector<int> std_vec = {1, 2, 3, 4, 5};

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Custom vector with std::allocator for double type") {
        coolstd::vector<double> custom_vec = {1.1, 2.2, 3.3, 4.4, 5.5};
        std::vector<double> std_vec = {1.1, 2.2, 3.3, 4.4, 5.5};

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Custom vector with std::allocator for std::string type") {
        coolstd::vector<std::string> custom_vec = {"one", "two", "three", "four", "five"};
        std::vector<std::string> std_vec = {"one", "two", "three", "four", "five"};

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("Custom vector with std::allocator for custom struct type") {
        struct MyStruct {
            int a;
            double b;
            bool operator==(const MyStruct& other) const {
                return a == other.a && b == other.b;
            }
        };

        coolstd::vector<MyStruct> custom_vec = {{1, 1.1}, {2, 2.2}, {3, 3.3}, {4, 4.4}, {5, 5.5}};
        std::vector<MyStruct> std_vec = {{1, 1.1}, {2, 2.2}, {3, 3.3}, {4, 4.4}, {5, 5.5}};

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }
}

TEST_CASE("CMember types", "[vector]") {
    using custom_vector = coolstd::vector<int>;

    SECTION("Member types") {
        static_assert(std::is_same<custom_vector::value_type, int>::value,
                      "value_type should be int");
        static_assert(std::is_same<custom_vector::allocator_type, std::allocator<int>>::value,
                      "allocator_type should be std::allocator<int>");
        static_assert(std::is_same<custom_vector::size_type, std::size_t>::value,
                      "size_type should be std::size_t");
        static_assert(std::is_same<custom_vector::difference_type, std::ptrdiff_t>::value,
                      "difference_type should be std::ptrdiff_t");
        static_assert(std::is_same<custom_vector::reference, int&>::value,
                      "reference should be int&");
        static_assert(std::is_same<custom_vector::const_reference, const int&>::value,
                      "const_reference should be const int&");
        static_assert(std::is_same<custom_vector::pointer, int*>::value, "pointer should be int*");
        static_assert(std::is_same<custom_vector::const_pointer, const int*>::value,
                      "const_pointer should be const int*");
    }
    using iterator = custom_vector::iterator;
    using const_iterator = custom_vector::const_iterator;

    SECTION("Iterator types") {
        static_assert(std::is_same<typename std::iterator_traits<iterator>::iterator_category,
                                   std::random_access_iterator_tag>::value,
                      "iterator_category should be random_access_iterator_tag");
        static_assert(std::is_same<typename std::iterator_traits<iterator>::value_type, int>::value,
                      "value_type should be int");
        static_assert(std::is_same<typename std::iterator_traits<iterator>::difference_type,
                                   std::ptrdiff_t>::value,
                      "difference_type should be std::ptrdiff_t");
        static_assert(std::is_same<typename std::iterator_traits<iterator>::pointer, int*>::value,
                      "pointer should be int*");
        static_assert(std::is_same<typename std::iterator_traits<iterator>::reference, int&>::value,
                      "reference should be int&");

        static_assert(std::is_same<typename std::iterator_traits<const_iterator>::iterator_category,
                                   std::random_access_iterator_tag>::value,
                      "iterator_category should be random_access_iterator_tag");
        static_assert(
            std::is_same<typename std::iterator_traits<const_iterator>::value_type, const int>::value,
            "value_type should be int");
        static_assert(std::is_same<typename std::iterator_traits<const_iterator>::difference_type,
                                   std::ptrdiff_t>::value,
                      "difference_type should be std::ptrdiff_t");
        static_assert(
            std::is_same<typename std::iterator_traits<const_iterator>::pointer, const int*>::value,
            "pointer should be const int*");
        static_assert(std::is_same<typename std::iterator_traits<const_iterator>::reference,
                                   const int&>::value,
                      "reference should be const int&");
    }
}

TEST_CASE("Algorithms", "[vector]") {
    using custom_vector = coolstd::vector<int>;
    using std_vector = std::vector<int>;

    SECTION("std::sort") {
        custom_vector custom_vec = {5, 3, 1, 4, 2};
        std_vector std_vec = {5, 3, 1, 4, 2};

        std::sort(custom_vec.begin(), custom_vec.end());
        std::sort(std_vec.begin(), std_vec.end());

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("std::find") {
        custom_vector custom_vec = {1, 2, 3, 4, 5};
        std_vector std_vec = {1, 2, 3, 4, 5};

        auto custom_it = std::find(custom_vec.begin(), custom_vec.end(), 3);
        auto std_it = std::find(std_vec.begin(), std_vec.end(), 3);

        REQUIRE(custom_it != custom_vec.end());
        REQUIRE(std_it != std_vec.end());
        REQUIRE(*custom_it == *std_it);
    }

    SECTION("std::copy") {
        custom_vector custom_vec = {1, 2, 3, 4, 5};
        std_vector std_vec = {1, 2, 3, 4, 5};

        custom_vector custom_vec_copy(custom_vec.size());
        std_vector std_vec_copy(std_vec.size());

        std::copy(custom_vec.begin(), custom_vec.end(), custom_vec_copy.begin());
        std::copy(std_vec.begin(), std_vec.end(), std_vec_copy.begin());

        REQUIRE_THAT(custom_vec_copy, Catch::Matchers::RangeEquals(std_vec_copy));
    }

    SECTION("std::reverse") {
        custom_vector custom_vec = {1, 2, 3, 4, 5};
        std_vector std_vec = {1, 2, 3, 4, 5};

        std::reverse(custom_vec.begin(), custom_vec.end());
        std::reverse(std_vec.begin(), std_vec.end());

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("std::for_each") {
        custom_vector custom_vec = {1, 2, 3, 4, 5};
        std_vector std_vec = {1, 2, 3, 4, 5};

        std::for_each(custom_vec.begin(), custom_vec.end(), [](int& n) { n *= 2; });
        std::for_each(std_vec.begin(), std_vec.end(), [](int& n) { n *= 2; });

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }

    SECTION("std::transform") {
        custom_vector custom_vec = {1, 2, 3, 4, 5};
        std_vector std_vec = {1, 2, 3, 4, 5};

        custom_vector custom_vec_transformed(custom_vec.size());
        std_vector std_vec_transformed(std_vec.size());

        std::transform(custom_vec.begin(), custom_vec.end(), custom_vec_transformed.begin(), [](int n) { return n * 3; });
        std::transform(std_vec.begin(), std_vec.end(), std_vec_transformed.begin(), [](int n) { return n * 3; });

        REQUIRE_THAT(custom_vec_transformed, Catch::Matchers::RangeEquals(std_vec_transformed));
    }

    SECTION("std::remove_if") {
        custom_vector custom_vec = {1, 2, 3, 4, 5};
        std_vector std_vec = {1, 2, 3, 4, 5};

        auto custom_it = std::remove_if(custom_vec.begin(), custom_vec.end(), [](int n) { return n % 2 == 0; });
        auto std_it = std::remove_if(std_vec.begin(), std_vec.end(), [](int n) { return n % 2 == 0; });

        custom_vec.erase(custom_it, custom_vec.end());
        std_vec.erase(std_it, std_vec.end());

        REQUIRE_THAT(custom_vec, Catch::Matchers::RangeEquals(std_vec));
    }
}
