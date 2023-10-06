# AccMut

## 概述

AccMut项目本质上是一个编译器，但不以编译为目标，而以变异为目标。旨在编译时对程序按照一定的模式施加变异（比如加法变减法），并加速执行变异体的过程。

## 工作原理

AccMut的三个重要组成部分：  
 1. 变异生成器：Mutation Generator

	按照一定规则生成变异点，输出变异描述文件，指导插装器插入变异。

 2. 变异插装器：Mutation Instrumenter

    根据变异描述文件，对不同类型的变异进行处理。	

 4. 运行时库：Runtime Library

    ？？？

----------------------------------------------------

### 1. 变异生成器：Mutation Generator

### 2. 变异插装器：Mutation Instrumenter

1. CallInst

2. StoreInst

3. ArithInst

4. ICmpInst
