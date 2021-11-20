#ifndef _SKETCH_STACK_SKETCH_SWITCH_H_
#define _SKETCH_STACK_SKETCH_SWITCH_H_

#include "Sketch.hpp"
#include <list>
#include <memory>

namespace sketchstack {

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
  std::shared_ptr<Sketch> currentSketch();
  /// Push and activate sketch.
  void pushSketch(std::shared_ptr<Sketch> sketch);
  /// Pop sketch.
  std::shared_ptr<Sketch> popSketch();
  /// Activate sketch.
  void activateSketch(std::shared_ptr<Sketch> sketch);
  /// Remove sketch from list.
  void removeSketch(std::shared_ptr<Sketch> sketch);

private:
  bool _autoPop;
  std::list<std::shared_ptr<Sketch>> _sketchList;
};

} // namespace sketchstack

#endif
