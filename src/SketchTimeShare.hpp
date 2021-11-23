#ifndef _SKETCH_STACK_SKETCH_TIME_SHARE_H_
#define _SKETCH_STACK_SKETCH_TIME_SHARE_H_

#include "Sketch.hpp"
#include <list>

namespace sketchinsketch {

/// Time-share scheduling of multiple sketches
class SketchTimeShare : public Sketch {
public:
  virtual void setup() override;
  virtual void loop() override;
  virtual void teardown() override;
  virtual void next() override;
  virtual void terminate() override;

  /// Add sketch to list.
  void addSketch(Sketch *sketch);
  /// Remove sketch from list.
  void removeSketch(Sketch *sketch);

private:
  std::list<Sketch *> _sketchList;
};

} // namespace sketchinsketch

#endif
