#pragma once

namespace sketchinsketch {

/// Sketch status.
enum SketchStatus {
  /// Initial status
  INIT = 0,
  /// Running status i.e. loop() are called consecutively.
  RUNNING,
  /// Terminating status i.e. it terminates after call teardown().
  TERMINATING,
  /// Terminated status.
  TERMINATED
};

/// Objected sketch
class Sketch {
public:
  /// arduino's setup() which is called when a sketch starts.
  virtual void setup() = 0;
  /// arduino's loop() which loops consecutively.
  virtual void loop() = 0;
  /// it is called when shutdown.
  virtual void teardown(){/* do nothing */};

  /// the sketch object name.
  virtual const char *getName();
  /// get status.
  virtual SketchStatus getStatus();

  /// begin the sketch.
  virtual void begin();
  /// run the sketch.
  virtual void run();
  /// update status for next iteration.
  virtual void next();
  /// terminate this sketch.
  virtual void terminate();

protected:
  /// Sketch status.
  SketchStatus _status = SketchStatus::INIT;
};
}; // namespace sketchinsketch
