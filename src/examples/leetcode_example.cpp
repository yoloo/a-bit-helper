#include "leetcode/leetcode.h"

#include <iostream>

// 1.
namespace Example01 {
    void test1() {
        std::vector<int> result = LeetCode::Array::TwoSum::one({2, 7, 11, 15}, 9);

        for (const auto& item : result)
            std::cout << item << std::endl;
    }
}

auto initLinkedList = [](int len) -> LeetCode::LinkedList::ListNode* {
    if (!len)
        return nullptr;

    int i = 1;
    auto head = new LeetCode::LinkedList::ListNode(i);

    auto cur = head;
    while (i < len)
    {
        cur->next = new LeetCode::LinkedList::ListNode(++i);
        cur       = cur->next;
    }

    return head;
};

auto dumpLinkedList = [](const LeetCode::LinkedList::ListNode* head) {
    if (nullptr == head || nullptr == head->next)
        return;

    auto cur = head;
    while (cur != nullptr) {
        std::cout << "Node: " << cur->val << std::endl;
        cur = cur->next;
    }

    std::cout << std::endl;
};

// 面试题-24
namespace ExampleInterview24 {
    void test1() {
        auto head = initLinkedList(10);
        dumpLinkedList(head);

        dumpLinkedList(LeetCode::LinkedList::ReverseLinkedList::one(head));
    }

    void test2() {
        auto head = initLinkedList(10);
        dumpLinkedList(head);

        dumpLinkedList(LeetCode::LinkedList::ReverseLinkedList::two(head));
    }

    void test3() {
        auto head = initLinkedList(3);
        dumpLinkedList(head);

        dumpLinkedList(LeetCode::LinkedList::ReverseLinkedList::three(head));
    }
}

// 24
namespace Example24 {
    void test1() {
        auto head = initLinkedList(5);
        dumpLinkedList(head);

        dumpLinkedList(LeetCode::LinkedList::SwapPairsLinkedList::one(head));
    }

    void test2() {
        auto head = initLinkedList(3);
        dumpLinkedList(head);

        dumpLinkedList(LeetCode::LinkedList::SwapPairsLinkedList::two(head));
    }

}

int main() {
    //Example01::test1();

    //ExampleInterview24::test1();
    //ExampleInterview24::test2);
    //ExampleInterview24::test3();

    Example24::test1();
    Example24::test2();

    return 0;
}