#pragma once

#include "Sketch.hpp"

#ifdef __AVR__
#include <ArduinoSTL.h>
#endif
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
  Sketch *currentSketch() const;
  /// Push and activate sketch.
  void pushSketch(Sketch *sketch);
  /// Pop sketch.
  Sketch *popSketch();
  /// Activate sketch.
  void activateSketch(const Sketch *sketch);
  /// Insert sketch to list.
  void insertSketchAt(std::size_t n, Sketch *sketch);
  /// Remove sketch from list.
  void removeSketch(Sketch *sketch);

  /// Check if the active sketch is chenged.
  bool isSketchChanged() { return _sketchChanged; };

private:
  bool _autoPop;
  bool _sketchWillBeChanged = false;
  bool _sketchChanged = false;
  std::list<Sketch *> _sketchList;
};

} // namespace sketchinsketch
