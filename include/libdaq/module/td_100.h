#pragma once

namespace libdaq {
namespace module {
class TD100 {
 public:
  TD100() = default;
  ~TD100() = default;
  static double Convert(double data);
};
}
}