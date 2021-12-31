#include "SketchSwitch.hpp"

#ifdef __AVR__
#include <ArduinoSTL.h>
#endif
#include <algorithm>
#include <iterator>

void sketchinsketch::SketchSwitch::setup() {
  for (auto sketch : _sketchList) {
    sketch->begin();
  }
}

void sketchinsketch::SketchSwitch::loop() {
  auto sketch = currentSketch();
  sketch->run();
}

void sketchinsketch::SketchSwitch::teardown() {
  // do nothing
}

void sketchinsketch::SketchSwitch::next() {
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
}

void sketchinsketch::SketchSwitch::terminate() {
  for (auto sketch : _sketchList) {
    sketch->terminate();
  }
  Sketch::terminate();
}

sketchinsketch::Sketch *sketchinsketch::SketchSwitch::currentSketch() const {
  return _sketchList.back();
}

void sketchinsketch::SketchSwitch::pushSketch(sketchinsketch::Sketch *sketch) {
  _sketchList.push_back(sketch);
  _sketchWillBeChanged = true;
}

sketchinsketch::Sketch *sketchinsketch::SketchSwitch::popSketch() {
  auto back = _sketchList.back();
  _sketchList.pop_back();
  _sketchWillBeChanged = true;
  return back;
}

void sketchinsketch::SketchSwitch::activateSketch(
    const sketchinsketch::Sketch *sketch) {
  auto result = std::find(_sketchList.begin(), _sketchList.end(), sketch);
  if (result != _sketchList.end()) {
    std::iter_swap(result, --_sketchList.end());
    _sketchWillBeChanged = true;
  }
}

void sketchinsketch::SketchSwitch::insertSketchAt(
    std::size_t n, sketchinsketch::Sketch *sketch) {
  auto pos = std::prev(_sketchList.end(), n);
  _sketchList.insert(pos, sketch);
  _sketchWillBeChanged = _sketchWillBeChanged || (n == 0);
}

void sketchinsketch::SketchSwitch::removeSketch(
    sketchinsketch::Sketch *sketch) {
  _sketchList.remove(sketch);
}
