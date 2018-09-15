#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>

#define EXIT_FAIL() { printf("Test failed"); exit(EXIT_FAILURE); }
#define EXIT_PASS() { printf("Test passed"); exit(EXIT_SUCCESS); }

#ifdef __cplusplus
#define TEST_REQUIRE_PASS(condition) { try { if (!(condition)) EXIT_FAIL(); } catch (...) { EXIT_FAIL(); } }
#define TEST_REQUIRE_FAIL(condition) { try { if ((condition)) EXIT_FAIL(); } catch (...) { } }
#else
#define TEST_REQUIRE_PASS(condition) { if (!(condition)) EXIT_FAIL(); }
#define TEST_REQUIRE_FAIL(condition) { if ((condition)) EXIT_FAIL(); }
#endif

#ifdef __cplusplus
}
#endif