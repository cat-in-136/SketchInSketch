#pragma once

#include "Sketch.hpp"

#ifdef __AVR__
#include <ArduinoSTL.h>
#endif
#include <list>
#include <memory>
#include <type_traits>

namespace sketchinsketch {

/// Time-share scheduling of multiple sketches
template <class T> class SketchTimeShareEx : public Sketch {
  using T_element = typename std::pointer_traits<T>::element_type;
  static_assert(std::is_convertible<T_element *, Sketch *>::value,
                "T must be convertable to pointer to Sketch.");

public:
  virtual void setup() override {
    for (auto sketch : _sketchList) {
      sketch->begin();
    }
  };

  virtual void loop() override {
    for (auto sketch : _sketchList) {
      switch (sketch->getStatus()) {
      case SketchStatus::INIT:
        sketch->begin();
        sketch->run();
        break;
      case SketchStatus::RUNNING:
        sketch->run();
        break;
      case SketchStatus::TERMINATING:
      case SketchStatus::TERMINATED:
      default:
        // do nothing
        break;
      }
    }
  };

  virtual void teardown() override{
      // do nothing
  };

  virtual void next() override {
    for (auto sketch : _sketchList) {
      sketch->next();
    }
    Sketch::next();
  };

  virtual void terminate() override {
    for (auto sketch : _sketchList) {
      sketch->terminate();
    }
    Sketch::terminate();
  };

  /// Add sketch to list.
  void addSketch(T sketch) { _sketchList.push_back(sketch); };
  /// Remove sketch from list.
  void removeSketch(T sketch) { _sketchList.remove(sketch); };

private:
  std::list<T> _sketchList;
};

using SketchTimeShare = SketchTimeShareEx<Sketch *>;

} // namespace sketchinsketch
