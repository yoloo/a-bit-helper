#include "leetcode/leetcode.h"

#include <iostream>

namespace Example01 {
    void test1() {
        std::vector<int> result = LeetCode::Array::TwoSum::one({2, 7, 11, 15}, 9);

        for (const auto& item : result)
            std::cout << item << std::endl;
    }
}

namespace Example24 {
    void test1() {
        int i = 0;
        auto head = new LeetCode::LinkedList::ReverseLinkedList::ListNode(i + 1);
        ++i;

        auto cur = head;
        do {
            auto node = new LeetCode::LinkedList::ReverseLinkedList::ListNode(i + 1);
            cur->next = node;
            cur = cur->next;

            ++i;
        } while (i < 5);

        cur = head;
        while (cur != nullptr) {
            std::cout << "before node: " << cur->val << std::endl;

            cur = cur->next;
        }

        auto another = LeetCode::LinkedList::ReverseLinkedList::one(head);
        cur = another;
        while (cur != nullptr) {
            std::cout << "after node: " << cur->val << std::endl;

            cur = cur->next;
        }
    }
}

int main() {
    //Example01::test1();
    Example24::test1();

    return 0;
}