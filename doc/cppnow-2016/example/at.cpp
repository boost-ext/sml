//
// Copyright (c) 2016-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>

template <class, class> struct pair {};

template <class... Ts> struct map : Ts... {};

template <class T> struct no_decay { using type = T; };

template <class TDefault, class> static no_decay<TDefault> lookup(...);

template <class, class TKey, class TValue>
static no_decay<TValue> lookup(pair<TKey, TValue> *);

template <class TDefault, class TKey, class T>
using at_key = decltype(lookup<TDefault, TKey>((T *)0));

template <class T, class TKey, class TDefault = void>
using at_key_t = typename at_key<TDefault, TKey, T>::type;

int main() {
  using m = map<
      pair<std::integral_constant<int, 1>, std::integral_constant<int, 1>>,
      pair<std::integral_constant<int, 2>, std::integral_constant<int, 2>>,
      pair<std::integral_constant<int, 3>, std::integral_constant<int, 3>>,
      pair<std::integral_constant<int, 4>, std::integral_constant<int, 4>>,
      pair<std::integral_constant<int, 5>, std::integral_constant<int, 5>>,
      pair<std::integral_constant<int, 6>, std::integral_constant<int, 6>>,
      pair<std::integral_constant<int, 7>, std::integral_constant<int, 7>>,
      pair<std::integral_constant<int, 8>, std::integral_constant<int, 8>>,
      pair<std::integral_constant<int, 9>, std::integral_constant<int, 9>>,
      pair<std::integral_constant<int, 10>, std::integral_constant<int, 10>>,
      pair<std::integral_constant<int, 11>, std::integral_constant<int, 11>>,
      pair<std::integral_constant<int, 12>, std::integral_constant<int, 12>>,
      pair<std::integral_constant<int, 13>, std::integral_constant<int, 13>>,
      pair<std::integral_constant<int, 14>, std::integral_constant<int, 14>>,
      pair<std::integral_constant<int, 15>, std::integral_constant<int, 15>>,
      pair<std::integral_constant<int, 16>, std::integral_constant<int, 16>>,
      pair<std::integral_constant<int, 17>, std::integral_constant<int, 17>>,
      pair<std::integral_constant<int, 18>, std::integral_constant<int, 18>>,
      pair<std::integral_constant<int, 19>, std::integral_constant<int, 19>>,
      pair<std::integral_constant<int, 20>, std::integral_constant<int, 20>>,
      pair<std::integral_constant<int, 21>, std::integral_constant<int, 21>>,
      pair<std::integral_constant<int, 22>, std::integral_constant<int, 22>>,
      pair<std::integral_constant<int, 23>, std::integral_constant<int, 23>>,
      pair<std::integral_constant<int, 24>, std::integral_constant<int, 24>>,
      pair<std::integral_constant<int, 25>, std::integral_constant<int, 25>>,
      pair<std::integral_constant<int, 26>, std::integral_constant<int, 26>>,
      pair<std::integral_constant<int, 27>, std::integral_constant<int, 27>>,
      pair<std::integral_constant<int, 28>, std::integral_constant<int, 28>>,
      pair<std::integral_constant<int, 29>, std::integral_constant<int, 29>>,
      pair<std::integral_constant<int, 30>, std::integral_constant<int, 30>>,
      pair<std::integral_constant<int, 31>, std::integral_constant<int, 31>>,
      pair<std::integral_constant<int, 32>, std::integral_constant<int, 32>>,
      pair<std::integral_constant<int, 33>, std::integral_constant<int, 33>>,
      pair<std::integral_constant<int, 34>, std::integral_constant<int, 34>>,
      pair<std::integral_constant<int, 35>, std::integral_constant<int, 35>>,
      pair<std::integral_constant<int, 36>, std::integral_constant<int, 36>>,
      pair<std::integral_constant<int, 37>, std::integral_constant<int, 37>>,
      pair<std::integral_constant<int, 38>, std::integral_constant<int, 38>>,
      pair<std::integral_constant<int, 39>, std::integral_constant<int, 39>>,
      pair<std::integral_constant<int, 40>, std::integral_constant<int, 40>>,
      pair<std::integral_constant<int, 41>, std::integral_constant<int, 41>>,
      pair<std::integral_constant<int, 42>, std::integral_constant<int, 42>>,
      pair<std::integral_constant<int, 43>, std::integral_constant<int, 43>>,
      pair<std::integral_constant<int, 44>, std::integral_constant<int, 44>>,
      pair<std::integral_constant<int, 45>, std::integral_constant<int, 45>>,
      pair<std::integral_constant<int, 46>, std::integral_constant<int, 46>>,
      pair<std::integral_constant<int, 47>, std::integral_constant<int, 47>>,
      pair<std::integral_constant<int, 48>, std::integral_constant<int, 48>>,
      pair<std::integral_constant<int, 49>, std::integral_constant<int, 49>>,
      pair<std::integral_constant<int, 50>, std::integral_constant<int, 50>>,
      pair<std::integral_constant<int, 51>, std::integral_constant<int, 51>>,
      pair<std::integral_constant<int, 52>, std::integral_constant<int, 52>>,
      pair<std::integral_constant<int, 53>, std::integral_constant<int, 53>>,
      pair<std::integral_constant<int, 54>, std::integral_constant<int, 54>>,
      pair<std::integral_constant<int, 55>, std::integral_constant<int, 55>>,
      pair<std::integral_constant<int, 56>, std::integral_constant<int, 56>>,
      pair<std::integral_constant<int, 57>, std::integral_constant<int, 57>>,
      pair<std::integral_constant<int, 58>, std::integral_constant<int, 58>>,
      pair<std::integral_constant<int, 59>, std::integral_constant<int, 59>>,
      pair<std::integral_constant<int, 60>, std::integral_constant<int, 60>>,
      pair<std::integral_constant<int, 61>, std::integral_constant<int, 61>>,
      pair<std::integral_constant<int, 62>, std::integral_constant<int, 62>>,
      pair<std::integral_constant<int, 63>, std::integral_constant<int, 63>>,
      pair<std::integral_constant<int, 64>, std::integral_constant<int, 64>>,
      pair<std::integral_constant<int, 65>, std::integral_constant<int, 65>>,
      pair<std::integral_constant<int, 66>, std::integral_constant<int, 66>>,
      pair<std::integral_constant<int, 67>, std::integral_constant<int, 67>>,
      pair<std::integral_constant<int, 68>, std::integral_constant<int, 68>>,
      pair<std::integral_constant<int, 69>, std::integral_constant<int, 69>>,
      pair<std::integral_constant<int, 70>, std::integral_constant<int, 70>>,
      pair<std::integral_constant<int, 71>, std::integral_constant<int, 71>>,
      pair<std::integral_constant<int, 72>, std::integral_constant<int, 72>>,
      pair<std::integral_constant<int, 73>, std::integral_constant<int, 73>>,
      pair<std::integral_constant<int, 74>, std::integral_constant<int, 74>>,
      pair<std::integral_constant<int, 75>, std::integral_constant<int, 75>>,
      pair<std::integral_constant<int, 76>, std::integral_constant<int, 76>>,
      pair<std::integral_constant<int, 77>, std::integral_constant<int, 77>>,
      pair<std::integral_constant<int, 78>, std::integral_constant<int, 78>>,
      pair<std::integral_constant<int, 79>, std::integral_constant<int, 79>>,
      pair<std::integral_constant<int, 80>, std::integral_constant<int, 80>>,
      pair<std::integral_constant<int, 81>, std::integral_constant<int, 81>>,
      pair<std::integral_constant<int, 82>, std::integral_constant<int, 82>>,
      pair<std::integral_constant<int, 83>, std::integral_constant<int, 83>>,
      pair<std::integral_constant<int, 84>, std::integral_constant<int, 84>>,
      pair<std::integral_constant<int, 85>, std::integral_constant<int, 85>>,
      pair<std::integral_constant<int, 86>, std::integral_constant<int, 86>>,
      pair<std::integral_constant<int, 87>, std::integral_constant<int, 87>>,
      pair<std::integral_constant<int, 88>, std::integral_constant<int, 88>>,
      pair<std::integral_constant<int, 89>, std::integral_constant<int, 89>>,
      pair<std::integral_constant<int, 90>, std::integral_constant<int, 90>>,
      pair<std::integral_constant<int, 91>, std::integral_constant<int, 91>>,
      pair<std::integral_constant<int, 92>, std::integral_constant<int, 92>>,
      pair<std::integral_constant<int, 93>, std::integral_constant<int, 93>>,
      pair<std::integral_constant<int, 94>, std::integral_constant<int, 94>>,
      pair<std::integral_constant<int, 95>, std::integral_constant<int, 95>>,
      pair<std::integral_constant<int, 96>, std::integral_constant<int, 96>>,
      pair<std::integral_constant<int, 97>, std::integral_constant<int, 97>>,
      pair<std::integral_constant<int, 98>, std::integral_constant<int, 98>>,
      pair<std::integral_constant<int, 99>, std::integral_constant<int, 99>>,
      pair<std::integral_constant<int, 100>, std::integral_constant<int, 100>>,
      pair<std::integral_constant<int, 101>, std::integral_constant<int, 101>>,
      pair<std::integral_constant<int, 102>, std::integral_constant<int, 102>>,
      pair<std::integral_constant<int, 103>, std::integral_constant<int, 103>>,
      pair<std::integral_constant<int, 104>, std::integral_constant<int, 104>>,
      pair<std::integral_constant<int, 105>, std::integral_constant<int, 105>>,
      pair<std::integral_constant<int, 106>, std::integral_constant<int, 106>>,
      pair<std::integral_constant<int, 107>, std::integral_constant<int, 107>>,
      pair<std::integral_constant<int, 108>, std::integral_constant<int, 108>>,
      pair<std::integral_constant<int, 109>, std::integral_constant<int, 109>>,
      pair<std::integral_constant<int, 110>, std::integral_constant<int, 110>>,
      pair<std::integral_constant<int, 111>, std::integral_constant<int, 111>>,
      pair<std::integral_constant<int, 112>, std::integral_constant<int, 112>>,
      pair<std::integral_constant<int, 113>, std::integral_constant<int, 113>>,
      pair<std::integral_constant<int, 114>, std::integral_constant<int, 114>>,
      pair<std::integral_constant<int, 115>, std::integral_constant<int, 115>>,
      pair<std::integral_constant<int, 116>, std::integral_constant<int, 116>>,
      pair<std::integral_constant<int, 117>, std::integral_constant<int, 117>>,
      pair<std::integral_constant<int, 118>, std::integral_constant<int, 118>>,
      pair<std::integral_constant<int, 119>, std::integral_constant<int, 119>>,
      pair<std::integral_constant<int, 120>, std::integral_constant<int, 120>>,
      pair<std::integral_constant<int, 121>, std::integral_constant<int, 121>>,
      pair<std::integral_constant<int, 122>, std::integral_constant<int, 122>>,
      pair<std::integral_constant<int, 123>, std::integral_constant<int, 123>>,
      pair<std::integral_constant<int, 124>, std::integral_constant<int, 124>>,
      pair<std::integral_constant<int, 125>, std::integral_constant<int, 125>>,
      pair<std::integral_constant<int, 126>, std::integral_constant<int, 126>>,
      pair<std::integral_constant<int, 127>, std::integral_constant<int, 127>>,
      pair<std::integral_constant<int, 128>, std::integral_constant<int, 128>>>;

  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 1>>,
                             std::integral_constant<int, 1>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 2>>,
                             std::integral_constant<int, 2>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 3>>,
                             std::integral_constant<int, 3>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 4>>,
                             std::integral_constant<int, 4>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 5>>,
                             std::integral_constant<int, 5>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 6>>,
                             std::integral_constant<int, 6>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 7>>,
                             std::integral_constant<int, 7>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 8>>,
                             std::integral_constant<int, 8>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 9>>,
                             std::integral_constant<int, 9>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 10>>,
                             std::integral_constant<int, 10>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 11>>,
                             std::integral_constant<int, 11>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 12>>,
                             std::integral_constant<int, 12>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 13>>,
                             std::integral_constant<int, 13>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 14>>,
                             std::integral_constant<int, 14>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 15>>,
                             std::integral_constant<int, 15>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 16>>,
                             std::integral_constant<int, 16>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 17>>,
                             std::integral_constant<int, 17>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 18>>,
                             std::integral_constant<int, 18>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 19>>,
                             std::integral_constant<int, 19>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 20>>,
                             std::integral_constant<int, 20>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 21>>,
                             std::integral_constant<int, 21>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 22>>,
                             std::integral_constant<int, 22>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 23>>,
                             std::integral_constant<int, 23>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 24>>,
                             std::integral_constant<int, 24>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 25>>,
                             std::integral_constant<int, 25>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 26>>,
                             std::integral_constant<int, 26>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 27>>,
                             std::integral_constant<int, 27>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 28>>,
                             std::integral_constant<int, 28>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 29>>,
                             std::integral_constant<int, 29>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 30>>,
                             std::integral_constant<int, 30>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 31>>,
                             std::integral_constant<int, 31>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 32>>,
                             std::integral_constant<int, 32>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 33>>,
                             std::integral_constant<int, 33>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 34>>,
                             std::integral_constant<int, 34>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 35>>,
                             std::integral_constant<int, 35>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 36>>,
                             std::integral_constant<int, 36>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 37>>,
                             std::integral_constant<int, 37>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 38>>,
                             std::integral_constant<int, 38>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 39>>,
                             std::integral_constant<int, 39>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 40>>,
                             std::integral_constant<int, 40>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 41>>,
                             std::integral_constant<int, 41>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 42>>,
                             std::integral_constant<int, 42>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 43>>,
                             std::integral_constant<int, 43>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 44>>,
                             std::integral_constant<int, 44>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 45>>,
                             std::integral_constant<int, 45>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 46>>,
                             std::integral_constant<int, 46>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 47>>,
                             std::integral_constant<int, 47>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 48>>,
                             std::integral_constant<int, 48>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 49>>,
                             std::integral_constant<int, 49>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 50>>,
                             std::integral_constant<int, 50>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 51>>,
                             std::integral_constant<int, 51>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 52>>,
                             std::integral_constant<int, 52>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 53>>,
                             std::integral_constant<int, 53>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 54>>,
                             std::integral_constant<int, 54>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 55>>,
                             std::integral_constant<int, 55>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 56>>,
                             std::integral_constant<int, 56>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 57>>,
                             std::integral_constant<int, 57>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 58>>,
                             std::integral_constant<int, 58>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 59>>,
                             std::integral_constant<int, 59>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 60>>,
                             std::integral_constant<int, 60>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 61>>,
                             std::integral_constant<int, 61>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 62>>,
                             std::integral_constant<int, 62>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 63>>,
                             std::integral_constant<int, 63>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 64>>,
                             std::integral_constant<int, 64>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 65>>,
                             std::integral_constant<int, 65>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 66>>,
                             std::integral_constant<int, 66>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 67>>,
                             std::integral_constant<int, 67>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 68>>,
                             std::integral_constant<int, 68>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 69>>,
                             std::integral_constant<int, 69>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 70>>,
                             std::integral_constant<int, 70>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 71>>,
                             std::integral_constant<int, 71>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 72>>,
                             std::integral_constant<int, 72>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 73>>,
                             std::integral_constant<int, 73>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 74>>,
                             std::integral_constant<int, 74>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 75>>,
                             std::integral_constant<int, 75>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 76>>,
                             std::integral_constant<int, 76>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 77>>,
                             std::integral_constant<int, 77>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 78>>,
                             std::integral_constant<int, 78>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 79>>,
                             std::integral_constant<int, 79>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 80>>,
                             std::integral_constant<int, 80>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 81>>,
                             std::integral_constant<int, 81>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 82>>,
                             std::integral_constant<int, 82>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 83>>,
                             std::integral_constant<int, 83>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 84>>,
                             std::integral_constant<int, 84>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 85>>,
                             std::integral_constant<int, 85>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 86>>,
                             std::integral_constant<int, 86>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 87>>,
                             std::integral_constant<int, 87>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 88>>,
                             std::integral_constant<int, 88>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 89>>,
                             std::integral_constant<int, 89>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 90>>,
                             std::integral_constant<int, 90>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 91>>,
                             std::integral_constant<int, 91>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 92>>,
                             std::integral_constant<int, 92>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 93>>,
                             std::integral_constant<int, 93>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 94>>,
                             std::integral_constant<int, 94>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 95>>,
                             std::integral_constant<int, 95>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 96>>,
                             std::integral_constant<int, 96>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 97>>,
                             std::integral_constant<int, 97>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 98>>,
                             std::integral_constant<int, 98>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 99>>,
                             std::integral_constant<int, 99>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 100>>,
                             std::integral_constant<int, 100>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 101>>,
                             std::integral_constant<int, 101>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 102>>,
                             std::integral_constant<int, 102>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 103>>,
                             std::integral_constant<int, 103>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 104>>,
                             std::integral_constant<int, 104>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 105>>,
                             std::integral_constant<int, 105>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 106>>,
                             std::integral_constant<int, 106>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 107>>,
                             std::integral_constant<int, 107>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 108>>,
                             std::integral_constant<int, 108>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 109>>,
                             std::integral_constant<int, 109>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 110>>,
                             std::integral_constant<int, 110>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 111>>,
                             std::integral_constant<int, 111>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 112>>,
                             std::integral_constant<int, 112>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 113>>,
                             std::integral_constant<int, 113>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 114>>,
                             std::integral_constant<int, 114>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 115>>,
                             std::integral_constant<int, 115>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 116>>,
                             std::integral_constant<int, 116>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 117>>,
                             std::integral_constant<int, 117>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 118>>,
                             std::integral_constant<int, 118>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 119>>,
                             std::integral_constant<int, 119>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 120>>,
                             std::integral_constant<int, 120>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 121>>,
                             std::integral_constant<int, 121>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 122>>,
                             std::integral_constant<int, 122>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 123>>,
                             std::integral_constant<int, 123>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 124>>,
                             std::integral_constant<int, 124>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 125>>,
                             std::integral_constant<int, 125>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 126>>,
                             std::integral_constant<int, 126>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 127>>,
                             std::integral_constant<int, 127>>{},
                "");
  static_assert(std::is_same<at_key_t<m, std::integral_constant<int, 128>>,
                             std::integral_constant<int, 128>>{},
                "");
}
