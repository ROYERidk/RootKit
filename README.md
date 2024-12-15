# RootKit

- Réalisation d'un rootkit
- Automatisation de la création d'un système LFS Alpine pour tester le rootkit
- Comprehension et implémentation des mécanismes de camouflage
- Mise en place de la persistence du rootkit

### Installation et utilisation

#### 0. IMPORTANT
Ces scripts fonctionnent sur une architecture Arch Linux, si vous êtes sur debian vous devrez certainement changer quelques lien vers vos fichiers.

#### 1. Cloner le dépôt avec les sous-modules

L'utilisation des sous-modules Git nous permet d'inclure les sources du noyau Linux.
```bash
git clone --recurse-submodules git@github.com:ROYERidk/RootKit.git
cd my_rootkit
```

#### 2. Compiler le noyau Linux :

Si c'est la première fois:
  - make first-time-setup

Sinon
  - make Kernel

#### 3. Compiler le rootkit

Le rootkit au format *.c* nommé *hook.c* doit-être placé dans la racine du répertoire

`make default` compilera le rootkit en *.ko* avec le noyau linux de la VM pour qu'il n'y ai pas de problèmes de compatibilités entre la compilation sur notre machine et l'utilisation du module sur notre VM.

PS : ne pas oublier de modifier l'adresse IP du serveur C2 dans hook.c (on utilise ici une IP privée pour le POC)

#### 4. Creation de l'image disque et lancement de la VM

`make disk` va crée l'image disque, si le dossier *vm* n'est pas dans la racine de votre dépôt crée le avant.
`make vm` va regarder si l'image existe pour pouvoir lancé la vm, si oui il boot la vm avec QEMU sinon il crée l'image disque puis boot la vm.

#### 5. Login

root:root
user:user

#### 6. Dossier partagé HOST/VM

Pour pouvoir simuler l'injection d'un rootkit nous utilisons un fichié partagé entre notre machine et notre vm.

1. mettre notre rootkit dans `/tmp/qemu-share` pour l'envoyer sur la vm
2. lancer la VM
3. `mkdir /shared`
4. `mount -t 9p -o trans=virtio share /shared`
5. insmod /shared/ldk-kit.ko
