#pragma once

#include "Sketch.hpp"

#ifdef __AVR__
#include <ArduinoSTL.h>
#endif
#include <algorithm>
#include <iterator>
#include <list>
#include <memory>
#include <type_traits>

namespace sketchinsketch {

/// Exclusive switching of multiple sketches
template <class T> class SketchSwitchEx : public Sketch {
  using T_element = typename std::pointer_traits<T>::element_type;
  static_assert(std::is_convertible<T_element *, Sketch *>::value,
                "T must be convertable to pointer to Sketch.");

public:
  /// Constructor.
  ///
  /// @param[in] autoPop whether terminated sketch is automatically popped.
  SketchSwitchEx(bool autoPop = true) : _autoPop(autoPop){};

  virtual void setup() override {
    for (auto sketch : _sketchList) {
      sketch->begin();
    }
  };

  virtual void loop() override {
    auto sketch = currentSketch();
    sketch->run();
  };

  virtual void teardown() override{
      // do nothing
  };

  virtual void next() override {
    bool flag = false;
    while (!flag && !_sketchList.empty()) {
      auto sketch = currentSketch();
      if (sketch->getStatus() == sketchinsketch::SketchStatus::INIT) {
        sketch->begin();
      }
      sketch->next();

      flag = true;
      if (_autoPop) {
        if (sketch->getStatus() == sketchinsketch::SketchStatus::TERMINATED) {
          popSketch();
          flag = false;
        }
      }
    }

    if (_sketchList.empty()) {
      terminate();
    }

    _sketchChanged = _sketchWillBeChanged;
    _sketchWillBeChanged = false;

    Sketch::next();
  };

  virtual void terminate() override {
    for (auto sketch : _sketchList) {
      sketch->terminate();
    }
    Sketch::terminate();
  };

  /// Get current sketch.
  T currentSketch() const { return _sketchList.back(); };

  /// Push and activate sketch.
  void pushSketch(T sketch) {
    _sketchList.push_back(sketch);
    _sketchWillBeChanged = true;
  };

  /// Pop sketch.
  T popSketch() {
    auto back = _sketchList.back();
    _sketchList.pop_back();
    _sketchWillBeChanged = true;
    return back;
  };

  /// Activate sketch.
  void activateSketch(const T sketch) {
    auto result = std::find(_sketchList.begin(), _sketchList.end(), sketch);
    if (result != _sketchList.end()) {
      std::iter_swap(result, --_sketchList.end());
      _sketchWillBeChanged = true;
    }
  };

  /// Insert sketch to list.
  void insertSketchAt(std::size_t n, T sketch) {
    auto pos = std::prev(_sketchList.end(), n);
    _sketchList.insert(pos, sketch);
    _sketchWillBeChanged = _sketchWillBeChanged || (n == 0);
  };

  /// Remove sketch from list.
  void removeSketch(T sketch) { _sketchList.remove(sketch); };

  /// Check if the active sketch is chenged.
  bool isSketchChanged() { return _sketchChanged; };

private:
  bool _autoPop;
  bool _sketchWillBeChanged = false;
  bool _sketchChanged = false;
  std::list<T> _sketchList;
};

using SketchSwitch = SketchSwitchEx<Sketch *>;

} // namespace sketchinsketch
