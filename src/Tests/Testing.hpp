#include <cstdlib>
#include <cstdio>

#define EXIT_FAIL() { printf("Test failed"); exit(EXIT_FAILURE); }
#define EXIT_PASS() { printf("Test passed"); exit(EXIT_SUCCESS); }

#define TEST_REQUIRE_PASS(condition) { try { if (!(condition)) EXIT_FAIL(); } catch (...) { EXIT_FAIL(); } }
#define TEST_REQUIRE_FAIL(condition) { try { if ((condition)) EXIT_FAIL(); } catch (...) { } }
