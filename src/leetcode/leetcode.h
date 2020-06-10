//
// Created by zhaoyongle on 2020/5/19.
//

#ifndef A_BIT_HELPER_LEET_CODE_H
#define A_BIT_HELPER_LEET_CODE_H

#include <vector>
#include <string>

namespace LeetCode
{
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
        struct ListNode {
            int val;
            ListNode* next;

            ListNode(int x) : val(x), next(nullptr) {}
        };

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
        struct StackVector {
        public:
            bool empty() { return !idx_; }

            size_t size() const { return idx_; }

            char at(size_t i) const {
                if (i > idx_)
                    return '\0';

                return inner_[i];
            }

            bool equal(const StackVector& s) {
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

        private:
            size_t idx_ = 0;
            std::vector<char> inner_{'\0'};
        };

        //844. 比较包含退格符的字符串
        namespace BackSpaceCompare {
            bool one(std::string S, std::string T);
        }
    }
};

#endif //A_BIT_HELPER_LEET_CODE_H
