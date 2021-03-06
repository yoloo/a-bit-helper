# 前言

刚开始接触递归时，多半是一脸懵逼。看着寥寥几行的递归调用，在大脑中展开时却是异常痛苦，顿觉脑容量不够。
为什么总是能看得懂，却写不出来呢？原因是：没方法，练得少！
任何事情都有一定套路，递归也不例外。掌握科学的方法，加之适当练习，最终一定会收获很多！

本文主要从以下几个方面讲递归：
1. 什么是递归？
2. 递归解题思路
3. 实战演练剖析

---

## 什么是递归？

简单的说，就是函数自己调用了自己。以斐波那契函数factorial()为例，factorial中存在着对factorial(n-1)调用，所以此函数是递归函数。

```c++
int factorial(int n) {
    if (n <= 1) {
        return 1; 
    }

    return n * factorial(n - 1);
}
```

进一步剖析，[递归]有递有归，先有递再有归。
* [递] 将问题拆解成子问题来解决，子问题再拆解成子子问题，...，直到被拆解的问题无需拆解，即可直接求解。
* [归] 最小的问题解决了，那么它的上一层子问题也就解决了，...，直到最开始的问题解决。
更直观的，其过程如下图所示：

```c++
f(6)
=> 6 * f(5)
=> 6 * (5 * f(4))
=> 6 * (5 * (4 * f(3)))
=> 6 * (5 * (4 * (3 * f(2))))
=> 6 * (5 * (4 * (3 * (2 * f(1)))))
=> 6 * (5 * (4 * (3 * (2 * 1))))
=> 6 * (5 * (4 * (3 * 2)))
=> 6 * (5 * (4 * 6))
=> 6 * (5 * 24)
=> 6 * 120
=> 720
```

求解问题f(6)，由于f(6)=6\*f(5)，所以f(6)拆解成对f(5)问题求解。同理，f(5)=5\*f(4)，也需要进一步拆分。...，直到f(1)，这就是*递*。f(1)解决了，
则f(2)=2\*f(1)也解决了，到最后f(6)也就解决了，这就是*归*。所以递归的本质是把问题拆分成**具有相同解决思路的子问题**，直到最后被拆解的子问题再也不能被拆分，

---

## 递归解题思路

通过上面对递归的剖析，可以发现递归往往有2个特点：
1. 一个问题可以拆解成具有相同解决思路的子问题；
2. 层层拆解后，最后一个子问题一定是已知可求解的；

那么，递归解题有什么套路呢？总结如下：
1. 定义函数，明确函数功能（要干啥？）；
2. 确定递归结束条件；
3. 寻找递推公式或递归关系；
4. 分析递归复杂度（空间/时间），进行优化；

---

## 实战演练剖析

### 热身赛

> 输入一个整数n，求n的阶乘。n! = 1\*2\*3\*4\*...\*n

* 定义函数
    > 函数factorial(int n)表示输入n，返回其阶乘
    
    ```c++
    /*
    * 求n的阶乘
    */
    int factorial(int n);

    ```

* 确定递归结束条件

  > 1的阶乘，即：factorial(1) = 1

* 寻找递推公式
  
  > n的阶乘，即：factorial(n) = n * factorial(n - 1)
  
  ```c++
  int factorial(int n) {
      if (n <= 1)
          return 1;
  
      return n * factorial(n - 1);
  }
  ```

* 分析递归复杂度

  > 调用factorial函数n次，时间复杂度O(n)

---

### 入门题

> 一只青蛙可以一次跳1个台阶或一次跳2个台阶。例如，跳上第1级台阶只有一种跳法：直接跳1个台阶；
> 跳上第2级台阶有两种跳法：每次跳1个台阶，跳2次；直接跳2个台阶，跳1次。请问跳上第n个台阶，有
> 多少种跳法？

* 定义函数

  > 函数jump(int n)表示输入n，返回跳上n个台阶总共的跳法

* 确定递归结束条件

  > 跳上第1级台阶，即：jump(1) = 1；
  > 跳上第2级台阶，即：jump(2) = 2;

* 寻找递推公式  

  由于每次只能跳1个或2个台阶，那么要跳到第n级台阶，自上而下思考，
  必须从n-1或n-2级台阶开始跳。因此，求解到第n级台阶的跳法问题，
  就转化成求解跳到n-1和n-2级台阶的问题。很显然递推公式如下：
  
  > jump(n) = jump(n - 1) + jump(n - 2)

  ```c++
  int jump(int n) {
      if (1 == n) 
          return 1;
  
      if (2 == n) 
          return 2;
  
      return jump(n - 1) + jump(n - 2);
  }
  ```

* 分析递归复杂度

  ```c++
  
  ```

### 初级题

### 中级题

### 进阶题

---

## 总结

掌握套路，多多练习，无它～
