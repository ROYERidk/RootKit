# RootKit

- Réalisation d'un rootkit sous forme de Linux Kernel Module
- Automatisation de la création d'un système LFS Alpine pour tester le rootkit
- Comprehension et implémentation des mécanismes de camouflage
- Mise en place de la persistence du rootkit

### Installation et utilisation

#### 0. IMPORTANT
Ces scripts fonctionnent sur une architecture Arch Linux, si vous êtes sur debian vous devrez certainement changer quelques lien vers vos fichiers.

#### 1. Cloner le dépôt avec les sous-modules

L'utilisation des sous-modules Git nous permet d'inclure les sources du noyau Linux et de BusyBox.
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

Le rootkit au format *.c* nommé *ldk-kit.c* doit-être placé dans la racine du répertoire

`make module` compilera le rootkit en *.ko* avec le noyau linux de la VM pour qu'il n'y ai pas de problèmes de compatibilités entre la compilation sur notre machine et l'utilisation du module sur notre VM. 

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
