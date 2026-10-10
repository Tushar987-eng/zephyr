/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	/* TODO(l8-task1): Push a value, call rb_init(4) again, then
	 * verify the buffer is empty and count is 0.
	 * See TEST_SPEC.md "Suite ring_buf_init" #2.
	 */
	zassert_ok(rb_push(999),"Pushing a value in buffer failed");
	zassert_ok(rb_init(4), "Initializing the buffer failed");
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	/* TODO(l8-task1): rb_push(42), rb_pop(&v) -> v == 42, buffer empty after.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #1.
	 */
	int v = 0;
	zassert_ok(rb_push(42), "Pushing 42 in buffer failed");
	zassert_ok(rb_pop(&v), "Popping a value from buffer failed");
	zassert_equal(v,42, "Popped value was expected to be 42");
	zassert_true(rb_is_empty(), "Buffer was expected to be empty");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	/* TODO(l8-task1): rb_push(1), rb_push(2), rb_push(3); pop three times
	 * and verify the values come out as 1, 2, 3 in that order.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #2.
	 */
	int value = 0;
	zassert_ok(rb_push(1), "Pushing 1 to buffer failed");
	zassert_ok(rb_push(2), "Pushing 2 to buffer failed");
	zassert_ok(rb_push(3), "Pushing 3 to buffer failed");
	zassert_ok(rb_pop(&value), "Popping a value from buffer failed");
	zassert_equal(value, 1, "Expected popped value was 1");
	zassert_ok(rb_pop(&value), "Popping a value from buffer failed");
	zassert_equal(value, 2, "Expected popped value was 2");
	zassert_ok(rb_pop(&value), "Popping a value from buffer failed");
	zassert_equal(value, 3, "Expected popped value was 3");
	zassert_true(rb_is_empty(), "Buffer was expected to be empty");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	/* TODO(l8-task1): Fill the buffer to its capacity of 4, then push
	 * one more value -> -ENOSPC.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #3.
	 */
	zassert_ok(rb_push(11), "Pushing 11 to buffer failed");
	zassert_ok(rb_push(22), "Pushing 22 to buffer failed");
	zassert_ok(rb_push(33), "Pushing 33 to buffer failed");
	zassert_ok(rb_push(44), "Pushing 44 to buffer failed");
	zassert_true(rb_is_full(), "Buffer was expected to be full");
	zassert_equal(rb_push(99), -ENOSPC, "Pushed a value in an already full buffer");
	zassert_equal(rb_count(), 4, "Expected count was 4");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	/* TODO(l8-task1): rb_push(7); rb_peek(&v) -> v == 7; rb_peek(&v) again
	 * -> v == 7; rb_count() still == 1.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #1.
	 */
	int value = 0;
	zassert_ok(rb_push(7), "Pushing 7 to the buffer failed");
	zassert_ok(rb_peek(&value), "Peeking the buffer failed");
	zassert_equal(value, 7, "Expected value was 7");
	zassert_ok(rb_peek(&value), "Peeking the buffer failed");
	zassert_equal(value, 7, "Expected value was 7");
	zassert_equal(rb_count(), 1, "Expected count was 1");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	/* TODO(l8-task1): rb_pop(NULL) -> -EINVAL.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #2.
	 */
	zassert_equal(rb_pop(NULL), -EINVAL, "pop(NULL) should return -EINVAL");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	/* TODO(l8-task1): push 4 values -> rb_is_full() == true, rb_count() == 4.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #3.
	 */
	zassert_ok(rb_push(11), "Pushing 11 to buffer failed");
	zassert_ok(rb_push(22), "Pushing 22 to buffer failed");
	zassert_ok(rb_push(33), "Pushing 33 to buffer failed");
	zassert_ok(rb_push(44), "Pushing 44 to buffer failed");
	zassert_true(rb_is_full(), "Buffer was expected to be full");
	zassert_equal(rb_count(), 4, "Buffer was expected to have 4 elements");
}
