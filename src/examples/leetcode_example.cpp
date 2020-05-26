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
    auto initLinkedList = []() -> LeetCode::LinkedList::ReverseLinkedList::ListNode* {
        int i = 1;
        auto head = new LeetCode::LinkedList::ReverseLinkedList::ListNode(i);

        auto cur = head;
        do {
            auto node = new LeetCode::LinkedList::ReverseLinkedList::ListNode(++i);
            cur->next = node;
            cur = cur->next;
        } while (i < 5);

        return head;
    };

    auto dumpLinkedList = [](const LeetCode::LinkedList::ReverseLinkedList::ListNode* head) {
        if (nullptr == head || nullptr == head->next)
            return;

        auto cur = head;
        while (cur != nullptr) {
            std::cout << "Node: " << cur->val << std::endl;
            cur = cur->next;
        }
    };

    void test1() {
        auto head = initLinkedList();
        dumpLinkedList(head);

        dumpLinkedList(LeetCode::LinkedList::ReverseLinkedList::one(head));
    }

    void test2() {
        auto head = initLinkedList();
        dumpLinkedList(head);

        dumpLinkedList(LeetCode::LinkedList::ReverseLinkedList::two(head));
    }
}

int main() {
    //Example01::test1();

    Example24::test1();
    Example24::test2();

    return 0;
}