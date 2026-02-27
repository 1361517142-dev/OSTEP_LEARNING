#include <stdio.h>    // 标准输入输出
#include <stdlib.h>   // 标准库
#include <string.h>   // 字符串处理
#include <unistd.h>   // Unix 系统调用
#include <sys/wait.h> // 等待子进程

#define MAX_CMD 1024
#define MAX_ARGS 64

// ========== 内建命令函数 ==========

// cd 命令：改变当前工作目录
// 参数：argv 参数数组
// 返回值：1 表示已处理（是内建命令）
int builtin_cd(char **argv) {
    // argv[0] 是 "cd"，argv[1] 是目标目录
    
    if (argv[1] == NULL) {
        // 没有参数，打印错误到标准错误输出
        // fprintf：指定输出流，stderr 无缓冲，立即显示
        fprintf(stderr, "cd: 缺少参数\n");
    } else {
        // chdir：系统调用，改变当前进程的工作目录
        // 参数：目标路径
        // 返回值：0 成功，-1 失败（如目录不存在）
        if (chdir(argv[1]) != 0) {
            // 失败，打印具体错误（如 "Permission denied"）
            perror("cd");
        }
    }
    return 1;  // 返回 1 表示"这是内建命令，已处理"
}

// exit 命令：退出 shell
int builtin_exit(char **argv) {
    // exit：终止当前进程，参数是退出码（0 表示正常）
    exit(0);
    // 不会执行到这里
}

// 检查并执行内建命令
// 参数：argv 参数数组
// 返回值：1 表示是内建命令已执行，0 表示不是内建命令
int check_builtin(char **argv) {
    // strcmp：字符串比较，相等返回 0
    // 比较 argv[0]（命令名）和 "cd"
    if (strcmp(argv[0], "cd") == 0) {
        return builtin_cd(argv);  // 执行 cd，返回 1
    }
    if (strcmp(argv[0], "exit") == 0) {
        return builtin_exit(argv);  // 执行 exit，返回 1
    }
    return 0;  // 不是内建命令，返回 0
}

int main() {
    char cmd[MAX_CMD];
    char *argv[MAX_ARGS];
    
    while (1) {
        printf("mysh> ");
        fflush(stdout);
        
        if (fgets(cmd, MAX_CMD, stdin) == NULL) break;
        cmd[strcspn(cmd, "\n")] = 0;
        
        // 解析命令
        int argc = 0;
        char *token = strtok(cmd, " ");
        while (token != NULL && argc < MAX_ARGS - 1) {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }
        argv[argc] = NULL;
        
        if (argc == 0) continue;
        
        // ========== 先检查是否是内建命令 ==========
        // 如果是，check_builtin 内部执行并返回 1，直接 continue 进入下一轮
        if (check_builtin(argv)) {
            continue;
        }
        
        // ========== 不是内建命令，用 fork/exec 执行外部程序 ==========
        pid_t pid = fork();
        
        if (pid == 0) {
            // 子进程
            execvp(argv[0], argv);
            perror("execvp");
            exit(1);
        } else if (pid > 0) {
            // 父进程等待
            waitpid(pid, NULL, 0);  // NULL 表示不关心退出状态
        } else {
            perror("fork");
        }
    }
    return 0;
}