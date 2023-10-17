# AccMut解析

## TODO

- [ ] 弄清楚recent_set等变量的意义
  - [ ] 走一遍从原始进程分流的流程
  - [ ] 走一遍从某个子进程分流的流程

## Issues

1. HOLDER和MUTATION_ID的作用？？？
   1. 根据MUTATION_ID的宏定义，它只用来标识父子进程，HOLDER为什么要开得这么大？（没有找到其他地方对HOLDER或者MUTATION_ID的使用）

## 流程

### EM，ME，SS

1. recent_set

    当前执行变异分析的指令上的活跃变异体

2. default_active_set
3. forked_active_set
4. temp_result
5. eqclass
