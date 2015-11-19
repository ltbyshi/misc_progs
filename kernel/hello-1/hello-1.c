#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>

int init_module()
{
    printk("Hello world!");
    __asm__("cli");
    return 0;
}

void cleanup_module()
{
    printk("Goodbye world!");
    __asm__("cli");
}
