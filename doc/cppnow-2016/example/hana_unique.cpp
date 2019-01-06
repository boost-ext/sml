//
// Copyright (c) 2016-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/hana/tuple.hpp>
#include <boost/hana/unique.hpp>
#include <boost/hana/sort.hpp>

namespace hana = boost::hana;

int main() {
  using namespace hana::literals;
  constexpr auto types = hana::make_tuple(
      1_c, 2_c, 3_c, 4_c, 5_c, 6_c, 7_c, 8_c, 9_c, 10_c, 11_c, 12_c, 13_c, 14_c,
      15_c, 16_c, 17_c, 18_c, 19_c, 20_c, 21_c, 22_c, 23_c, 24_c, 25_c, 26_c,
      27_c, 28_c, 29_c, 30_c, 31_c, 32_c, 33_c, 34_c, 35_c, 36_c, 37_c, 38_c,
      39_c, 40_c, 41_c, 42_c, 43_c, 44_c, 45_c, 46_c, 47_c, 48_c, 49_c, 50_c,
      51_c, 52_c, 53_c, 54_c, 55_c, 56_c, 57_c, 58_c, 59_c, 60_c, 61_c, 62_c,
      63_c, 64_c, 1_c, 65_c, 66_c, 67_c, 68_c, 69_c, 70_c, 71_c, 72_c, 73_c,
      74_c, 75_c, 76_c, 77_c, 78_c, 79_c, 80_c, 81_c, 82_c, 83_c, 84_c, 85_c,
      86_c, 87_c, 88_c, 89_c, 90_c, 91_c, 92_c, 93_c, 94_c, 95_c, 96_c, 97_c,
      98_c, 99_c, 100_c, 101_c, 102_c, 103_c, 104_c, 105_c, 106_c, 107_c, 108_c,
      109_c, 110_c, 111_c, 112_c, 113_c, 114_c, 115_c, 116_c, 117_c, 118_c,
      119_c, 120_c, 121_c, 122_c, 123_c, 124_c, 125_c, 126_c, 127_c, 128_c,
      1_c);

  using u = decltype(hana::unique(hana::sort(types)));
  static_assert(
      std::is_same<
          decltype(hana::make_tuple(
              1_c, 2_c, 3_c, 4_c, 5_c, 6_c, 7_c, 8_c, 9_c, 10_c, 11_c, 12_c,
              13_c, 14_c, 15_c, 16_c, 17_c, 18_c, 19_c, 20_c, 21_c, 22_c, 23_c,
              24_c, 25_c, 26_c, 27_c, 28_c, 29_c, 30_c, 31_c, 32_c, 33_c, 34_c,
              35_c, 36_c, 37_c, 38_c, 39_c, 40_c, 41_c, 42_c, 43_c, 44_c, 45_c,
              46_c, 47_c, 48_c, 49_c, 50_c, 51_c, 52_c, 53_c, 54_c, 55_c, 56_c,
              57_c, 58_c, 59_c, 60_c, 61_c, 62_c, 63_c, 64_c, 65_c, 66_c, 67_c,
              68_c, 69_c, 70_c, 71_c, 72_c, 73_c, 74_c, 75_c, 76_c, 77_c, 78_c,
              79_c, 80_c, 81_c, 82_c, 83_c, 84_c, 85_c, 86_c, 87_c, 88_c, 89_c,
              90_c, 91_c, 92_c, 93_c, 94_c, 95_c, 96_c, 97_c, 98_c, 99_c, 100_c,
              101_c, 102_c, 103_c, 104_c, 105_c, 106_c, 107_c, 108_c, 109_c,
              110_c, 111_c, 112_c, 113_c, 114_c, 115_c, 116_c, 117_c, 118_c,
              119_c, 120_c, 121_c, 122_c, 123_c, 124_c, 125_c, 126_c, 127_c,
              128_c)),
          u>{},
      "");
}
