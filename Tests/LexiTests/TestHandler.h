#ifndef TEST_HANDLER_H
#define TEST_HANDLER_H

#include "../../Models/FileModel.h"
#include "../../LogicLayer/FileLogic.h"
#include <filesystem>

class TestHandler {
public:
    TestHandler() = default;
    void test_local_file_ops();
};

#endif // TEST_HANDLER_H

