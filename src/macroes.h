#pragma once

#define __CONCAT__(a, b) a##b
#define CONCAT(a, b) __CONCAT__(a, b)
#define PAD(n) char CONCAT(pad, __LINE__)[n] = {};