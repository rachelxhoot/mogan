# 《零基础SICP》公开课
《零基础SICP》面向任何无编程基础、对Scheme和编程的本质感兴趣的小伙伴。

我们期望读者：
+ 具有高中水平的数学基础和英文基础
+ 对操作系统（Windows/macOS/Linux三者之一）有一定了解，能够学会如何打开终端，并使用cd命令切换当前目录

如果您已经学会编程了，《零基础SICP》公开课仍旧值得您花时间观看和学习，您可以通过练习题来判断是否可以直接跳过其中的一些课程。

## 课件和软件
在墨干中点击`帮助->墨客星球`，可以找到《零基础SICP》的讲义、课件、练习、勘误等信息。

本课程提供的资料：
+ 交互式SICP：https://gitee.com/XmacsLabs/interactive-sicp
  - 本公开课所使用的讲义，源自SICP网页版，由墨客实验室手动整理为TeXmacs文档
  - 该仓库中每一课的讲义和练习以单独的TeXmacs文档存在
+ 墨干理工套件：https://mogan.app
+ S7 Scheme解释器安装包: https://gitee.com/XmacsLabs/s7-scheme-binary
+ S7 Scheme官网：https://ccrma.stanford.edu/software/s7/

相关资料：
+ 南京大学的[SICP课程主页](https://cs.nju.edu.cn/xyfeng/teaching/SICP/index.htm)
+ 北京大学的[SICP课程主页](https://www.math.pku.edu.cn/teachers/qiuzy/progtech/)
+ 新加坡国立大学的[SICP课程主页](https://www.comp.nus.edu.sg/~cs1101s/)
+ Berkeley的[CS 61A: 计算机程序的构造和解释](https://cs61a.org)
+ MIT的[SICP课程](https://ocw.mit.edu/courses/6-001-structure-and-interpretation-of-computer-programs-spring-2005/pages/syllabus/)
+ SICP中文译本：[《计算机程序的构造和解释》](https://book.douban.com/subject/1148282/)
+ SICP课本官网：https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/index.html
+ R7RS Small: https://small.r7rs.org

## 第9课：创办一家一周工作四天的公司
+ B站回放：https://www.bilibili.com/video/BV1AH4y157T4/
+ 操作系统：Debian 12
+ 墨干版本：v1.2.5.4

沈浪熊猫儿（人类）总结的视频内容：
### 一、学习小众编程语言，可以找到躺平的机会
+ `00:26` 创办一家一周工作四天的公司
+ `01:23` 一些使用小众编程语言的公司和场景
### 二、三鲤的使命、愿景、价值观和四天工作制
+ `04:53` 使命是服务科技创新与教育发展
+ `06:28` 愿景是定义人类传承、挖掘、创造知识的接口
+ `08:45` 团队第一、知行合一、自我成长、传递快乐、独立个体
### 三、做什么和怎么做
+ `23:21` 小e老师主导的项目是计算机代数系统，比如Mathematica（全程录制、全部开源、全靠大家）
+ `30:06` 四天工作制的具体规则，周一到周四是工作日，周六和周日是非工作日

## 第8课：有理数的数据抽象
+ B站回放：https://www.bilibili.com/video/BV1eT421y7bf/
+ 操作系统：Debian 12
+ 墨干版本：v1.2.5.3

MathAgape（人类）总结的视频内容

+ `00:11` 从本视频开始，进入 SICP 的第二章——构造数据抽象（第一章是构造函数抽象）。本视频对应的是 SICP 2.1.1～2.1.3的内容。

### 有理数的抽象
+ `01:22` 有理数与分式：构造函数、选择函数、显示函数
+ `03:54` 有理数的算数运算：分式的加、减、乘、除
+ `06:55` 定义分式加：用到 let*、选择函数、构造函数，并用 gcd 处理约分

### 抽象层级
+ `12:15` 接口与实现的分离
+ `16:18` 比喻：做蛋挞不必关心先有鸡还是先有蛋
+ `13:43` 答疑：浮点数？
+ `18:22` 答疑：如何让用户自然语言做计算？（NLP，Torch）

### 有序对的抽象
+ `22:58` 有序对构造器、选择器

### 链表的抽象
+ `26:24` 链表的构造器：()，cons；链表的选择器：car，cdr
+ `27:55` 一个特殊的构造器：list

### 定义数据
+ `35:45` 定义数据的一般性步骤：确定逻辑自洽的选择函数和构造函数
+ `37:50` 例子：基于皮亚诺公理，定义自然数这种数据

### 总结
+ `42:14` 为什么需要抽象层级？
+ `44:15` Scheme 原语（R7RS small）

### 答疑、补充
+ `46:14` 答疑：关于书上 2.1.3 的代码
+ `51:26` 补充：链表本质是递归的嵌套的 pair，即使显示出来看起来是平铺的形式

### 代码清单
+ `54:01` 中文编程与分数的排版显示是如何实现的
+ `58:04` 在构造时就化简分数
+ `59:02` 架构师与抽象层级

## 第7课：用高阶函数做抽象-lambda,let
+ B站回放：https://www.bilibili.com/video/BV16T42117es/
+ 操作系统：Debian 12
+ 墨干版本: v1.2.5.2

MathAgape（人类）总结的视频内容
### 预热
+ `00:19` 用 let 循环一个函数运行多次（这是针对习题1.22-1.24的答疑；有些程序运行一次时间极短，难以统计时间，可用循环很多次的方式解决）
+ `03:55` 比较 Python

### 正课
+ `07:14` 本课会替换掉书本上较为复杂的数学例子，让编程教程更易懂
+ `08:43` 回顾第一课的观念：编程的基本原理是对数据和计算的组合和抽象（总结，非书上原话）；本课重点：用函数去做计算的组合和抽象
+ `09:18` 以求和公式为例，讲解在参数中传递函数值（函数也可作为参数）
+ `12:42` 用 lambda 构造匿名函数的好处
+ `14:46` 以斐波那契数对，讲解用 let 创建局部变量的好处（避免重复计算，方便修改）
+ `17:36` 实例：区间折半法，搜索算法，二分查找法
+ `29:08` 实例：寻找不动点（在函数的有限次迭代之后回到相同值的点叫做周期点；不动点是周期等于 1 的周期点）
+ `34:32` 总结
### 答疑
+ `38:32` Scheme 的 let 相当于 Python 的？；宏：let*；let 与 define

## 第6.2课：时空复杂度习题1.14~1.20讨论
+ B站回放：https://www.bilibili.com/video/BV1E1421U7AM/
+ 操作系统：Windows 11
+ 墨干版本：v1.2.5.1

MathAgape（人类）总结的视频内容：
+ `00:22` 习题1.14
+ `08:07` 习题1.15
  - `08:46` 法1:用调试控制台查看计算次数
  - `09:37` 法2:用全集变量输出计算次数
  - `13:22` 法3:用参数变量模拟全局变量，更符合函数式编程的本意
+ `14:28` 习题1.16
  - `14:34` 用分段函数表示
+ `19:41` 习题1.17
  - `20:22` 位运算
  - `31:28` 逻辑与
+ `38:12` 习题1.18
  - `39:45` 矩阵乘法
  - `42:23` 用 Octave 插件计算矩阵
  - `45:33` 下载 Octave
+ `47:08` 习题1.20
  - `51:00` 打印应用序求值计算过程次数

## 第6.1课：时空复杂度随堂练习答疑
+ B站回放：https://www.bilibili.com/video/BV13r421h7gi/
+ 操作系统：Windows 11
+ 墨干版本：v1.2.5.1

MathAgape（人类）总结的视频内容：
+ `00:24` 优化 list-min
+ `03:28` 实现 append
+ `04:49` 比较不同斐波那契算法的时空复杂度，并通过实验比较真实消耗时间（详细讲解了数形递归的时空复杂度如何估算）
+ `15:52` 墨干图形编程

## 第5课：时空复杂度
+ B站回放：https://www.bilibili.com/video/BV1N1421Q78o/
+ 操作系统：UOS 1060 (龙芯3A5000)
+ 墨干版本：v1.2.5.1

MathAgape（人类）总结的视频内容：
+ `00:12` 介绍我们的官方主页，这里汇总零基础SICP公开课的所有信息：https://mogan.app/zh/guide/SICP.html
+ `02:48` 源起杂谈：南京大学的SICP课程（冯新宇）
+ `10:46` 本课开始
+ `11:54` 什么是正则？
+ `15:42` Θ记法、增长阶
  - `18:00` 算法导论对 Θ、O、Ω 的形式化定义
+ `20:22` 链表（list）的底层原理
  - `27:30` 求链表长度/链表中最小值的时间复杂度
  - `31:46` 【练习1:优化 list-min】
+ `34:38` range（n）
  - `37:58` 【练习2:用 cons 实现 append】
+ `43:32` 求幂算法
  - `44:14` 应用序展开，递归，延迟计算，栈，空间复杂度
+ `1:02:04` 求幂算法优化方案
+ `1:14:34` 最大公约数
  - `1:16:30` 【练习3:证明欧几里得算法的复杂度为Θ(log n) 】
+ `1:18:14` 总结
  - `1:24:30` 【练习4:找到最优化的斐波那契数列，实验看看它在3s内能算到的最大值】
+ `1:30:04` 排序算法
  - `1:30:26` 不可变结构（immutable）
  - `1:32:42` 冒泡排序

## 第4.2课：递归与迭代 习题1.11~1.13
+ B站回放：https://www.bilibili.com/video/BV14t421L7Pg/
+ 操作系统：Windows 10
+ 墨干版本：v1.2.5

MathAgape（人类）总结的视频内容：
+ `00:23` 习题 1.9 复习
  - 辨析过程（procedure）与计算过程（process）
+ `03:57` 习题 1.10 复习
  - 考虑n为非自然数的情况
+ `08:13` 习题1.12 杨辉三角
+ `11:48` 怎样下载、使用解释器（这次是Windows系统的教程，MacOS 见上期）
+ `22:26` 副作用
+ `30:53` 在cmd里打印杨辉三角
+ `36:08` 练习：写出和书上格式一样居中对齐的杨辉三角】
+ `43:42` 在墨干的调试控制台里打印杨辉三角（用for）
+ `55:48` for 循环的定义（宏）
+ `37:42` 习题1.13 证明Fib(n)以指数增长
+ `59:53` 答疑：习题1.2

注意：这一期中讲到quasiquote，实际上是墨干V1.2.5的一个错误，这个错误我们将在下周一（2024/03/18）之前发布的墨干V1.2.5.1中解决，目前不会影响大家学习SICP第一章的。

## 第4.1课：递归与迭代 习题1.9~1.10
+ B站回放：https://www.bilibili.com/video/BV1Kz421Q72B/
+ 操作系统：macOS 14
+ 墨干版本：v1.2.5
+ 勘误：英文课件习题1.13的希腊字母 ψ 应读作 psai

MathAgape（人类）总结的视频内容：
+ `01:31` 复习：代码清单
+ `04:44` 技巧：用scheme生成墨干里的树
  ``` scheme
  (stree->tree '
         (tree "a"
            (tree "b")
            (tree "c")))
  ```

+ `06:28` 习题1.9 
  - 用 inc、dec实现加法的两种方式
  - 递归/迭代的计算过程（可参考书上p.21-23）

+ `11:41` 习题1.10
  - Ackermann 函数
  - 墨干的可折叠结构（13:04）
    * 插入->可运行->Scheme
    * 通过回车操控折叠

+ `16:09` 习题1.13 提示

+ `20:21` 怎样下载、使用解释器
- `20:51` 下载地址
- `20:59` macOS 演示教程
  ``` bash
  ; 首先打开“终端”
  cd ; 进入家目录
  ls ; 查看家目录

  mkdir bin ; 新建文件夹
  ; 把下载的文件放到 bin 文件夹，可改短文件名为 s7
  cd bin
  chmod +x s7 ; 添加可执行权限
  ./s7 ; 运行
  ctrl+c ; 退出
  touch add.scm ; 创建 scm 文件

  ; 编辑 scm 文件，例如：
  (display (+ 1 1))
  (display "\n")

  time ./s7 add.scm ; 运行并查看时间
  ```


## 第3课：递归与迭代
+ B站回放：https://www.bilibili.com/video/BV1UJ4m1a7PA/
+ 操作系统: Windows 11
+ 墨干版本: v1.2.5

MathAgape（人类）总结的视频内容：
+ `00:45` 答疑 关于REPL
  - 以文心一言类比REPL
  - 没有REPL的语言？ c语言
  - 编译器与解释器的区别
+ `12:01` 复习：条件表达式
  - 应用序求值
  - 短路运算
  - 以围棋类比scheme语言
+ `31:15` 循环：求和（书上是阶乘）
  - 伪代码与常规写法（for、set!）
  - 线性递归
  - 线性递归的应用序展开
+ `01:03:00` 递归&迭代：斐波那契数列
  - 树形递归
  - 两种迭代法实现
  - 比较三种算法的效率（焦点工具栏->输出选项->显示花费时间）

## 第2.2课：编程的基本原理 习题1.7~1.9
+ B站回放: https://www.bilibili.com/video/BV1gx4y1C7wn/
+ 操作系统: macOS 13 (M1芯片)
+ 墨干版本：v1.2.4

MathAgape（人类）总结的视频内容：
+ `00:35` 习题1.7
  - 关键1：把 good-enough? 的算法从“绝对误差”改成“相对误差”，即可避免被开方数过小或过大导致的问题；
  - 关键2：由于 sqrt 已经在标准库中，做练习时建议起一个新名字如 sqrt-new，以免混淆
+ `06:40` 开发技巧：如何一次性执行全部代码，而不是一个个回车
  - 方法1：转成 .scm 文件，再使用“调试控制台”或“终端”查看
  - 方法2：焦点工具栏：求值->全部求值
+ `16:48` 如何交作业
  - 文件->导出->可编辑PDF（方便我们在邮箱预览）
+ `24:47` 习题1.9
  - 预热讨论：什么是inc、dec；什么是寄存器
## 第2.1课：编程的基本原理 习题1.1~1.8
+ B站回放: https://www.bilibili.com/video/BV1kU421d7jz/
+ 操作系统: macOS 13 (M1芯片)
+ 墨干版本：v1.2.4
+ 内容：第一章第一节的练习题
+ 勘误：习题1.7的讲解有误，正确思路是：把绝对误差的算法改成相对误差。
## 第1课：编程的基本原理
+ B站回放：https://www.bilibili.com/video/BV1cp421f7xP/
+ 操作系统：macOS 13 (M1芯片)
+ 墨干版本：v1.2.4
+ 内容
  - 表达式
  - 命名与环境
  - 组合式的求值
  - 复合函数
  - 函数应用的代换模型
  - 条件表达式和谓词
## 第0课：准备Scheme编程的环境
+ B站回放：https://www.bilibili.com/video/BV1CK421y77g/
+ 操作系统：UOS 1050 (龙芯3A5000)
+ 墨干版本：v1.2.4
+ 内容：
  - 如何下载安装墨干
  - 如何在墨干中插入Scheme会话
  - 如何通过`帮助->墨客星球`找到《零基础SICP》的交互式讲义、课件、练习、代码清单