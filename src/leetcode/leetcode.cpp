//
// Created by zhaoyongle on 2020/5/19.
//

#include "leetcode.h"

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
            ListNode* one(ListNode* head) {
                if (nullptr == head || nullptr == head->next)
                    return head;

                ListNode* prev = nullptr;
                ListNode* cur  = head;
                while (cur != nullptr) {
                    auto next = cur->next;
                    cur->next = prev;
                    prev = cur;

                    cur = next;
                }

                return prev;
            }

            ListNode* two(ListNode* head) {
                if (nullptr == head || nullptr == head->next)
                    return head;

                auto cur = two(head->next);
                head->next->next = head;
                head->next       = nullptr;

                return cur;
            }

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
    }

}