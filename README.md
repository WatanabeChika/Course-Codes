# 关于此库

记录学习期间写的代码，按课程名将代码进行归类。  

**仅做记录用，勿吐槽代码质量。**

## CS1602 计算导论
HW 1-HW 3 小作业
> HW 1: Tower of Hanoi & The Josephus Problem ~~具体的题目已经记不清了~~  
> HW 2: 计算 n 以内的质数和判断大质数  
> HW 3: 高精度运算 ~~是的即使 Python 支持也要重写~~  

minimatrix 大作业
> 运用 numpy 库实现了一个自己的矩阵类

其他
> leetcode300 & LC300Holiday: 关于 LeetCode300 题的部分解答，仍未全部完成  
> leetcode300 题及答案也一并附上  

## CS1604 程序设计原理与方法

Assignment 1-Assignment 5 小作业  
> Assignment 1: C++基本语法  
> Assignment 2: 函数与库  
> Assignment 3: 字符串  
> Assignment 4: 抽象数据类型 (ADT)  
> Assignment 5: 设计 ADT

Final Project 大作业  
> Battlefield ~~丐版文明 6~~

cs1604_lib 是课程用到的库
> StanfordCppLib

详情请见各作业里的说明文档

## CS1605 程序设计实践

snake_src_mock 是课程给的基础运行代码  
mysnake 在 snake_src_mock 上做了个性化创新处理
> Snake Game 是一款模拟贪吃蛇游戏  
> **需在 Linux 环境下运行**

pets_battle 尚未完成 ~~绝不是因为是其他班的作业才没写呢！~~
> Pet Battle 是一款模拟宝可梦对战游戏  

## CS2602 数据结构

myclasses 是自建数据结构，包括：
> seqlist: 线性表  
> linklist/dlinklist: 链表/双向链表  
> btree: 二叉树  
> bsearchtree: 二叉查找树  
> sort: 排序函数  

奇怪的地方：main 是空的、random1 是 ~~打随机肉鸽用的~~ 随机整数生成器。

homework 是 5 次作业，题目详见 ***que_intro.md***。

## ICE2604 电类工程导论（B 类）

只有小组作业部分，具体来说：
> 1. 爬取了维基百科上关于 CS 的所有页面并整合成数据库。  
> 2. 搭建了 HTML 页面，完成了配套的 js、css 文件以供展示。 ~~丁真页面是测试用页面~~  
> 3. HTML 端实现了登录、收藏、数据检索功能，以及一张根本看不清的关系图。  
> 4. 还留了很多其他页面的接口，但是因时间有限并未完成。  

~~不要问我 tool.py 为什么是空的，因为本以为能整合出工具模块但是实际上并没有~~

## AI3603 人工智能理论及应用
HW 1-HW 3 小作业
> HW 1: A* 搜索算法  
> HW 2: 强化学习（算法/DNN）    
> HW 3: 优化图像分割模型（小组为单位）  

Final Project 大作业
> 五子棋对战 AI（MCTS/AlphaZero/DNN）

总体来说，除了 HW 1，其他需要完成的部分相对简单且局限（如调参）。

## CS3601 操作系统
分为 5 次 Lab 实验。具体实验内容、实验指导和初始代码在：[OS-Course-Lab
](https://github.com/SJTU-IPADS/OS-Course-Lab)。
> Lab 0: 拆炸弹（熟悉 ARM 汇编语句逻辑）  
> Lab 1: ChCore 机器启动  
> Lab 2: ChCore 内存管理    
> Lab 3: ChCore 进程线程管理     
> Lab 4: ChCore 多核调度和进程间通信

Lab 的提交结果以```tar.gz```形式存放在文件夹里。

## CS1108 数据科学引论——Python 之道

**选修课**
只有小组作业部分，具体来说：
> 1. 沿用了电类工程导论作业的思路，即爬虫+HTML 展示。  
> 2. 数据是萌娘百科上的明日方舟干员数据，ArkOperators.py 和 new.py 是爬虫文件。  
> 3. HTML 页面较简陋，主要是作为用 echarts 进行数据可视化的载体。 ~~但是这真的很酷~~  
> 4. 分职业页面仅仅改了后端数据，前端的可视化是一致的。  

希望这份明日方舟页面在今后能够被逐渐完善。 ~~但是应该不可能了，悲~~

## CS3612 机器学习

有两次编程作业。
> HW1：支持向量机（SVM）相关，图像二分类识别。  
> HW2：卷积神经网络（CNN）相关，手写数字识别。

## NIS3607 区块链技术及应用

只有期中大作业需要编程。
> 用 Golang 实现了一个简单的比特币 PoW 的仿真程序，模拟一定数量的节点生成区块链的状态。
> - 设置参数包括：节点数量、每个轮次出块的成功率
>    - 测量区块链的增长速度
>
> - 设置一定数量的恶意节点实施攻击
>     - 测量不同恶意节点比例（10%-40%）条件下，统计分叉攻击成功的长度
>     - 测量不同恶意节点比例条件下，自私挖矿收益比例

## NIS7023 多媒体内容安全

只有期末大作业需要编程。
> 调用 API 和开源项目库，实现了文生图任务和 Deepfake 任务的批量处理。

## NIS8027 空间信息网络与安全前沿技术 

只有期末大作业需要编程。
> 利用论文《LEOCraft: Towards Designing Performant LEO Networks》的框架对其实验进行验证评估。