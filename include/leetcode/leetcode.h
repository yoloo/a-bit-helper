//
// Created by zhaoyongle on 2020/5/19.
//

#ifndef A_BIT_HELPER_LEET_CODE_H
#define A_BIT_HELPER_LEET_CODE_H

#include <vector>
#include <string>

namespace LeetCode
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //
    // 单链表节点
    //
    struct ListNode {
        int val;
        ListNode* next;

        ListNode(int x) : val(x), next(nullptr) {}
    };

    //
    // 字符栈
    //
    struct MyStackVectorChar {
    public:
        bool empty() { return !idx_; }

        size_t size() const { return idx_; }

        char at(size_t i) const {
            if (i > idx_)
                return '\0';

            return inner_[i];
        }

        bool equal(const MyStackVectorChar& s) {
            if (size() != s.size())
                return false;

            for (int i = 0; i <= idx_; ++i) {
                if (at(i) != s.at(i))
                    return false;
            }

            return true;
        }

        void push(char c) {
            ++idx_;
            if (idx_ <= inner_.size())
                inner_.push_back('\0');

            inner_[idx_] = c;
        }

        char pop() {
            if (idx_)
            {
                --idx_;
                return inner_.at(idx_ + 1);
            }

            return '\0';
        }

        char peek() {
            if (idx_) {
                return inner_.at(idx_);
            }

            return '\0';
        }

    private:
        size_t idx_ = 0;
        std::vector<char> inner_{'\0'};
    };

    //
    // 模板栈
    //
    template <typename T>
    class MyStackVectorT {
    public:
        MyStackVectorT() {
            idx_++;
            inner_.push_back(T{});
        }

    public:
        bool empty() {
            return !size();
        }

        void push(T t) {
            if (idx_ >= inner_.size()) {
                idx_++;
                inner_.push_back(t);
                return;
            }

            inner_[idx_++] = t;
        }

        T pop() {
            if (idx_ <= 1)
                return T{};

            return inner_[--idx_];
        }

        T peek() {
            if (idx_ <= 1)
                return T{};

            return inner_[idx_ - 1];
        }

    public:
        T at(size_t i) const {
            if (i <= 0 || i >= idx_)
                return T{};

            inner_[i];
        }

        size_t size() const {
            return idx_ - 1;
        }

        bool equal(const MyStackVectorT<T>& s) {
            if (size() != s.size())
                return false;

            for (int i = 1; i < idx_; ++i) {
                if (at(i) != s.at(i))
                    return false;
            }

            return true;
        }

    private:
        size_t idx_ = 0;
        std::vector<T> inner_;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // 数组
    namespace Array
    {
        //1. 两数之和
        namespace TwoSum {
            //a.一次哈希
            std::vector<int> one(const std::vector<int>& nums, int target);

            //b.暴力破解
            std::vector<int> two(const std::vector<int>& nums, int target);
        }
    };

    // 链表
    namespace LinkedList {

        //206. 反转链表
        namespace ReverseLinkedList {
            // a.双指针迭代
            ListNode* one(ListNode* head);

            // b.递归实现
            ListNode* two(ListNode* head);

            // c.暴力破解
            ListNode* three(ListNode* head);
        }

        //24. 两两交换链表相邻节点
        namespace SwapPairsLinkedList {
            // a.双指针迭代(相邻节点交换val)
            ListNode* one(ListNode* head);

            // b.双指针迭代(重构next)
            ListNode* two(ListNode* head);
        }

        //141. 判断链表是否有环
        namespace HasCycleLinkedList {
            // a.哈希表
            bool one(ListNode* head);

            // b.快慢指针
            bool two(ListNode* head);
        }

        //142. 返回链表入环节点
        namespace DetectCycleLinkedList {
            // a.哈希表
            ListNode* one(ListNode* head);
        }
    }

    // 堆栈
    namespace Stack {

        //844. 比较包含退格符的字符串
        namespace BackSpaceCompare {
            bool one(std::string S, std::string T);
        }

        //20. 判断字符串是否合法('(', ')', '[', ']', '{', '}')
        namespace IsValidString {
            bool one(std::string s);
        }

        //232. 用栈实现队列
        namespace Stack2Queue {

            class MyQueue {
            public:
                /** Initialize your data structure here. */
                MyQueue() {

                }

                /** Push element x to the back of queue. */
                void push(int x) {
                    in_.push(x);
                }

                /** Removes the element from in front of queue and returns that element. */
                int pop() {
                    if (!out_.empty())
                        return out_.pop();

                    while (!in_.empty()) {
                        out_.push(in_.pop());
                    }

                    return out_.pop();
                }

                /** Get the front element. */
                int peek() {
                    if (!out_.empty())
                        return out_.peek();

                    while (!in_.empty()) {
                        out_.push(in_.pop());
                    }

                    return out_.peek();
                }

                /** Returns whether the queue is empty. */
                bool empty() {
                    return in_.empty() && out_.empty();
                }

            private:
                MyStackVectorT<int> in_;
                MyStackVectorT<int> out_;
            };
        }
    }
};

#endif //A_BIT_HELPER_LEET_CODE_H
