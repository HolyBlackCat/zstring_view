#pragma once

#include <compare>
#include <cstddef>
#include <iosfwd>
#include <ranges>
#include <string_view>
#include <string>

namespace em
{
    template <typename T, typename Traits = std::char_traits<T>>
    class basic_zstring_view;

    using zstring_view = basic_zstring_view<char>;
    using wzstring_view = basic_zstring_view<wchar_t>;
    using u8zstring_view = basic_zstring_view<char8_t>;
    using u16zstring_view = basic_zstring_view<char16_t>;
    using u32zstring_view = basic_zstring_view<char32_t>;

    inline namespace literals
    {
        inline namespace string_view_literals
        {
            [[nodiscard]] constexpr zstring_view    operator""_zsv( const char     *str, std::size_t len) noexcept;
            [[nodiscard]] constexpr wzstring_view   operator""_zsv( const wchar_t  *str, std::size_t len) noexcept;
            [[nodiscard]] constexpr u8zstring_view  operator""_zsv( const char8_t  *str, std::size_t len) noexcept;
            [[nodiscard]] constexpr u16zstring_view operator""_zsv( const char16_t *str, std::size_t len) noexcept;
            [[nodiscard]] constexpr u32zstring_view operator""_zsv( const char32_t *str, std::size_t len) noexcept;
        }
    }

    template <typename T, typename Traits/*= std::char_traits<T>*/>
    class basic_zstring_view
    {
      public:
        using underlying_string_view_type = std::basic_string_view<T, Traits>;

        using traits_type            = typename underlying_string_view_type::traits_type;
        using value_type             = typename underlying_string_view_type::value_type;
        using pointer                = typename underlying_string_view_type::pointer;
        using const_pointer          = typename underlying_string_view_type::const_pointer;
        using reference              = typename underlying_string_view_type::reference;
        using const_reference        = typename underlying_string_view_type::const_reference;
        using const_iterator         = typename underlying_string_view_type::const_iterator;
        using iterator               = typename underlying_string_view_type::iterator;
        using const_reverse_iterator = typename underlying_string_view_type::const_reverse_iterator;
        using reverse_iterator       = typename underlying_string_view_type::reverse_iterator;
        using size_type              = typename underlying_string_view_type::size_type;
        using difference_type        = typename underlying_string_view_type::difference_type;

      private:
        underlying_string_view_type storage;

      public:
        static constexpr size_type npos = underlying_string_view_type::npos;

        constexpr basic_zstring_view() {}
        basic_zstring_view(std::nullptr_t) = delete;
        constexpr basic_zstring_view(const char *target) noexcept : storage(target) {}
        constexpr basic_zstring_view(const std::string &target) noexcept : storage(target) {} // No `std::string_view` constructor to ensure we have the null terminator.

        [[nodiscard]] constexpr operator const underlying_string_view_type &() const {return storage;}
        [[nodiscard]] constexpr const underlying_string_view_type &underlying_string_view() const {return storage;}

        // For libfmt, and hopefully one day for `std::format` too.
        // Not sure if this can return by const reference or not, seems safer not to.
        friend constexpr underlying_string_view_type format_as(const basic_zstring_view &self) noexcept {return self.underlying_string_view();}

        friend auto operator<=>(const basic_zstring_view<T> &, const basic_zstring_view<T> &) = default;

        [[nodiscard]] constexpr auto begin  () const noexcept {return storage.begin  ();}
        [[nodiscard]] constexpr auto cbegin () const noexcept {return storage.cbegin ();}
        [[nodiscard]] constexpr auto end    () const noexcept {return storage.end    ();}
        [[nodiscard]] constexpr auto cend   () const noexcept {return storage.cend   ();}
        [[nodiscard]] constexpr auto rbegin () const noexcept {return storage.rbegin ();}
        [[nodiscard]] constexpr auto crbegin() const noexcept {return storage.crbegin();}
        [[nodiscard]] constexpr auto rend   () const noexcept {return storage.rend   ();}
        [[nodiscard]] constexpr auto crend  () const noexcept {return storage.crend  ();}

        [[nodiscard]] constexpr const_reference operator[](size_type i) const noexcept {return storage[i];}
        [[nodiscard]] constexpr const_reference at(size_type i) const noexcept {return storage.at(i);}

        [[nodiscard]] constexpr const_reference front() const noexcept {return storage.front();}
        [[nodiscard]] constexpr const_reference back() const noexcept {return storage.back();}

        [[nodiscard]] constexpr const_pointer c_str() const noexcept {return storage.data();}
        [[nodiscard]] constexpr const_pointer data() const noexcept {return storage.data();}

