/* 100% vibe coded :( */

#ifndef RUD_VARIANT_HPP
#define RUD_VARIANT_HPP

#include "rud/string.hpp"
#include "rud/system.hpp"
#include <type_traits>

namespace rud {

    // ------------------------------------------------------------
    // Recursive union storage
    // ------------------------------------------------------------

    template<typename T, typename... Rest>
    union VariantStorage {
        T head;
        VariantStorage<Rest...> tail;
    };

    template<typename T>
    union VariantStorage<T> {
        T head;
    };

    // ------------------------------------------------------------
    // Variant type
    // ------------------------------------------------------------

    template<typename... Ts>
    struct Variant {
        VariantStorage<Ts...> storage;
        u32 index;
    };

    // ------------------------------------------------------------
    // Set
    // ------------------------------------------------------------

    template<typename T, u32 I, typename First, typename... Rest>
    void variant_set_impl(VariantStorage<First, Rest...>& storage, u32& index, T value) {
        if constexpr (std::is_same_v<T, First>) {
            storage.head = value;
            index = I;
        } else {
            variant_set_impl<T, I + 1>(storage.tail, index, value);
        }
    }

    template<typename T, u32 I, typename Last>
    void variant_set_impl(VariantStorage<Last>& storage, u32& index, T value) {
        if constexpr (std::is_same_v<T, Last>) {
            storage.head = value;
            index = I;
        } else {
            panic(Lit("Type not found in variant"));
        }
    }

    template<typename T, typename... Ts>
    void variant_set(Variant<Ts...>& var, T value) {
        variant_set_impl<T, 0>(var.storage, var.index, value);
    }

    // ------------------------------------------------------------
    // Get
    // ------------------------------------------------------------

    template<typename T, u32 I, typename First, typename... Rest>
    T variant_get_impl(const VariantStorage<First, Rest...>& storage, u32 index) {
        if (index == I) {
            if constexpr (std::is_same_v<T, First>) {
                return storage.head;
            } else {
                panic(Lit("Type mismatch in variant_get"));
            }
        } return variant_get_impl<T, I + 1>(storage.tail, index);
    }

    template<typename T, u32 I, typename Last>
    T variant_get_impl(const VariantStorage<Last>& storage, u32 index) {
        if (index == I) {
            if constexpr (std::is_same_v<T, Last>) {
                return storage.head;
            } else {
                panic(Lit("Type mismatch in variant_get"));
            }
        }
        panic(Lit("Invalid variant index"));
    }

    template<typename T, typename... Ts>
    T variant_get(const Variant<Ts...>& var) {
        return variant_get_impl<T, 0>(var.storage, var.index);
    }

    // ------------------------------------------------------------
    // Make
    // ------------------------------------------------------------

    template<typename VariantType, typename T>
    VariantType variant_make(T value) {
        VariantType var{};
        variant_set(var, value);
        return var;
    }

    // ------------------------------------------------------------
    // Type queries
    // ------------------------------------------------------------

    template<typename T>
    constexpr bool variant_contains_type() {
        return false;
    }

    template<typename T, typename First, typename... Rest>
    constexpr bool variant_contains_type() {
        if constexpr (std::is_same_v<T, First>) {
            return true;
        } else {
            return variant_contains_type<T, Rest...>();
        }
    }

    template<typename T, typename... Ts>
    constexpr bool variant_has_type(const Variant<Ts...>&) {
        return variant_contains_type<T, Ts...>();
    }

} // namespace rud

#endif

