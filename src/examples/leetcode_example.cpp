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

auto initLinkedList = [](int len) -> LeetCode::ListNode* {
    if (!len)
        return nullptr;

    int i = 1;
    auto head = new LeetCode::ListNode(i);

    auto cur = head;
    while (i < len)
    {
        cur->next = new LeetCode::ListNode(++i);
        cur       = cur->next;
    }

    return head;
};

auto dumpLinkedList = [](const LeetCode::ListNode* head) {
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

auto initCycleLinkedList = [](const std::initializer_list<int>& vals, int pos) -> LeetCode::ListNode* {
    if (!vals.size())
        return nullptr;

    LeetCode::ListNode* head  = nullptr;
    LeetCode::ListNode* next  = nullptr;

    LeetCode::ListNode* cycle = nullptr;

    int idx = 0;
    LeetCode::ListNode* cur = head;
    for (const auto& item : vals) {
        next = new LeetCode::ListNode(item);

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

// 142
namespace Example142 {
    void test1() {
        auto head = initCycleLinkedList({3, 2, 0, -4}, 1);
        auto cycle = LeetCode::LinkedList::DetectCycleLinkedList::one(head);

        if (cycle != nullptr)
            std::cout << "cycle node val: " << cycle->val << std::endl;
    }
}

// 848
namespace Example848 {
    void test1() {
        {
            std::string S("ab#c");
            std::string T("ad#c");

            std::cout << S << "<=>" << T << " " << (LeetCode::Stack::BackSpaceCompare::one(S, T) ? "True" : "False") << std::endl;
        }

        {
            std::string S("ab##");
            std::string T("c#d#");

            std::cout << S << "<=>" << T << " " << (LeetCode::Stack::BackSpaceCompare::one(S, T) ? "True" : "False") << std::endl;
        }

        {
            std::string S("a##c");
            std::string T("#a#c");

            std::cout << S << "<=>" << T << " " << (LeetCode::Stack::BackSpaceCompare::one(S, T) ? "True" : "False") << std::endl;
        }

        {
            std::string S("a#c");
            std::string T("b");

            std::cout << S << "<=>" << T << " " << (LeetCode::Stack::BackSpaceCompare::one(S, T) ? "True" : "False") << std::endl;
        }
    }
}

// 232
namespace Example232 {
    void test1() {
        LeetCode::MyStackVectorT<char> st1;

        st1.push('a');
        st1.push('b');

        std::cout << "peek=" << st1.peek() << std::endl;
        std::cout << "pop=" << st1.pop() << std::endl;
        std::cout << "pop=" << st1.pop() << std::endl;

        std::cout << "stack-size:" << st1.size() << std::endl;
        std::cout << "stack-empty:" << st1.empty() << std::endl;

        std::cout << "pop=" << st1.pop() << std::endl;
        std::cout << "peek=" << st1.peek() << std::endl;

        st1.push('c');
        st1.push('d');
        st1.push('e');
        std::cout << "stack-size:" << st1.size() << std::endl;
        std::cout << "stack-empty:" << st1.empty() << std::endl;
        std::cout << "peek=" << st1.peek() << std::endl;
        std::cout << "pop=" << st1.pop() << std::endl;
        std::cout << "pop=" << st1.pop() << std::endl;
        std::cout << "stack-size:" << st1.size() << std::endl;
        std::cout << "stack-empty:" << st1.empty() << std::endl;
    }

    void test2() {
        LeetCode::Stack::Stack2Queue::MyQueue myqueue;
        myqueue.push(1);
        myqueue.push(2);
        std::cout << myqueue.peek() << std::endl;
        std::cout << myqueue.pop() << std::endl;
        std::cout << myqueue.empty() << std::endl;
    }
}

int main() {
    //Example01::test1();

    //ExampleInterview24::test1();
    //ExampleInterview24::test2);
    //ExampleInterview24::test3();

    //Example24::test1();
    //Example24::test2();

    //Example141::test1();
    //Example141::test2();
    //Example141::test3();
    //Example141::test4();

    //Example142::test1();

    //Example848::test1();

    //Example232::test1();
    Example232::test2();

    return 0;
}