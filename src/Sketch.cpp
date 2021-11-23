#include "Sketch.hpp"

const char *sketchinsketch::Sketch::getName() { return nullptr; };

sketchinsketch::SketchStatus sketchinsketch::Sketch::getStatus() {
  return _status;
};

void sketchinsketch::Sketch::begin() {
  if (_status == SketchStatus::INIT) {
    _status = SketchStatus::RUNNING;
    setup();
  }
}

void sketchinsketch::Sketch::run() {
  if (_status == SketchStatus::RUNNING) {
    loop();
  }
}

void sketchinsketch::Sketch::next() {
  if (_status == SketchStatus::TERMINATING) {
    teardown();
    _status = SketchStatus::TERMINATED;
  }
}

void sketchinsketch::Sketch::terminate() {
  if (_status == SketchStatus::RUNNING) {
    _status = SketchStatus::TERMINATING;
  }
}
