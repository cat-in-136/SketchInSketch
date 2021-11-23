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

  virtual void setup() override { _setup(); };
  /// loop() which loops consecutively.
  virtual void loop() override { _loop(); };
  /// called when shutdown.
  virtual void teardown() override { _teardown(); };

  /// set sketch object name.
  void setName(const char *name) { _name = name; }
  /// the sketch object name.
  virtual const char *getName() const override { return _name; };

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

  virtual void setup() override {
    _setup();
    terminate();
  };
  /// loop() which loops consecutively.
  virtual void loop() override{/* do nothing */};

  /// set sketch object name.
  void setName(const char *name) { _name = name; }
  /// the sketch object name.
  virtual const char *getName() const override { return _name; };

protected:
  const char *_name = nullptr;
  void (*_setup)();
};

}; // namespace sketchinsketch