        [[nodiscard]] constexpr size_type size() const noexcept {return storage.size();} // No `length()`, screw that.
        [[nodiscard]] constexpr size_type max_size() const noexcept {return storage.max_size();}

        [[nodiscard]] constexpr bool empty() const noexcept {return storage.empty();}

        constexpr void remove_prefix(size_type n) const noexcept {return storage.remove_prefix(n);} // No `remove_suffix` to maintain null-terminated-ness.

        constexpr void swap(basic_zstring_view &other) noexcept {storage.swap(other.storage);}

        constexpr size_type copy(T *dest, size_type count, size_type pos = 0) const noexcept {return storage.copy(dest, count, pos);}

        [[nodiscard]] constexpr basic_zstring_view substr(size_type pos) const noexcept {basic_zstring_view ret; ret.storage = storage.substr(pos); return ret;}
        [[nodiscard]] constexpr underlying_string_view_type substr(size_type pos, size_type size) const noexcept {return storage.substr(pos, size);}

        [[nodiscard]] constexpr int compare(underlying_string_view_type v)                                                                     const noexcept {return storage.compare(v);}
        [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, underlying_string_view_type v)                                   const noexcept {return storage.compare(pos1, count1, v);}
        [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, underlying_string_view_type v, size_type pos2, size_type count2) const noexcept {return storage.compare(pos1, count1, v, pos2, count2);}
        [[nodiscard]] constexpr int compare(const T *s)                                                                                        const noexcept {return storage.compare(s);}
        [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, const T *s)                                                      const noexcept {return storage.compare(pos1, count1, s);}
        [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, const T *s, size_type count2)                                    const noexcept {return storage.compare(pos1, count1, s, count2);}

        [[nodiscard]] constexpr bool starts_with(underlying_string_view_type sv) const noexcept {return storage.starts_with(sv);}
        [[nodiscard]] constexpr bool starts_with(T ch)                           const noexcept {return storage.starts_with(ch);}
        [[nodiscard]] constexpr bool starts_with(const T *s)                     const noexcept {return storage.starts_with(s );}
        [[nodiscard]] constexpr bool ends_with  (underlying_string_view_type sv) const noexcept {return storage.ends_with  (sv);}
        [[nodiscard]] constexpr bool ends_with  (T ch)                           const noexcept {return storage.ends_with  (ch);}
        [[nodiscard]] constexpr bool ends_with  (const T *s)                     const noexcept {return storage.ends_with  (s );}
        [[nodiscard]] constexpr bool contains   (underlying_string_view_type sv) const noexcept {return storage.contains   (sv);}
        [[nodiscard]] constexpr bool contains   (T ch)                           const noexcept {return storage.contains   (ch);}
        [[nodiscard]] constexpr bool contains   (const T *s)                     const noexcept {return storage.contains   (s );}

        [[nodiscard]] constexpr size_type find(underlying_string_view_type v, size_type pos = 0) const noexcept {return storage.find(v, pos);}
        [[nodiscard]] constexpr size_type find(T ch, size_type pos = 0)                          const noexcept {return storage.find(ch, pos);}
        [[nodiscard]] constexpr size_type find(const T *s, size_type pos, size_type count)       const noexcept {return storage.find(s, pos, count);}
        [[nodiscard]] constexpr size_type find(const T *s, size_type pos = 0)                    const noexcept {return storage.find(s, pos);}

        [[nodiscard]] constexpr size_type rfind(underlying_string_view_type v, size_type pos = 0) const noexcept {return storage.rfind(v, pos);}
        [[nodiscard]] constexpr size_type rfind(T ch, size_type pos = 0)                          const noexcept {return storage.rfind(ch, pos);}
        [[nodiscard]] constexpr size_type rfind(const T *s, size_type pos, size_type count)       const noexcept {return storage.rfind(s, pos, count);}
        [[nodiscard]] constexpr size_type rfind(const T *s, size_type pos = 0)                    const noexcept {return storage.rfind(s, pos);}

        [[nodiscard]] constexpr size_type find_first_of(underlying_string_view_type v, size_type pos = 0) const noexcept {return storage.find_first_of(v, pos);}
        [[nodiscard]] constexpr size_type find_first_of(T ch, size_type pos = 0)                          const noexcept {return storage.find_first_of(ch, pos);}
        [[nodiscard]] constexpr size_type find_first_of(const T *s, size_type pos, size_type count)       const noexcept {return storage.find_first_of(s, pos, count);}
        [[nodiscard]] constexpr size_type find_first_of(const T *s, size_type pos = 0)                    const noexcept {return storage.find_first_of(s, pos);}

