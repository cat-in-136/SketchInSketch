#include "SketchSwitch.hpp"

void sketchstack::SketchSwitch::setup() {
  for (auto sketch : _sketchList) {
    sketch->start();
  }
}

void sketchstack::SketchSwitch::loop() {
  auto sketch = currentSketch();
  sketch->run();
}

void sketchstack::SketchSwitch::teardown() {
  // do nothing
}

void sketchstack::SketchSwitch::next() {
  do {
    auto sketch = currentSketch();
    if (sketch->getStatus() == sketchstack::SketchStatus::INIT) {
      sketch->start();
    }
    sketch->next();

    if (_autoPop) {
      if (sketch->getStatus() == sketchstack::SketchStatus::TERMINATED) {
        popSketch();
      }
    }
  } while (false);

  if (_sketchList.empty()) {
    terminate();
  }
  Sketch::next();
}

void sketchstack::SketchSwitch::terminate() {
  for (auto sketch : _sketchList) {
    sketch->terminate();
  }
  Sketch::terminate();
}

sketchstack::Sketch *sketchstack::SketchSwitch::currentSketch() {
  return _sketchList.back();
}

void sketchstack::SketchSwitch::pushSketch(sketchstack::Sketch *sketch) {
  _sketchList.push_back(sketch);
}

sketchstack::Sketch *sketchstack::SketchSwitch::popSketch() {
  auto back = _sketchList.back();
  _sketchList.pop_back();
  return back;
}

void sketchstack::SketchSwitch::removeSketch(sketchstack::Sketch *sketch) {
  _sketchList.remove(sketch);
}
