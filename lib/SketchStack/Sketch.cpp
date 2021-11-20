#include "Sketch.hpp"

const char *sketchstack::Sketch::getName() { return nullptr; };

sketchstack::SketchStatus sketchstack::Sketch::getStatus() { return _status; };

void sketchstack::Sketch::start() {
  if (_status == SketchStatus::INIT) {
    _status = SketchStatus::RUNNING;
    setup();
  }
}

void sketchstack::Sketch::run() {
  if (_status == SketchStatus::RUNNING) {
    loop();
  }
}

void sketchstack::Sketch::next() {
  if (_status == SketchStatus::TERMINATING) {
    teardown();
    _status = SketchStatus::TERMINATED;
  }
}

void sketchstack::Sketch::terminate() {
  if (_status == SketchStatus::RUNNING) {
    _status = SketchStatus::TERMINATING;
  }
}
