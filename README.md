# RootKit

- Réalisation d'un rootkit sous forme de Linux Kernel Module
- Automatisation de la création d'un système LFS pour tester le rootkit
- Comprehension et implémentation des mécanismes de camouflage
- Mise en place de la persistence du rootkit

### Installation

#### 1. Cloner le dépôt avec les sous-modules

L'utilisation des sous-modules Git nous permet d'inclure les sources du noyau Linux et de BusyBox.
```bash
git clone --recurse-submodules git@github.com:ROYERidk/RootKit.git
cd my_rootkit
```

#### 2. Compiler le noyau Linux et BusyBox :

Compiler BusyBox :
```bash
cd busybox
make defconfig
make menuconfig
make -j$(nproc)
make install
cd ..
```
Si vous avez des erreurs à la compilation vérifiez que vous avez bien cocher "Build static binary" dans menuconfig et que vous avez mis à *n* la variable suivante dans le .config
```bash
nano .config
CONFIG_TC=n
```

#### 3. Exécuter le script de mise en place de l'environnement :
```bash
./devenv.sh
```

#### 4. Lancer notre VM victime avec QEMU
```bash
./boot.sh
```
