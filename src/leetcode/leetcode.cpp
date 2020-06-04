//
// Created by zhaoyongle on 2020/5/19.
//

#include "leetcode.h"

#include <set>
#include <unordered_map>

namespace LeetCode {
    namespace Array {
        // 1. 两数之和
        namespace TwoSum {
            // 1. 哈希映射
            std::vector<int> one(const std::vector<int> &nums, int target) {
                std::unordered_map<int, int> lefts;
                for (int i = 0; i < nums.size(); ++i) {
                    int sub = target - nums[i];
                    if (lefts.find(sub) != lefts.end())
                        return {lefts[sub], i};

                    lefts[nums[i]] = i;
                }

                return {};
            }

            // 2. 双层循环
            std::vector<int> two(const std::vector<int> &nums, int target) {
                int len = nums.size();
                for (int i = 0; i < len; ++i) {
                    for (int j = i + 1; j < len; ++j) {
                        if (nums[i] + nums[j] == target)
                            return {i, j};
                    }
                }

                return {};
            }
        }
    }

    namespace LinkedList {
        // 206. 反转链表
        namespace ReverseLinkedList {
            //
            // 1. 双指针迭代
            //
            ListNode* one(ListNode* head) {
                if (nullptr == head || nullptr == head->next)
                    return head;

                ListNode* prev = nullptr;
                auto cur       = head;
                while (cur != nullptr) {
                    // 1. 当前节点的next节点为新的头节点
                    head = cur->next;

                    // 2. 当前节点与前置节点反转
                    cur->next = prev;
                    prev = cur;

                    // 3. 当前节点指向已保存新的头节点
                    cur = head;
                }

                return prev;
            }

            //
            // 2. 递归调用
            //
            ListNode* two(ListNode* head) {
                if (nullptr == head || nullptr == head->next)
                    return head;

                auto cur = two(head->next);
                head->next->next = head;
                head->next       = nullptr;

                return cur;
            }

            //
            // 3. 交换数值
            //
            ListNode* three(ListNode* head) {
                if (nullptr == head || nullptr == head->next)
                    return head;

                std::vector<ListNode*> addrs;

                auto cur = head;
                while (cur != nullptr) {
                    addrs.push_back(cur);

                    cur = cur->next;
                }

                size_t half = addrs.size() / 2;
                for (size_t i = 0; i < half; ++i) {
                    std::swap(addrs.at(i)->val, addrs.at(addrs.size() - 1 - i)->val);
                }

                return head;
            }
        }

        // 24. 交换链表相邻节点
        namespace SwapPairsLinkedList {
            // 1. 两两交换值
            ListNode* one(ListNode* head) {
                if (nullptr == head || nullptr == head->next)
                    return head;

                auto cur  = head;
                while (cur != nullptr && cur->next != nullptr) {
                    std::swap(cur->val, cur->next->val);

                    cur  = cur->next->next;
                }

                return head;
            }

            // 2. 递归调整
            ListNode* two(ListNode* head) {
                if (nullptr == head || nullptr == head->next)
                    return head;

                auto next = head->next;
                head->next = two(next->next);
                next->next = head;

                return next;
            }
        }

        // 141. 判断链表是否有环
        namespace HasCycleLinkedList {
            // 1. 哈希集合
            bool one(ListNode* head) {
                if (nullptr == head || nullptr == head->next)
                    return false;

                std::set<ListNode*> olds;
                while (head != nullptr) {
                    if (olds.count(head))
                        return true;

                    olds.insert(head);

                    head = head->next;
                }

                return false;
            }

            // 2. 快慢指针
            bool two(ListNode* head) {
                if (nullptr == head || nullptr == head->next)
                    return false;

                auto slow = head;
                auto fast = slow;
                while (fast != nullptr && fast->next != nullptr) {
                    slow = slow->next;
                    fast = fast->next;

                    if (slow == fast)
                        return true;
                }

                return false;
            }
        }

        // 142. 返回链表入环节点
        namespace DetectCycleLinkedList {
            // 1. 哈希集合
            ListNode* one(ListNode* head) {
                if (nullptr == head || nullptr == head->next)
                    return nullptr;

                std::set<ListNode*> lefts;

                while (head != nullptr) {
                    if (lefts.count(head))
                        return head;

                    lefts.insert(head);

                    head = head->next;
                }

                return nullptr;
            }
        }
    }

}