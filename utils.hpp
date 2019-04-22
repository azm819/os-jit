#include <memory>
#include <sys/mman.h>
#include <cstring>

void *data;

unsigned char code[] = {0x55,                                     //   	push   %rbp
                        0x48, 0x89, 0xe5,                         //  	mov    %rsp,%rbp
                        0x89, 0x7d, 0xec,                         //  	mov    %edi,-0x14(%rbp)
                        0xc7, 0x45, 0xf4, 0x01, 0x00, 0x00, 0x00, // 	movl   $0x1,-0xc(%rbp)
                        0xc7, 0x45, 0xfc, 0x02, 0x00, 0x00, 0x00, // 	movl   $0x2,-0x4(%rbp)
                        0xc7, 0x45, 0xf8, 0x00, 0x00, 0x00, 0x00, //	movl   $0x0,-0x8(%rbp)
                        0x8b, 0x45, 0xf8,                         //  	mov    -0x8(%rbp),%eax
                        0x3b, 0x45, 0xfc,                         // 	cmp    -0x4(%rbp),%eax
                        0x7d, 0x10,                               //   	jge    34 <_Z1fi+0x34>
                        0x8b, 0x45, 0xf4,                         //   	mov    -0xc(%rbp),%eax
                        0x0f, 0xaf, 0x45, 0xec,                   // 	imul   -0x14(%rbp),%eax
                        0x89, 0x45, 0xf4,                         // 	mov    %eax,-0xc(%rbp)
                        0x83, 0x45, 0xf8, 0x01,                   //  	addl   $0x1,-0x8(%rbp)
                        0xeb, 0xe8,                               //   	jmp    1c <_Z1fi+0x1c>
                        0x8b, 0x45, 0xf4,                         // 	mov    -0xc(%rbp),%eax
                        0x5d,                                     //   	pop    %rbp
                        0xc3,                                     //   	retq
};

int size = sizeof(code);
size_t powPosition = 17;
int currentPow = 2;

typedef int (*f)(int);

void printRules() {
    std::cout << "Эта функция вычисляет степень числа(квадрат по дефолту)\n"
                 "_______________________________________________________\n"
                 "                                                       \n"
                 "\"calculate\" ------------------ Запуск функции,\n"
                 "------------------------------ после чего будет предложенно ввести число\n"
                 "\"modify\" --------------------- Изменение степению \n"
                 "------------------------------ после чего будет предложенно ввести степень\n"
                 "\"current_pow\" ---------------- Вывести вычисляемую на данный момент степень\n"
                 "\"exit\" ----------------------- Завершить сеанс\n"
                 "\"help\" ----------------------- Показать эту информацию\n\n";
}

void predProccess() {
    data = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        return;
    }
    memcpy(data, code, size);
}

void changeAccess(int access) {
    if (mprotect(data, size, PROT_READ | access) == -1)
        perror("mprotect");
}

int execute(int x) {
    changeAccess(PROT_EXEC);
    auto function = (f) data;
    return function(x);
}

void modify(int n) {
    changeAccess(PROT_WRITE);
    n = (unsigned char) n;
    ((unsigned char *) data)[powPosition] = n;
    currentPow = n;
}

void getCurrentPow() {
    std::cout << "Степень на данный момент: " << currentPow << '\n';
}

void freeUpMemory() {
    if (munmap(data, size) == -1)
        perror("munmap");
}