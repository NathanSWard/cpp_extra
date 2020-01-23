Extensions to the c++ standard library that can help simplify code or provide other utility.

* `"algorithm.hpp"`
  * `T min_unused(It first, It last, T value = {})`
    * returns the minimum value between two iterators
      * e.g. `min_unused(1, 2, 3, 5)` -> 4
  * `void adjacent_pair(It first, It last, BinOp op)`
    * calls a binary op for each adjacent pair between two iterators
  * `void for_every_pair(It first, It last, BinOp op)`
    * calls a binary op for every pair conbination between two iterators
  * `std::pair<InIt, OutIt> copy_while(InIt first, InIt last, OutIt result, Pred p)`
    * copies elements from a range into aother range while the values satify a precondition
  * `void split(It first, It last, T const& t, BinOp op)`
    * calls an a binary operation for each pair of iterators that lay between a specified value
* `"bit.hpp"`
  * `To bit_cast<To, From>(From const&)`
    * C++20 function to safely cast from one type to another without causing UB
* `"functional.hpp"`
  * `OutFn bind_front(InFn&&, Args&&...)`
    * C++20's bind_front that binds n arguments to the beginning of a callable
  * `CurriedFn curry(Fn&&)`
    * Partially applies one value at a time to a callable
       * e.g. `curry(add)(1)(2) == add(1, 2)`
  * `Fn compose(Fns&&...)`
    * compose n functions such that compose(a, b, c)(x) == a(b(c(x)))
* `"iterator.hpp"`
   * `zip(Containers&&...)`
     * zip n number of ranges together. `begin()`/`end()` returns a tuple of the ranges iterators
       * note: ranges much have `begin()` and `end()` functions
   * `czip()` `rzip()` `crzip()`
     * similiar to zip() except ranges must provide c/r/cr`begin()` and c/r/cr`end()` functions
* `"memory.hpp"`
   * `ptr<T>`
     * A non-owning smart pointer type.
     * Implicitly convertible from all pointer/_ptr types.
   * `non_null_ptr<T>`
     * A non-owning, never null, smart pointer type.
     * Impllicitly convertible form all pointer/_ptr types.
* `"string.hpp"`
  * `class strtok`
    * `strtok::strtok(std::string_view str)`
      * `str`: the string to split.
    * `std::string_view strtok::operator(std::string_view tokens)`
      * `tokens`: the tokens to split the string on.
      * Can be called multiple times, will return an empty view once the entire string is split.
    * `std::string_view strtok::operator(std::string_view str, std::string_view tokens)`
      * `str`: string to have the `strtok` object split.
      * `tokens`: the tokens to split the string on.
  * `std::vector<std::string_view> strtok_all(std::string_view str, std::string_view tokens)`
    * Similar to the `strtok` class, however, `str` is split all at once returning a vector of all split views.
    * e.g. `assert(strtok_all("hello world", " ") == std::vector{"hello"sv, "world"sv});`
* `"tuple.hpp"`
  * `void for_each(Fn&&, Tuple&&)`
    * Apply a function to each element in a tuple
* `"type_traits.hpp"`
  * C++20
    * `remove_cvref<T>`
    * `is_unbounded_array<T>`
  * `remove_ref_warp`
    * remove a reference wrapper return a type T&
  * `contains<T, C>`
    * `::value` if true if T is in templated container C
  * `remove_duplicates<C, Ts...>`
    * `::type` is a C<Us...> where Us are all Ts... removing duplicate types
  * `has_tupe<T, Us...>`
    * `::value` is true is T is found int Us...
* `"variant.hpp"`
  * `template<class... Fns> struct overloaded;`
    * useful utility when using std::visit on a variant.
      * e.g. `std::visit(overload{/*lambdas here*/}, variant);`
