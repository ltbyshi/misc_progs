#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>

static void recur(void)
{
    while(1){};
}

int init_module()
{
    printk("Hello world!");
    recur();
    __asm__("cli");
    return 0;
}

void cleanup_module()
{
    printk("Goodbye world!");
    __asm__("cli");
}
