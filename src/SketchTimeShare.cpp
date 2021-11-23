#include "SketchTimeShare.hpp"

void sketchinsketch::SketchTimeShare::setup() {
  for (auto sketch : _sketchList) {
    sketch->start();
  }
}

void sketchinsketch::SketchTimeShare::loop() {
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

void sketchinsketch::SketchTimeShare::teardown() {
  // do nothing
}

void sketchinsketch::SketchTimeShare::next() {
  for (auto sketch : _sketchList) {
    sketch->next();
  }
  Sketch::next();
}

void sketchinsketch::SketchTimeShare::terminate() {
  for (auto sketch : _sketchList) {
    sketch->terminate();
  }
  Sketch::terminate();
}

void sketchinsketch::SketchTimeShare::addSketch(Sketch *sketch) {
  _sketchList.push_back(sketch);
}

void sketchinsketch::SketchTimeShare::removeSketch(Sketch *sketch) {
  _sketchList.remove(sketch);
}
