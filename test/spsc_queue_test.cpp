#include <gtest/gtest.h>

#include "spsc_queue.h"

TEST(SPSC, SizePrimitive) {
    int buf[16];
    rutils::spsc_queue<int, 16> test_queue(buf);
    ASSERT_EQ(test_queue.size(), sizeof(buf));
}

TEST(SPSC, SizeStruct) {
    struct TestStruct {
        int x;
        char y;
    } test_struct[128];
    rutils::spsc_queue<TestStruct, 128> test_queue(test_struct);
    ASSERT_EQ(test_queue.size(), sizeof(test_struct));
}

TEST(SPSC, EnqueueAndDequeuePrimitive) {
    int buf[1024];
    rutils::spsc_queue<int, 1024> test_queue(buf);
    int enqueue_ct = 0;
    int dequeue_ct = 0;
    // enqueue 1024 items
    for (; enqueue_ct < 1024; enqueue_ct++) {
        test_queue.enqueue(enqueue_ct);
    }
    ASSERT_EQ(test_queue.full(), true);
    // dequeue 512 items
    for (; dequeue_ct < 512; dequeue_ct++) {
        int x;
        test_queue.dequeue(x);
        ASSERT_EQ(x, dequeue_ct);
    }
    ASSERT_EQ(test_queue.item_count(), 512);
    for (; enqueue_ct < 1536; enqueue_ct++) {
        test_queue.enqueue(enqueue_ct);
    }
    ASSERT_EQ(test_queue.full(), true);
    for (; dequeue_ct < 1536; dequeue_ct++) {
        int x;
        test_queue.dequeue(x);
        ASSERT_EQ(x, dequeue_ct);
    }
    ASSERT_EQ(test_queue.empty(), true);
}
