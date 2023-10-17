# WinMut源码解析

## TODO

- [ ] 全局变量的作用
  - [ ] GoodvarArg
  - [ ] GoodvarArgInBlock_
- [ ] optimizedInstrumentation的作用
- [ ] 判断是否需要解释
  - [x] HasMutant
  - [x] IsOriginal
    - [x] 如何区分子进程和原始进程
  - [ ] IsRedundant
- [ ] 判断是否需要融合分流
  - [ ] 找到不在本BB外使用的goodvar，保存为多值变量
  - [ ] 处理多值变量参与运算的情况（变分执行）
    - [ ] 多值变量存储
    - [ ] 多值变量参与运算
      - [ ] 多值变量作为操作数
      - [ ] 多值变量作为结果
- [ ] 通过查看对IR的修改明确流程

## 抽象

![Alt text](assets/README-IMAGES/README-WinMut%E8%A7%A3%E6%9E%90/image-1.png)

考虑两个重要分支点：

1. 判断是编译还是解释
2. 判断需不需要作等价类融合并分流

### 判断是否需要解释

**要素：直接运算还是call运行时库（WinMut）**  
通过在插入时创建基本块解决  [添加控制流](lib/Transforms/WinMut/WAInstrumenter.cpp#L2032)  

- 判据：全局变量GoodvarArg->status（在运行时被改变，改了之后再fork，从而影响子进程）
- 影响点：处理时进行修改  [修改GoodvarArg->status](include/llvm/WinMutRuntime/mutations/MutationManager.h#L732)

### 判断是否需要融合分流

**要素：是否分流**  
在运行时库中解决  
判据：