        [[nodiscard]] constexpr size_type find_last_of(underlying_string_view_type v, size_type pos = 0) const noexcept {return storage.find_last_of(v, pos);}
        [[nodiscard]] constexpr size_type find_last_of(T ch, size_type pos = 0)                          const noexcept {return storage.find_last_of(ch, pos);}
        [[nodiscard]] constexpr size_type find_last_of(const T *s, size_type pos, size_type count)       const noexcept {return storage.find_last_of(s, pos, count);}
        [[nodiscard]] constexpr size_type find_last_of(const T *s, size_type pos = 0)                    const noexcept {return storage.find_last_of(s, pos);}

        [[nodiscard]] constexpr size_type find_first_not_of(underlying_string_view_type v, size_type pos = 0) const noexcept {return storage.find_first_not_of(v, pos);}
        [[nodiscard]] constexpr size_type find_first_not_of(T ch, size_type pos = 0)                          const noexcept {return storage.find_first_not_of(ch, pos);}
        [[nodiscard]] constexpr size_type find_first_not_of(const T *s, size_type pos, size_type count)       const noexcept {return storage.find_first_not_of(s, pos, count);}
        [[nodiscard]] constexpr size_type find_first_not_of(const T *s, size_type pos = 0)                    const noexcept {return storage.find_first_not_of(s, pos);}

        [[nodiscard]] constexpr size_type find_last_not_of(underlying_string_view_type v, size_type pos = 0) const noexcept {return storage.find_last_not_of(v, pos);}
        [[nodiscard]] constexpr size_type find_last_not_of(T ch, size_type pos = 0)                          const noexcept {return storage.find_last_not_of(ch, pos);}
        [[nodiscard]] constexpr size_type find_last_not_of(const T *s, size_type pos, size_type count)       const noexcept {return storage.find_last_not_of(s, pos, count);}
        [[nodiscard]] constexpr size_type find_last_not_of(const T *s, size_type pos = 0)                    const noexcept {return storage.find_last_not_of(s, pos);}

        // Clang chokes when trying to `friend` those in a generic way, with a `const T *` parameter. Whatever, just friend all of them.
        friend constexpr zstring_view    literals::string_view_literals::operator""_zsv( const char     *str, std::size_t len) noexcept;
        friend constexpr wzstring_view   literals::string_view_literals::operator""_zsv( const wchar_t  *str, std::size_t len) noexcept;
        friend constexpr u8zstring_view  literals::string_view_literals::operator""_zsv( const char8_t  *str, std::size_t len) noexcept;
        friend constexpr u16zstring_view literals::string_view_literals::operator""_zsv( const char16_t *str, std::size_t len) noexcept;
        friend constexpr u32zstring_view literals::string_view_literals::operator""_zsv( const char32_t *str, std::size_t len) noexcept;
    };

    [[nodiscard]] constexpr zstring_view    literals::string_view_literals::operator""_zsv( const char     *str, std::size_t len) noexcept {zstring_view    ret; ret.storage = std::string_view   (str, len); return ret;}
    [[nodiscard]] constexpr wzstring_view   literals::string_view_literals::operator""_zsv( const wchar_t  *str, std::size_t len) noexcept {wzstring_view   ret; ret.storage = std::wstring_view  (str, len); return ret;}
    [[nodiscard]] constexpr u8zstring_view  literals::string_view_literals::operator""_zsv( const char8_t  *str, std::size_t len) noexcept {u8zstring_view  ret; ret.storage = std::u8string_view (str, len); return ret;}
    [[nodiscard]] constexpr u16zstring_view literals::string_view_literals::operator""_zsv( const char16_t *str, std::size_t len) noexcept {u16zstring_view ret; ret.storage = std::u16string_view(str, len); return ret;}
    [[nodiscard]] constexpr u32zstring_view literals::string_view_literals::operator""_zsv( const char32_t *str, std::size_t len) noexcept {u32zstring_view ret; ret.storage = std::u32string_view(str, len); return ret;}

    template <typename T, typename Traits>
    std::basic_ostream<T, Traits> &operator<<(std::basic_ostream<T, Traits> &os, basic_zstring_view<T, Traits> v) {return os << v.underlying_string_view();}
}

template <typename T, typename Traits>
inline constexpr bool std::ranges::enable_borrowed_range<em::basic_zstring_view<T, Traits>> = true;

template <typename T, typename Traits>
inline constexpr bool std::ranges::enable_view<em::basic_zstring_view<T, Traits>> = true;

template <> struct std::hash<em::zstring_view   > : std::hash<std::string_view   > {};
template <> struct std::hash<em::wzstring_view  > : std::hash<std::wstring_view  > {};
template <> struct std::hash<em::u8zstring_view > : std::hash<std::u8string_view > {};
template <> struct std::hash<em::u16zstring_view> : std::hash<std::u16string_view> {};
template <> struct std::hash<em::u32zstring_view> : std::hash<std::u32string_view> {};
