#ifndef CAPTURE_GROUP_H
#define CAPTURE_GROUP_H

#include <vector>
#include <string>

class CaptureGroup {
public:
    std::vector<std::string> get_capture_groups();
private:
    std::vector<std::string> captures;
};

#endif
