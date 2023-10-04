# WinMut2

## 主要目标

1. 扩展变异处理范围
2. 使用静态方法加速程序状态划分

## Example for quick start

[WinMut-README.md](WinMut-README.md)

## 领域历史

### 用途

### 方法进化

针对一阶变异（一个变体最多包含一个变异点）

1. Standard Mutation Analysis

   最简单的方法是什么？  
   为每个修改（变异体）都生成一个程序，分别执行。  
   ![Standard Mutation Analysis](image.png)

2. Split-stream Execution

   遇到变异点之前的程序不都一样吗？  
   先执行一个进程，等到变异点出现时，再分出子进程来执行（通过fork）。  
   ![Alt text](image-1.png)

3. AccMut  

   分出的子进程真彼此不同吗？
   程序执行的唯一依据就是对数据的修改，`int c = a + b`和`int c = a - b`或许在语义上不同，但从程序执行的角度而言（即程序状态），当`b = 0`时，此处的两个变异体是不需要分出子进程的。

4. WinMut

## More Infomation

- <https://llvm.org/docs/CodingStandards.html>
- <https://llvm.org/doxygen/>

## TODO

- [ ] 理解WinMut代码流程
  - [ ] 阅读AccMut源码
  - [x] 调试AccMut输出（放弃调试链接问题，直接调试WinMut代码输出）
  - [ ] 阅读WinMut源码
  - [ ] 调试WinMut输出

## 日报

- 2023-10-04

  阅读调试AccMut源码。  
  由于链接文件问题放弃调试AccMut输出，但仍可以查看其对IR的修改过程作为理解LLVM的入门。

- 2023-10-03

  针对Pass修改增量编译，构建AccMut

- 2023-10-02

  编译llvm

- 2023-09-29

  创建git项目
