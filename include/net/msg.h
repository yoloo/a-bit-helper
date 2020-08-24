#ifndef __NET_H__
#define __NET_H__

#include <sstream>

namespace AsioNet {
#pragma pack(1)
    // 1. msg head
    struct MsgHead
    {
        uint32_t size:24;    // body size
        uint32_t zip:1;      // is zipped
        uint32_t encrypt:1;  // is encrypted
        uint32_t checksum:1; // checksum
        uint32_t reserved:5; // reserved

        size_t total_size() const { return sizeof(MsgHead) + size; }

        size_t body_size() const { return size; }

        std::string dump();
    };

    // 2. msg type
    struct MsgType {
        static uint8_t ID_1(uint16_t id);
        static uint8_t ID_2(uint16_t id);
        static uint16_t ID(uint8_t fir, uint8_t sec);

        union {
            struct {
                uint8_t id_1;
                uint8_t id_2;
            };

            uint16_t id = 0;
        };
    };

#pragma pack()

}

#endif//__NET_H__