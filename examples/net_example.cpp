#include "iostream"

#include "net/msg.h"

int main() {
    // 1. msg-head
    AsioNet::MsgHead head{};
    head.size     = 220;
    head.zip      = 1;
    head.encrypt  = 0;
    head.checksum = 1;
    head.reserved = 0;

    std::cout << head.dump() << std::endl;
    std::cout << "body-size=" << head.body_size() << std::endl;
    std::cout << "total-size=" << head.total_size() << std::endl;

    // 2. msg-id
    auto type = AsioNet::MsgType::ID(2, 71);
    std::cout << "msg-type=" << type << std::endl;
    std::cout << "msg-id1=" << +AsioNet::MsgType::ID_1(type) << std::endl;
    std::cout << "msg-id2=" << +AsioNet::MsgType::ID_2(type) << std::endl;

    return 0;
}