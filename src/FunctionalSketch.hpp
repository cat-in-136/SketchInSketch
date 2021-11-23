#pragma once

#include "Sketch.hpp"

namespace sketchinsketch {

/// Simple functional sketch
class FunctionalSketch : public Sketch {
public:
  // constructor
  FunctionalSketch(void (*setup)(), void (*loop)(), void (*teardown)())
      : _setup(setup), _loop(loop), _teardown(teardown){};
  ~FunctionalSketch(){};

  virtual void setup() { _setup(); };
  /// loop() which loops consecutively.
  virtual void loop() { _loop(); };
  /// called when shutdown.
  virtual void teardown() { _teardown(); };

  /// set sketch object name.
  void setName(const char *name) { _name = name; }
  /// the sketch object name.
  virtual const char *getName() { return _name; };

protected:
  const char *_name = nullptr;
  void (*_setup)();
  void (*_loop)();
  void (*_teardown)();
};

/// Simple one-off functional sketch
class OneOffFunctionalSketch : public Sketch {
public:
  // constructor
  OneOffFunctionalSketch(void (*func)()) : _setup(func){};
  ~OneOffFunctionalSketch(){};

  virtual void setup() {
    _setup();
    terminate();
  };
  /// loop() which loops consecutively.
  virtual void loop(){/* do nothing */};

  /// set sketch object name.
  void setName(const char *name) { _name = name; }
  /// the sketch object name.
  virtual const char *getName() { return _name; };

protected:
  const char *_name = nullptr;
  void (*_setup)();
};

}; // namespace sketchinsketch
