#include <SketchInSketch.h>
#include <array>
#include <unity.h>

#ifdef ARDUINO
#include <Arduino.h>
#endif

class TestSketchImpl : public sketchinsketch::Sketch {
public:
  virtual void setup(){};
  virtual void loop(){};
  virtual void teardown(){};
};

void test_sketch_lifecycle() {
  TestSketchImpl sketch;
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::INIT, sketch.getStatus());
  sketch.begin();
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::RUNNING, sketch.getStatus());
  sketch.run();
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::RUNNING, sketch.getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::RUNNING, sketch.getStatus());
  sketch.terminate();
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATING,
                    sketch.getStatus());
  sketch.run();
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATING,
                    sketch.getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATED,
                    sketch.getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATED,
                    sketch.getStatus());
}

void test_functional_sketch_lifecycle() {
  static uint8_t setup_called = 0;
  static uint8_t loop_called = 0;
  static uint8_t teardown_called = 0;

  sketchinsketch::FunctionalSketch sketch(
      [] { setup_called++; }, [] { loop_called++; }, [] { teardown_called++; });

  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::INIT, sketch.getStatus());
  sketch.begin();
  TEST_ASSERT_EQUAL(1, setup_called);
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::RUNNING, sketch.getStatus());
  sketch.run();
  TEST_ASSERT_EQUAL(1, loop_called);
  sketch.next();
  sketch.run();
  TEST_ASSERT_EQUAL(2, loop_called);
  sketch.next();
  sketch.terminate();
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATING,
                    sketch.getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(1, setup_called);
  TEST_ASSERT_EQUAL(2, loop_called);
  TEST_ASSERT_EQUAL(1, teardown_called);
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATED,
                    sketch.getStatus());
}

void test_oneoff_functional_sketch_lifecycle() {
  static uint8_t setup_called = 0;

  sketchinsketch::OneOffFunctionalSketch sketch([] { setup_called++; });

  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::INIT, sketch.getStatus());
  sketch.begin();
  TEST_ASSERT_EQUAL(1, setup_called);
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATING,
                    sketch.getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(1, setup_called);
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATED,
                    sketch.getStatus());
}

void test_sketch_switch_list_operation() {
  std::array<sketchinsketch::OneOffFunctionalSketch *, 8> children;
  sketchinsketch::SketchSwitch sketch;
  for (uint8_t i = 0; i < children.size(); i++) {
    children[i] = new sketchinsketch::OneOffFunctionalSketch([] {});
    sketch.pushSketch(children[i]);
  }

  TEST_ASSERT_EQUAL(children[7], sketch.popSketch());
  TEST_ASSERT_EQUAL(children[6], sketch.currentSketch());
  sketch.activateSketch(children[4]);
  TEST_ASSERT_EQUAL(children[4], sketch.popSketch());
  sketch.removeSketch(children[5]);
  TEST_ASSERT_EQUAL(children[6], sketch.popSketch());
  TEST_ASSERT_EQUAL(children[3], sketch.popSketch());
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

  auto sketch1 = new sketchinsketch::FunctionalSketch(
      [] { sketch1_setup_called++; }, [] { sketch1_loop_called++; },
      [] { sketch1_teardown_called++; });
  auto sketch2 = new sketchinsketch::FunctionalSketch(
      [] { sketch2_setup_called++; }, [] { sketch2_loop_called++; },
      [] { sketch2_teardown_called++; });
  auto sketch3 = new sketchinsketch::FunctionalSketch(
      [] { sketch3_setup_called++; }, [] { sketch3_loop_called++; },
      [] { sketch3_teardown_called++; });
  sketchinsketch::SketchSwitch sketch;
  sketch.pushSketch(sketch1);
  sketch.pushSketch(sketch2);

  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::INIT, sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::INIT, sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::INIT, sketch2->getStatus());
  sketch.begin();
  TEST_ASSERT_EQUAL(1, sketch1_setup_called);
  TEST_ASSERT_EQUAL(1, sketch2_setup_called);
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::RUNNING, sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::RUNNING,
                    sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::RUNNING,
                    sketch2->getStatus());
  TEST_ASSERT_EQUAL(sketch2, sketch.currentSketch());
  sketch.run();
  TEST_ASSERT_EQUAL(0, sketch1_loop_called);
  TEST_ASSERT_EQUAL(1, sketch2_loop_called);
  sketch.next();
  sketch.pushSketch(sketch3);
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::INIT, sketch3->getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::RUNNING,
                    sketch3->getStatus());
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
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATED,
                    sketch3->getStatus());
  TEST_ASSERT_EQUAL(sketch2, sketch.currentSketch());
  // sketch2->terminate();
  TEST_ASSERT_EQUAL(sketch2, sketch.popSketch());
  TEST_ASSERT_EQUAL(sketch1, sketch.currentSketch());
  sketch1->terminate();
  sketch.next();
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATED,
                    sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATED,
                    sketch.getStatus());
}

void test_sketch_timeshare_lifecycle() {
  static uint8_t sketch1_setup_called = 0;
  static uint8_t sketch1_loop_called = 0;
  static uint8_t sketch1_teardown_called = 0;
  static uint8_t sketch2_setup_called = 0;

  auto sketch1 = new sketchinsketch::FunctionalSketch(
      [] { sketch1_setup_called++; }, [] { sketch1_loop_called++; },
      [] { sketch1_teardown_called++; });
  auto sketch2 = new sketchinsketch::OneOffFunctionalSketch(
      [] { sketch2_setup_called++; });
  sketchinsketch::SketchTimeShare sketch;
  sketch.addSketch(sketch1);
  sketch.addSketch(sketch2);

  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::INIT, sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::INIT, sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::INIT, sketch2->getStatus());
  sketch.begin();
  TEST_ASSERT_EQUAL(1, sketch1_setup_called);
  TEST_ASSERT_EQUAL(1, sketch2_setup_called);
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::RUNNING, sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::RUNNING,
                    sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATING,
                    sketch2->getStatus());
  sketch.run();
  TEST_ASSERT_EQUAL(1, sketch1_loop_called);
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::RUNNING, sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::RUNNING,
                    sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATING,
                    sketch2->getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::RUNNING, sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::RUNNING,
                    sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATED,
                    sketch2->getStatus());
  sketch.terminate();
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATING,
                    sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATING,
                    sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATED,
                    sketch2->getStatus());
  sketch.next();
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATED,
                    sketch.getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATED,
                    sketch1->getStatus());
  TEST_ASSERT_EQUAL(sketchinsketch::SketchStatus::TERMINATED,
                    sketch2->getStatus());
}

#ifdef ARDUINO
void setup() {
  delay(2000); // add 2-sec wait for the board w/o software resetting via
               // Serial.DTR/RTS
#else
int main(int argc, char *argv[]) {
#endif
  UNITY_BEGIN();
  RUN_TEST(test_sketch_lifecycle);
  RUN_TEST(test_functional_sketch_lifecycle);
  RUN_TEST(test_oneoff_functional_sketch_lifecycle);
  RUN_TEST(test_sketch_switch_lifecycle);
  RUN_TEST(test_sketch_timeshare_lifecycle);
  UNITY_END();

#ifndef ARDUINO
  return 0;
#endif
}

#ifdef ARDUINO
void loop() {}
#endif
