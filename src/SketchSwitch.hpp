#pragma once

#include "Sketch.hpp"
#include <list>

namespace sketchinsketch {

/// Exclusive switching of multiple sketches
class SketchSwitch : public Sketch {
public:
  /// Constructor.
  ///
  /// @param[in] autoPop whether terminated sketch is automatically popped.
  SketchSwitch(bool autoPop = true) : _autoPop(autoPop){};

  virtual void setup() override;
  virtual void loop() override;
  virtual void teardown() override;
  virtual void next() override;
  virtual void terminate() override;

  /// Get current sketch.
  Sketch *currentSketch();
  /// Push and activate sketch.
  void pushSketch(Sketch *sketch);
  /// Pop sketch.
  Sketch *popSketch();
  /// Activate sketch.
  void activateSketch(Sketch *sketch);
  /// Remove sketch from list.
  void removeSketch(Sketch *sketch);

private:
  bool _autoPop;
  std::list<Sketch *> _sketchList;
};

} // namespace sketchinsketch
