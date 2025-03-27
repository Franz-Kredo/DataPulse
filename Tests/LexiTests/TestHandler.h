#ifndef TEST_HANDLER_H
#define TEST_HANDLER_H

#include "../../Models/FileModel.h"
#include "../../LogicLayer/FileLogic.h"
#include <filesystem>
#include "../../Models/CommandModel.h"

class TestHandler {
public:
    TestHandler() = default;
    void test_local_file_ops();
    void test_network_connection();

private:
    //void test_remote_file_read(SftpSessionModel *sftpSessionModel);
    void test_remote_file_read(SftpSessionModel *sftpSessionModel, CommandModel *commandModel); 
    void test_remote_file_write(SftpSessionModel* sftpSessionModel, CommandModel* commandModel);
};

#endif // TEST_HANDLER_H

