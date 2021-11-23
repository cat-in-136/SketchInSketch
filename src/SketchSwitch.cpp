#include "SketchSwitch.hpp"

void sketchinsketch::SketchSwitch::setup() {
  for (auto sketch : _sketchList) {
    sketch->start();
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
  do {
    auto sketch = currentSketch();
    if (sketch->getStatus() == sketchinsketch::SketchStatus::INIT) {
      sketch->start();
    }
    sketch->next();

    if (_autoPop) {
      if (sketch->getStatus() == sketchinsketch::SketchStatus::TERMINATED) {
        popSketch();
      }
    }
  } while (false);

  if (_sketchList.empty()) {
    terminate();
  }
  Sketch::next();
}

void sketchinsketch::SketchSwitch::terminate() {
  for (auto sketch : _sketchList) {
    sketch->terminate();
  }
  Sketch::terminate();
}

sketchinsketch::Sketch *sketchinsketch::SketchSwitch::currentSketch() {
  return _sketchList.back();
}

void sketchinsketch::SketchSwitch::pushSketch(sketchinsketch::Sketch *sketch) {
  _sketchList.push_back(sketch);
}

sketchinsketch::Sketch *sketchinsketch::SketchSwitch::popSketch() {
  auto back = _sketchList.back();
  _sketchList.pop_back();
  return back;
}

void sketchinsketch::SketchSwitch::removeSketch(sketchinsketch::Sketch *sketch) {
  _sketchList.remove(sketch);
}
