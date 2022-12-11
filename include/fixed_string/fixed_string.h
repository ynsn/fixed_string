//
// Created by Yoram Janssen on 11/12/2022.
//

#ifndef FIXED_STRING_FIXED_STRING_H
#define FIXED_STRING_FIXED_STRING_H

#ifndef FIXED_STRING_NS
  #define FIXED_STRING_NS stdext
#endif

#define FIXED_STRING_LITERALS_NS FIXED_STRING_NS #_literals

#include <string_view>

namespace FIXED_STRING_NS {

template<class T, std::size_t N, class Traits = std::char_traits<T>>
struct basic_fixed_string {
  using type = basic_fixed_string;
  using value_type = T;
  using traits_type = Traits;
  using view = std::basic_string_view<T>;

  using size_type = typename view::size_type;
  using difference_type = typename view::difference_type;

  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;

  using iterator = pointer;
  using const_iterator = const_pointer;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  static constexpr std::size_t npos = view::npos;

  constexpr basic_fixed_string() noexcept = default;
  constexpr basic_fixed_string(const T(&str)[N]) noexcept { std::copy_n(str, N, value); }
  constexpr basic_fixed_string(const basic_fixed_string &copy) noexcept: basic_fixed_string(copy.value) {}

  constexpr basic_fixed_string &operator=(const basic_fixed_string &other) noexcept {
    std::copy_n(other.value, N, value);
    return *this;
  }

  constexpr basic_fixed_string &operator=(const T(&str)[N]) noexcept {
    std::copy_n(str, N, value);
    return *this;
  }

  constexpr explicit operator view() const noexcept {
    return std::string_view(value, length());
  }

  friend constexpr std::ostream &operator<<(std::ostream &os, const basic_fixed_string str) {
    os << str.value;
    return os;
  }

  constexpr iterator begin() noexcept { return value; }
  constexpr const_iterator begin() const noexcept { return value; }
  constexpr iterator end() noexcept { return value + N - 1; }
  constexpr const_iterator end() const noexcept { return value + N - 1; }

  constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(begin()); }
  constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(begin()); }
  constexpr reverse_iterator rend() noexcept { return reverse_iterator(end()); }
  constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(end()); }

  constexpr const_iterator cbegin() const noexcept { return begin(); }
  constexpr const_iterator cend() const noexcept { return end(); }
  constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
  constexpr const_reverse_iterator crend() const noexcept { return rend(); }

  constexpr size_type size() const noexcept { return N - 1; }
  constexpr size_type length() const noexcept { return N - 1; }
  constexpr size_type max_size() const noexcept { return N - 1; }
  constexpr size_type capacity() const noexcept { return N - 1; }
  constexpr bool empty() const noexcept { return N == 0; }

  constexpr const_reference operator[](size_type pos) const noexcept { return value[pos]; }
  constexpr reference operator[](size_type pos) noexcept { return value[pos]; }

  constexpr const_reference at(size_type pos) const noexcept { return value[pos]; }
  constexpr reference at(size_type pos) noexcept { return value[pos]; }

  constexpr const_reference front() const noexcept { return value[0]; }
  constexpr reference front() noexcept { return value[0]; }
  constexpr const_reference back() const noexcept { return value[size() - 1]; }
  constexpr reference back() noexcept { return value[size() - 1]; }

  constexpr size_type copy(const basic_fixed_string &dest, size_type count = N, size_type pos = 0) const noexcept {
    if (pos > length())
      return npos;

    const size_type cnt = std::min(count, length() - pos);
    traits_type::copy(dest.value, value + pos, cnt);
    return cnt;
  }

  constexpr size_type copy(pointer dest, size_type count = N, size_type pos = 0) const noexcept {
    if (pos > length())
      return npos;

    const size_type cnt = std::min(count, length() - pos);
    traits_type::copy(dest, value + pos, cnt);
    return cnt;
  }

  constexpr basic_fixed_string &replace(size_type pos, view string) noexcept {
    std::copy_n(string.data(), string.size(), value + pos);
    return *this;
  }

  template<std::size_t PrefixSize> requires (PrefixSize < N)
  constexpr basic_fixed_string<T, N - PrefixSize> remove_prefix() const noexcept {
    basic_fixed_string<T, N - PrefixSize> new_fixed_string;
    copy(new_fixed_string.value, N - PrefixSize, PrefixSize);
    return new_fixed_string;
  }

  template<std::size_t SuffixSize> requires (SuffixSize < N)
  constexpr basic_fixed_string<T, N - SuffixSize> remove_suffix() const noexcept {
    basic_fixed_string<T, N - SuffixSize> new_fixed_string;
    copy(new_fixed_string.value, N - SuffixSize - 1, 0);
    return new_fixed_string;
  }

  template<size_type Pos = 0, size_type Count = npos> requires (Pos + Count < N)
  constexpr basic_fixed_string<T, Count + 1> substr() const noexcept {
    basic_fixed_string<T, Count + 1> new_fixed_string;
    copy(new_fixed_string.value, Count, Pos);
    return new_fixed_string;
  }

  constexpr basic_fixed_string<T, N> substr(size_type pos = 0, size_type count = npos) const noexcept {
    basic_fixed_string<T, N> new_fixed_string;
    copy(new_fixed_string.value, count, pos);
    return new_fixed_string;
  }

  constexpr int compare(view str) const noexcept {
    const std::size_t len = std::min(length(), str.length());
    const int comparison = traits_type::compare(data(), str.data(), len);
    if (comparison != 0)
      return comparison;
    if (length() == str.length())
      return 0;
    return length() < str.length() ? -1 : 1;
  }

  constexpr bool starts_with(view substr) const noexcept {
    if (substr.length() > length())
      return false;

    for (int i = 0; i < substr.length(); ++i) {
      if (value[i] != substr[i])
        return false;
    }

    return true;
  }

  constexpr bool starts_with(value_type c) const noexcept { return !empty() && traits_type::eq(front(), c); }
  constexpr bool starts_with(const_pointer c) const noexcept { return starts_with(view(c)); }

  constexpr bool ends_with(view substr) const noexcept {
    if (substr.length() > length())
      return false;

    // I am drunk and tired please fix this ugly stuff
    for (int i = 0; i < substr.length(); i--) {
      if (value[length() - 1 - i] != substr[substr.length() - 1 - i])
        return false;
    }

    return true;
  }

  constexpr bool ends_with(value_type c) const noexcept { return !empty() && traits_type::eq(back(), c); }
  constexpr bool ends_with(const_pointer c) const noexcept { return ends_with(view(c)); }

  constexpr void swap(basic_fixed_string &other) noexcept { std::swap(value, other.value); }

  inline constexpr const T *c_str() const noexcept { return value; }
  inline constexpr const T *data() const noexcept { return value; }
  inline constexpr std::string_view str() const noexcept { return (std::string_view) *this; }

  constexpr size_type find(view str, size_type pos = 0) const noexcept {
    while (pos + str.length() <= length()) {
      if (traits_type::compare(value + pos, str.data(), str.length()) == 0) {
        return pos;
      }

      pos++;
    }

    return npos;
  }

  constexpr size_type find(value_type c, size_type pos = 0) const noexcept { return find(view(&c, 1), pos); }
  constexpr size_type find(const_pointer str, size_type pos = 0) const noexcept { return find(view(str), pos); }
  constexpr size_type find(const_pointer str, size_type pos, size_type n) const noexcept {
    return find(view(str, n), pos);
  }

  constexpr size_type rfind(view str, size_type pos = npos) const noexcept {
    pos = std::min(pos, length() - str.length());
    while (pos != npos) {
      if (traits_type::compare(value + pos, str.data(), str.length()) == 0) {
        return pos;
      }

      pos--;
    }

    return npos;
  }

  constexpr size_type rfind(value_type c, size_type pos = npos) const noexcept { return rfind(view(&c, 1), pos); }
  constexpr size_type rfind(const_pointer str, size_type pos = npos) const noexcept { return rfind(view(str), pos); }
  constexpr size_type rfind(const_pointer str, size_type pos, size_type n) const noexcept {
    return rfind(view(str, n), pos);
  }

  constexpr size_type find_first_of(view str, size_type pos = 0) const noexcept {
    while (pos < length()) {
      if (traits_type::find(str.data(), str.length(), value[pos]) != nullptr) {
        return pos;
      }

      pos++;
    }

    return npos;
  }

  constexpr size_type find_first_of(value_type c, size_type pos = 0) const noexcept {
    return find_first_of(view(&c, 1), pos);
  }

  constexpr size_type find_first_of(const_pointer str, size_type pos = 0) const noexcept {
    return find_first_of(view(str), pos);
  }

  constexpr size_type find_first_of(const_pointer str, size_type pos, size_type n) const noexcept {
    return find_first_of(view(str, n), pos);
  }

  constexpr size_type find_last_of(view str, size_type pos = npos) const noexcept {
    pos = std::min(pos, length() - 1);
    while (pos != npos) {
      if (traits_type::find(str.data(), str.length(), value[pos]) != nullptr) {
        return pos;
      }

      pos--;
    }

    return npos;
  }

  constexpr size_type find_last_of(value_type c, size_type pos = npos) const noexcept {
    return find_last_of(view(&c, 1), pos);
  }

  constexpr size_type find_last_of(const_pointer str, size_type pos = npos) const noexcept {
    return find_last_of(view(str), pos);
  }

  constexpr size_type find_last_of(const_pointer str, size_type pos, size_type n) const noexcept {
    return find_last_of(view(str, n), pos);
  }

  constexpr size_type find_first_not_of(view str, size_type pos = 0) const noexcept {
    while (pos < length()) {
      if (traits_type::find(str.data(), str.length(), value[pos]) == nullptr) {
        return pos;
      }

      pos++;
    }

    return npos;
  }

  constexpr size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept {
    return find_first_not_of(view(&c, 1), pos);
  }

  constexpr size_type find_first_not_of(const_pointer str, size_type pos = 0) const noexcept {
    return find_first_not_of(view(str), pos);
  }

  constexpr size_type find_first_not_of(const_pointer str, size_type pos, size_type n) const noexcept {
    return find_first_not_of(view(str, n), pos);
  }

  constexpr size_type find_last_not_of(view str, size_type pos = npos) const noexcept {
    pos = std::min(pos, length() - 1);
    while (pos != npos) {
      if (traits_type::find(str.data(), str.length(), value[pos]) == nullptr) {
        return pos;
      }

      pos--;
    }

    return npos;
  }

  constexpr size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept {
    return find_last_not_of(view(&c, 1), pos);
  }

  constexpr size_type find_last_not_of(const_pointer str, size_type pos = npos) const noexcept {
    return find_last_not_of(view(str), pos);
  }

  constexpr size_type find_last_not_of(const_pointer str, size_type pos, size_type n) const noexcept {
    return find_last_not_of(view(str, n), pos);
  }

  constexpr bool contains(view substr) const noexcept {
    return find(substr) != npos;
  }

  constexpr bool contains(value_type c) const noexcept { return find(c) != npos; }
  constexpr bool contains(const_pointer c) const noexcept { return contains(view(c)); }

  T value[N]{0};
};

template<std::size_t N>
struct fixed_string : basic_fixed_string<char, N> {
  constexpr fixed_string(const char(&str)[N]) : basic_fixed_string<char, N>(str) {}
  constexpr fixed_string(const basic_fixed_string<char, N> &other) : basic_fixed_string<char, N>(other) {}
};

template<fixed_string String>
struct literal {};

namespace literals {

} // namespace literals

} // namespace FIXED_STRING_NS

#endif //FIXED_STRING_FIXED_STRING_H
