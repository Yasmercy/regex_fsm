#ifndef RESULT_H
#define RESULT_H

#include <vector>
#include "capture_group.h"

struct Result {
    // container for the result of a regex matching
    std::vector<CaptureGroup> captures;
    bool matched;
};

#endif
