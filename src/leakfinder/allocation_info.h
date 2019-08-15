#ifndef __ALLOCATION_INFO_H__
#define __ALLOCATION_INFO_H__

#include <vector>
#include <string>

#include <pthread.h>

namespace bornander
{
    namespace memory
    {
        class allocation_info
        {
        public:
            typedef long long address_type;

        private:
            allocation_info::address_type address;
            size_t size;
            std::vector<std::string> stacktrace;
            pthread_t thread_id;
        public:
            allocation_info(void* address, size_t size, char** stacktrace, size_t depth, pthread_t thread_id);

            allocation_info::address_type get_address() const;
            size_t get_size() const;
            std::vector<std::string> get_stacktrace() const;
            pthread_t get_thread_id() const;
        };
    }
}

#endif//__ALLOCATION_INFO_H__