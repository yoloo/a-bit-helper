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

//////////////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////////////

auto initCycleLinkedList = [](const std::initializer_list<int>& vals, int pos) -> LeetCode::LinkedList::ListNode* {
    if (!vals.size())
        return nullptr;

    LeetCode::LinkedList::ListNode* head  = nullptr;
    LeetCode::LinkedList::ListNode* next  = nullptr;

    LeetCode::LinkedList::ListNode* cycle = nullptr;

    int idx = 0;
    LeetCode::LinkedList::ListNode* cur = head;
    for (const auto& item : vals) {
        next = new LeetCode::LinkedList::ListNode(item);

        if (pos == idx) {
            cycle = next;
        }
        ++idx;

        if (head != nullptr) {
            cur->next = next;
            cur = cur->next;
        }
        else {
            cur = head = next;
            cur->next  = nullptr;
        }
    }

    if (next != nullptr && cycle != nullptr) {
        next->next = cycle;
    }

    return head;
};

//////////////////////////////////////////////////////////////////////////////////////////////////

// 141
namespace Example141 {
    void test1() {
        bool hasCycle = LeetCode::LinkedList::HasCycleLinkedList::one(initCycleLinkedList({3, 2, 0, -4}, 1));
        if (hasCycle)
            std::cout << "有环" << std::endl;
        else
            std::cout << "无环" << std::endl;
    }

    void test2() {
        bool hasCycle = LeetCode::LinkedList::HasCycleLinkedList::one(initCycleLinkedList({3, 2, 0, -4}, -1));
        if (hasCycle)
            std::cout << "有环" << std::endl;
        else
            std::cout << "无环" << std::endl;
    }

    void test3() {
        bool hasCycle = LeetCode::LinkedList::HasCycleLinkedList::two(initCycleLinkedList({3, 2, 0, -4}, 1));
        if (hasCycle)
            std::cout << "有环" << std::endl;
        else
            std::cout << "无环" << std::endl;
    }

    void test4() {
        bool hasCycle = LeetCode::LinkedList::HasCycleLinkedList::two(initCycleLinkedList({3, 2, 0, -4}, -1));
        if (hasCycle)
            std::cout << "有环" << std::endl;
        else
            std::cout << "无环" << std::endl;
    }
}

int main() {
    //Example01::test1();

    //ExampleInterview24::test1();
    //ExampleInterview24::test2);
    //ExampleInterview24::test3();

    //Example24::test1();
    //Example24::test2();

    Example141::test1();
    Example141::test2();
    Example141::test3();
    Example141::test4();

    return 0;
}