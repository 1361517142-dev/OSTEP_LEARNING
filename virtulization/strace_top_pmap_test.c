// hello.c
// #include <stdio.h>

// int main() {
//     printf("Hello World\n");
//     return 0;
// }

// cpu_hog.c
// #include <stdio.h>
// #include <unistd.h>

// int main() {
//     printf("PID: %d\n", getpid());  // 打印进程ID
//     printf("开始死循环，占用CPU...\n");
    
//     while(1) {
//         // 空循环，纯消耗CPU
//     }
    
//     return 0;
// }

// memory_layout.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_initialized = 42;      // 数据段
int global_uninitialized;         // BSS段

int main() {
    int local_var;                // 栈
    int *heap_var = malloc(1024); // 堆
    
    printf("PID: %d\n", getpid());
    printf("代码段(main): %p\n", main);
    printf("数据段(global_initialized): %p\n", &global_initialized);
    printf("BSS段(global_uninitialized): %p\n", &global_uninitialized);
    printf("堆(heap_var): %p\n", heap_var);
    printf("栈(&local_var): %p\n", &local_var);
    
    printf("\n程序正在运行，请在新终端运行: pmap %d\n", getpid());
    printf("按 Enter 结束...\n");
    getchar();  // 等待输入，保持运行
    
    free(heap_var);
    return 0;
}