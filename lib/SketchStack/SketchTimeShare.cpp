#include "SketchTimeShare.hpp"

void sketchstack::SketchTimeShare::setup() {
  for (auto sketch : _sketchList) {
    sketch->start();
  }
}

void sketchstack::SketchTimeShare::loop() {
  for (auto sketch : _sketchList) {
    switch (sketch->getStatus()) {
    case SketchStatus::INIT:
      sketch->start();
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
}

void sketchstack::SketchTimeShare::teardown() {
  // do nothing
}

void sketchstack::SketchTimeShare::next() {
  for (auto sketch : _sketchList) {
    sketch->next();
  }
  Sketch::next();
}

void sketchstack::SketchTimeShare::terminate() {
  for (auto sketch : _sketchList) {
    sketch->terminate();
  }
  Sketch::terminate();
}

void sketchstack::SketchTimeShare::addSketch(Sketch *sketch) {
  _sketchList.push_back(sketch);
}

void sketchstack::SketchTimeShare::removeSketch(Sketch *sketch) {
  _sketchList.remove(sketch);
}
