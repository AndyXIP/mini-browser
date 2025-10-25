#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <string>
#include <cstdlib>


#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__ << " - " << message << "\n"; \
            std::exit(1); \
        } \
    } while (0)

#define ASSERT_EQ(expected, actual, message) \
    do { \
        if ((expected) != (actual)) { \
            std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__ << " - " << message << "\n"; \
            std::cerr << "  Expected: " << (expected) << "\n"; \
            std::cerr << "  Actual:   " << (actual) << "\n"; \
            std::exit(1); \
        } \
    } while (0)

#define TEST(test_name) \
    void test_name(); \
    static struct test_name##_runner { \
        test_name##_runner() { \
            std::cout << "Running " << #test_name << "...\n"; \
            test_name(); \
            std::cout << "  PASS\n"; \
        } \
    } test_name##_instance; \
    void test_name()

#endif
