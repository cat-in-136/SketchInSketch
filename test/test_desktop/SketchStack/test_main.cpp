#include <SketchStack.h>
#include <unity.h>

class TestSketchImpl : public sketchstack::Sketch {
public:
  virtual void setup(){};
  virtual void loop(){};
  virtual void teardown(){};
};

void test_sketch_lifecycle() {
  TestSketchImpl sketch;
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::INIT, sketch.getStatus());
  sketch.start();
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::RUNNING, sketch.getStatus());
  sketch.run();
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::RUNNING, sketch.getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::RUNNING, sketch.getStatus());
  sketch.terminate();
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATING, sketch.getStatus());
  sketch.run();
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATING, sketch.getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATED, sketch.getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATED, sketch.getStatus());
}

void test_functional_sketch_lifecycle() {
  static uint8_t setup_called = 0;
  static uint8_t loop_called = 0;
  static uint8_t teardown_called = 0;

  sketchstack::FunctionalSketch sketch(
      [] { setup_called++; }, [] { loop_called++; }, [] { teardown_called++; });

  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::INIT, sketch.getStatus());
  sketch.start();
  TEST_ASSERT_EQUAL(1, setup_called);
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::RUNNING, sketch.getStatus());
  sketch.run();
  TEST_ASSERT_EQUAL(1, loop_called);
  sketch.next();
  sketch.run();
  TEST_ASSERT_EQUAL(2, loop_called);
  sketch.next();
  sketch.terminate();
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATING, sketch.getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(1, setup_called);
  TEST_ASSERT_EQUAL(2, loop_called);
  TEST_ASSERT_EQUAL(1, teardown_called);
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATED, sketch.getStatus());
}

void test_oneoff_functional_sketch_lifecycle() {
  static uint8_t setup_called = 0;

  sketchstack::OneOffFunctionalSketch sketch([] { setup_called++; });

  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::INIT, sketch.getStatus());
  sketch.start();
  TEST_ASSERT_EQUAL(1, setup_called);
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATING, sketch.getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(1, setup_called);
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATED, sketch.getStatus());
}

void test_sketch_switch_lifecycle() {
  static uint8_t sketch1_setup_called = 0;
  static uint8_t sketch1_loop_called = 0;
  static uint8_t sketch1_teardown_called = 0;
  static uint8_t sketch2_setup_called = 0;
  static uint8_t sketch2_loop_called = 0;
  static uint8_t sketch2_teardown_called = 0;
  static uint8_t sketch3_setup_called = 0;
  static uint8_t sketch3_loop_called = 0;
  static uint8_t sketch3_teardown_called = 0;

  auto sketch1=
      new sketchstack::FunctionalSketch([] { sketch1_setup_called++; },
                                        [] { sketch1_loop_called++; },
                                        [] { sketch1_teardown_called++; });
  auto sketch2=
      new sketchstack::FunctionalSketch([] { sketch2_setup_called++; },
                                        [] { sketch2_loop_called++; },
                                        [] { sketch2_teardown_called++; });
  auto sketch3=
      new sketchstack::FunctionalSketch([] { sketch3_setup_called++; },
                                        [] { sketch3_loop_called++; },
                                        [] { sketch3_teardown_called++; });
  sketchstack::SketchSwitch sketch;
  sketch.pushSketch(sketch1);
  sketch.pushSketch(sketch2);

  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::INIT, sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::INIT, sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::INIT, sketch2->getStatus());
  sketch.start();
  TEST_ASSERT_EQUAL(1, sketch1_setup_called);
  TEST_ASSERT_EQUAL(1, sketch2_setup_called);
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::RUNNING, sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::RUNNING, sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::RUNNING, sketch2->getStatus());
  TEST_ASSERT_EQUAL(sketch2, sketch.currentSketch());
  sketch.run();
  TEST_ASSERT_EQUAL(0, sketch1_loop_called);
  TEST_ASSERT_EQUAL(1, sketch2_loop_called);
  sketch.next();
  sketch.pushSketch(sketch3);
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::INIT, sketch3->getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::RUNNING, sketch3->getStatus());
  TEST_ASSERT_EQUAL(1, sketch1_setup_called);
  TEST_ASSERT_EQUAL(1, sketch2_setup_called);
  TEST_ASSERT_EQUAL(1, sketch3_setup_called);
  TEST_ASSERT_EQUAL(sketch3, sketch.currentSketch());
  sketch.run();
  TEST_ASSERT_EQUAL(0, sketch1_loop_called);
  TEST_ASSERT_EQUAL(1, sketch2_loop_called);
  TEST_ASSERT_EQUAL(1, sketch3_loop_called);
  sketch3->terminate();
  sketch.next();
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATED,
                    sketch3->getStatus());
  TEST_ASSERT_EQUAL(sketch2, sketch.currentSketch());
  // sketch2->terminate();
  TEST_ASSERT_EQUAL(sketch2, sketch.popSketch());
  TEST_ASSERT_EQUAL(sketch1, sketch.currentSketch());
  sketch1->terminate();
  sketch.next();
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATED,
                    sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATED, sketch.getStatus());
}

void test_sketch_timeshare_lifecycle() {
  static uint8_t sketch1_setup_called = 0;
  static uint8_t sketch1_loop_called = 0;
  static uint8_t sketch1_teardown_called = 0;
  static uint8_t sketch2_setup_called = 0;

  auto sketch1=
      new sketchstack::FunctionalSketch([] { sketch1_setup_called++; },
                                        [] { sketch1_loop_called++; },
                                        [] { sketch1_teardown_called++; });
  auto sketch2=
      new sketchstack::OneOffFunctionalSketch([] { sketch2_setup_called++; });
  sketchstack::SketchTimeShare sketch;
  sketch.addSketch(sketch1);
  sketch.addSketch(sketch2);

  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::INIT, sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::INIT, sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::INIT, sketch2->getStatus());
  sketch.start();
  TEST_ASSERT_EQUAL(1, sketch1_setup_called);
  TEST_ASSERT_EQUAL(1, sketch2_setup_called);
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::RUNNING, sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::RUNNING, sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATING,
                    sketch2->getStatus());
  sketch.run();
  TEST_ASSERT_EQUAL(1, sketch1_loop_called);
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::RUNNING, sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::RUNNING, sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATING,
                    sketch2->getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::RUNNING, sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::RUNNING, sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATED,
                    sketch2->getStatus());
  sketch.terminate();
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATING, sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATING,
                    sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATED,
                    sketch2->getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATED, sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATED,
                    sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchstack::SketchStatus::TERMINATED,
                    sketch2->getStatus());
}

int main(int argc, char *argv[]) {
  UNITY_BEGIN();
  RUN_TEST(test_sketch_lifecycle);
  RUN_TEST(test_functional_sketch_lifecycle);
  RUN_TEST(test_oneoff_functional_sketch_lifecycle);
  RUN_TEST(test_sketch_switch_lifecycle);
  RUN_TEST(test_sketch_timeshare_lifecycle);
  UNITY_END();

  return 0;
}
