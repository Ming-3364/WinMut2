# WinMut源码解析

## 延伸

### 技术模型

- 实现对c源代码中对应的局部变量的融合
- 针对变异分析，提前进行基本块重构
- **探索保持变分执行的收益上限**
- 在设计上保留在局部位置执行高阶变异的可能
- 考虑概率模型？？？
- 从代码特征入手
  - 从比较条件顺藤摸瓜，（变量只作为判据，不在if中使用）
  
### 磨刀

- 对IR的理解：LLVM语言手册
- 指导思想：越抽象考虑越慢，具体情况具体分析更快
  - 挑选具有代表性的实验对象
  - 对实验对象的代码特征的理解
- 量子物理的多态与坍缩

### 注意事项

- 考虑统计数据，比如编译时间，运行时库的占用时间，在原始程序中的时间，以及中间空白出的调用时间。
- 考虑正确性验证，构建进程图
- **考虑能否通过llvm自己的分析拿到一些数据？？？**
- 考虑在llvm18上的重构？？

## TODO

- [ ] 全局变量的作用
  - [ ] Mutation mutarray[]
  - [ ] RegMutInfo mutrmi
  - [ ] MutSpecsxxx mutSpecsGV.xxx
  - [ ] MutSpecsxxx mutSpecsDepGV.xxx
  - [ ] MutSpecsxxx mutSpecsNoDepGV.xxx
  - [ ] GoodvarArg
  - [ ] GoodvarArgInBlock
  - [ ] BlockRegMutBound BlockBoundArray
  - [ ] GoodvarArg* GoodvarArgArray[]

- [ ] 判断是否需要解释
  - [x] HasMutant
  - [x] IsOriginal
    - [x] 如何区分子进程和原始进程
  - [ ] IsRedundant的判据

- [ ] 判断是否需要融合分流
  - [x] get & filter: 找到不在本BB外使用的goodvar，保存为多值变量
  - [ ] 处理多值变量参与运算的情况（变分执行）
    - [ ] 创建运行环境：多值变量存储
    - [ ] 运行时     ：多值变量参与运算
      - [ ] __accmut__process_i32_arith_GoodVar_init (needInit没有在任何地方被使用，所以init和普通的版本完全一致)
      - [ ] __accmut__process_i32_arith_GoodVar [process_Goodvar](include/llvm/WinMutRuntime/mutations/MutationManager.h#L679)
        - [ ] 多值变量作为操作数
        - [ ] 多值变量作为结果
      - [x] __accmut__process_i32_arith [process](include/llvm/WinMutRuntime/mutations/MutationManager.h#L590)

- [ ] 搭实验环境
  - [x] shell语法
  - [x] 看run.sh
  - [ ] makefile语法
  - [ ] 测试项目的执行
  - [ ] 编写demo，插入正确性验证和效率分析代码

- [ ] 考察
  - [ ] 考察内容
    - [ ] 十个项目中的模块个数、函数个数和基本块个数
  - [ ] 实现
    - [ ] pass的clang命令行启动
    - [ ] 设计demo

- [ ] 变分保持长度（BB长度）与执行时间
  - [ ] 执行时计时源码
  - [ ] 统计脚本与执行

## 抽象

![Alt text](assets/README-IMAGES/README-WinMut%E8%A7%A3%E6%9E%90/image-1.png)

考虑两个重要分支点：

1. 判断是编译还是解释
2. 判断需不需要作等价类融合并分流

### 判断是否需要解释

**要素：直接运算还是call运行时库（WinMut）**  
通过在插入时创建基本块解决  [添加控制流](lib/Transforms/WinMut/WAInstrumenter.cpp#L2032)  

winmut中一共实现了3级：

1. 函数级

    - 判据：mutid != 0 && (mutid < left || mutid > right) [函数级](lib/Transforms/WinMut/WAInstrumenter.cpp#L646)

2. 基本块级

    - 判据：mutid.local != 0

3. 指令级（仅涉及goodvar）

   - 判据：全局变量GoodvarArg->status（在运行时被改变，改了之后再fork，从而影响子进程）
   - 影响点：处理时进行修改  [修改GoodvarArg->status](include/llvm/WinMutRuntime/mutations/MutationManager.h#L732)

### 判断是否需要融合分流

**要素：对中间变量上执行的变异不进行融合分流**

1. 定义GoodVariables（中间变量）

    注意：在此框架中，从变异生成到变异插装都是在LLVM IR上进行的，这里的“中间变量”也是在LLVM IR中的定义。

    例如：

    ``` c
    int sum = a + b;
    int avg = sum / 2;
    return avg;
    ```

    这里的sum不会产生一个IR的中间变量，他反映在LLVM IR中时，是一个alloc出的局部变量，在运算完成后，需要store，不符合WinMut中对GoodVariables的定义。[(checkUser)](lib/Transforms/WinMut/WAInstrumenter.cpp#L2302)

    而以下的c代码对产生一个IR中的中间变量：

    ``` c
    unsigned sum = 5;
    sum = sum + 1 + 0;
    sum = sum + 1 + 0;
    unsigned avg = sum / 2;
    return avg;
    // 注一：此处不使用参数的a和b，否则会因为BB过大而被裁剪
    // 注二：此处将产生对应中间变量的代码复制一遍，是因为在filter中对中间变量要求数目超过1
    ```

    对应的IR：

    ``` llvm
    %0 = load i32, i32* %sum, align 4
    %add = add i32 %0, 1
    %add1 = add i32 %add, 0
    store i32 %add1, i32* %sum, align 4
    %1 = load i32, i32* %sum, align 4
    %add2 = add i32 %1, 1
    %add3 = add i32 %add2, 0
    store i32 %add3, i32* %sum, align 4
    %2 = load i32, i32* %sum, align 4
    %div = udiv i32 %2, 2
    store i32 %div, i32* %avg, align 4
    %3 = load i32, i32* %avg, align 4
    ret i32 %3
    ```

    这里的对sum的运算中，会产生2个IR的中间变量：`%add`和`%add2`，本质上是IR中的三元式的要求，使C到IR的变换过程中需要产生中间变量。

2. 处理中间变量

## 说明

- 使用tmpCtrl标识临时控制变更，仅用作梳理流程时的控制工具（脚手架）。
