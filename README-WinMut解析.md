# WinMut源码解析

## TODO

- [ ] 全局变量的作用
- [ ] 如何删除冗余的解释器调用
- [ ] 如何在更大范围内做等价类融合

## 抽象

![Alt text](assets/README-IMAGES/README-WinMut%E8%A7%A3%E6%9E%90/image-1.png)

考虑两个重要分支点：

1. 判断是编译还是解释
2. 判断需不需要作等价类融合并分流

### 判断编译还是解释

通过对
