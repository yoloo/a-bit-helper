#include "msg.h"

namespace AsioNet {
    std::string MsgHead::dump() {
        std::ostringstream oss;
        auto bin = (uint8_t*)this;
        for (size_t i = 0; i < sizeof(MsgHead); ++ i)
            oss << (int32_t)bin[i] << ",";

        oss << " | size=" << size
            << " zip=" << zip
            << " encrypt=" << encrypt
            << " checksum=" << checksum
            << " reserved=" << reserved;

        return oss.str();
    }

    uint8_t MsgType::ID_1(uint16_t id) {
        MsgType tp;
        tp.id = id;
        return tp.id_1;
    }

    uint8_t MsgType::ID_2(uint16_t id) {
        MsgType tp;
        tp.id = id;
        return tp.id_2;
    }

    uint16_t MsgType::ID(uint8_t fir, uint8_t sec) {
        MsgType type;
        type.id_1 = fir;
        type.id_2 = sec;
        return type.id;
    }
}