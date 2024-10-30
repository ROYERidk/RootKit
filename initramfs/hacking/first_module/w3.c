#include <linux/init.h>
#include <linux/module.h>
#include "./kernel.h"

MODULE_LICENSE("GPL");

static int __init mod_init(void) { 

    printk(KERN_INFO "rootkit: init\n");    
    
    return 0;
}

static void __exit mod_exit(void) {

    printk(KERN_INFO "rootkit: exit\n");

}


module_init(mod_init);
module_exit(mod_exit);