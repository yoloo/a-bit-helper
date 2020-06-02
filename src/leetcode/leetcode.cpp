//
// Created by zhaoyongle on 2020/5/19.
//

#include "leetcode.h"

#include <set>
#include <unordered_map>

namespace LeetCode {
    namespace Array {
        namespace TwoSum {
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

        namespace SwapPairsLinkedList {
            ListNode* one(ListNode* head) {
                if (nullptr == head || nullptr == head->next)
                    return head;

                auto cur  = head;
                auto next = cur->next;
                while (cur != nullptr && next != nullptr) {
                    std::swap(cur->val, next->val);

                    cur  = next->next;
                    if (cur != nullptr)
                        next = cur->next;
                }

                return head;
            }

            ListNode* two(ListNode* head) {
                if (nullptr == head || nullptr == head->next)
                    return head;

                auto next = head->next;
                next->next = two(next->next);

                head->next = next->next;
                next->next = head;

                return next;
            }
        }

        namespace HasCycleLinkedList {
            bool one(ListNode* head) {
                if (nullptr == head || nullptr == head->next)
                    return false;

                std::set<ListNode*> addrs;
                auto cur = head;
                while (cur != nullptr) {
                    if (addrs.count(cur))
                        return true;

                    addrs.insert(cur);

                    cur = cur->next;
                }

                return false;
            }

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

        namespace DetectCycleLinkedList {
            ListNode* one(ListNode* head) {
                if (nullptr == head || nullptr == head->next)
                    return nullptr;

                std::set<ListNode*> lefts;

                auto cur = head;
                while (cur != nullptr) {
                    if (lefts.count(cur))
                        return cur;

                    lefts.insert(cur);

                    cur = cur->next;
                }

                return nullptr;
            }
        }
    }

}