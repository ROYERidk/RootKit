### 1. Introduction

#### 1.1 Contexte et objectifs du projet

Dans le cadre de ce projet, nous avons été chargés de concevoir et de développer un **rootkit Linux** sous la forme d’un module noyau. L’objectif principal est de mettre en œuvre les concepts étudiés relatifs au noyau Linux, aux appels système, et aux mécanismes de camouflage et de persistance, tout en respectant les contraintes techniques définies.

Ce travail nous a permis de manipuler directement les structures internes du noyau et de développer un ensemble de fonctionnalités avancées, telles que la surcharge d’appels système et la communication avec des programmes compagnons, tout en nous conformant aux bonnes pratiques de codage sécurisées.

#### 1.2 Rappel des exigences et contraintes du sujet

Le projet a été réalisé en respectant les directives suivantes :

- **Création d’un environnement de test** : Nous avons automatisé la création d'un système Linux From Scratch Alpine pour démontrer le fonctionnement de notre rootkit.

- **Fonctionnalités obligatoires** : Développement d’un rootkit offrant un camouflage efficace, une persistance sur le système, et une capacité à interagir avec des programmes compagnons via des moyens tels que des appels système.

- **Respect des contraintes** : Conformité avec les règles de codage sécurisées de l’ANSSI, interdiction de l’utilisation détournée de l’appel système _kill_, et garantie de l’originalité du code produit.

Ce rapport détaille nos choix techniques, la mise en œuvre du rootkit, ainsi que l’organisation du travail au sein de l’équipe. Il offre également une analyse des défis rencontrés et des solutions apportées, en vue de démontrer notre compréhension des concepts abordés.

### 2. Organisation du Projet
#### 2.1 Tableau de répartition des tâches, des rôles et du travail fournis

| Nom     | Tâche                                                                                                                  | Travail fournis |
| ------- | ---------------------------------------------------------------------------------------------------------------------- | --------------- |
| Pierre  | Création de l'environnement de developpement, rédaction des parties communes du rapport                                | %     28.33     |
| Manu    | Création d'un module reverse shell, camouflage et persistance du Rootkit                                               | %     28.33     |
| Anthony | Création d'un module de misc device et aide à la création d'un module de reverse shell, camouflage du Rootkit          | %     28.33     |
| Théo    | Création d'un module de keylogger                                                                                      | %     15        |

#### 2.2 Github

Voici le lien de notre répertoire Github pour ce projet : https://github.com/ROYERidk/RootKit

### 3. Concepts et Architecture du Rootkit  
#### 3.1 Fonctionnalités

Le rootkit correspond à un module que l'on va charger sur la machine cible. Le rootkit que nous avons implémenté est basé sur l'utilisation de kprobes pour effectuer un hook de la fonction do_sys_openat2. Nous avons coder le module de tel sorte qu'un reverse shell soit initié vers un serveur C2 dès lors qu'un fichier est lu via la commande cat.

#### 3.1.2 Module misc device
		
Un misc device est un type de périphérique dans le noyau Linux.C'est est un périphériques de type caractère, il permet de faire une escalade de privilège après avoir recu une cléfs d'activation de la part d'un autre module.


#### **Perspectives d'Évolution**
Amélioration de la furtivité : Optimiser le camouflage des processus, fichiers et répertoires via des techniques avancées de hooking (getdents, /proc, etc.)

Gestion conditionnelle des privilèges : Intégrer une élévation granulaire ou conditionnée à des critères précis.
Compatibilité multi-noyaux : Adapter le module aux noyaux récents.

#### 3.1.3 Module Keylogger

#### **Défis de Déploiement et Intégration**

La première version du keylogger a été développée en Python, utilisant la bibliothèque `pynput` pour la capture des événements clavier. Cette approche initiale a été marqué par deux défis techniques majeurs :

**1. Dépendances Python**
L'environnement Linux From Scratch imposait une intégration précise des bibliothèques :
- `pynput` : Capture des événements clavier
- `requests` : Transmission des données
- `json` : Sérialisation 
- `threading` : Gestion asynchrone

**2. Contraintes de l'Environnement**
- Faible consommation de ressources système
- Indépendance vis-à-vis des configurations spécifiques
- Possibilité de charger/décharger le module sans impact sur la stabilité du système

#### **Évolution vers un Module Noyau**
Suite aux discussions avec l'équipe, un recode a été entrepris. Le module a été 'réécrit' en langage C, sous forme de module noyau Linux, intégrant directement le script Python comme une chaîne de caractères.

#### **Fonctionnement Technique**
Le module noyau encapsule le script Python et l'exécute via l'infrastructure noyau Linux. Les points clés incluent :

- Définition du script Python comme une chaîne de caractères dans le code C
- Utilisation de call_usermodehelper() pour lancer l'interpréteur Python
- Configuration d'un environnement minimal pour l'exécution du script
- Journalisation des événements via printk()

#### **Perspectives d'Évolution**

Axes d'amélioration :
- Chiffrement des données (Chiffrer les frappes avant de les stocker pour les protéger contre l’accès non autorisé.)
- Transmission furtive 
- Optimisation mémoire (Réduire l’empreinte mémoire du keylogger en effaçant les données après leur traitement.)
- Furtivité (Masquer les fichiers et processus associés au keylogger pour éviter toute détection.)
- Optimisation de la persistence (envisager de l’améliorer en injectant le code dans des processus système légitimes, comme ceux utilisés pour les mises à jour système ou les processus en cours d'exécution au démarrage)

#### 3.3 Scénarios d’accès initial

Pour la démonstration de notre rootkit nous utilisons un fichier partagé entre la VM Alpine et la machine hôte. Cela mime une intrusion physique qui se conclu en une clé USB branché sur une machine.

### 4. **Environnement de Développement**
#### 4.1 Arborescence du projet

***SCHEMA***

#### 4.2 Configuration et compilation du Kernel Linux
Lors de la première utilisation le Kernel à besoin d'être configuré puis compilé, c'est pour cela qu'il est mis en place un *make first-time-setup*.

Si il est seulement nécessaire de compilé le Kernel, un *make kernel* suffira.

#### 4.3 Configuration de la VM LFS Alpine

La VM LFS Alpine est configurée automatiquement grâce au script *build_img.sh* qui peut être appelé de deux manières avec le Makefile:
- make disk : Regarde si l'image disque *disk.img* existe déjà, si oui il ne fait rien. Sinon, il crée cette dernière.
- make vm : Appelle *make disk* puis le script *boot.sh* pour lancer la VM.

#### 4.4 Compilation des modules
Il est important que notre rootkit soit compilé avec le Kernel correspondant à notre VM sous peine que les modules de notre
Rootkit se trouvent incompatible avec la machine qu'elle infecte.
C'est pour cela que notre Makefile permet de compiler facilement et avec le bon Kernel notre Rootkit. A l'aide de *make default* nos modules au format *.c* situé à la racine du projet seront compilés correctement.

#### 4.5 Lancement de la VM
La vm est lancée à l'aide de *make vm* qui appelle *make disk* puis le script *boot.sh* pour lancer la VM.


### 5. **Conception Technique**  
#### 5.1 Techniques de camouflage  
	Plusieurs techniques de camouflage ont été utilisées dans notre tentative d’obfuscation des répertoires et des processus. Voici un récapitulatif des approches testées :

## Suppression de structures liées dans les listes du noyau
Nous avons utilisé la fonction list_del(&mod) pour supprimer des modules du noyau Linux afin de cacher leur présence dans les structures internes. Cette méthode est couramment utilisée dans le développement de rootkits pour retirer un module de la liste des modules chargés (/proc/modules).

## Suppression d’objets kobject
L’appel à kobject_del a été employé pour supprimer des objets dans la hiérarchie des kobject utilisé pour masquer des fichiers ou des entrées dans /sys. Bien que cela permette 

Hooking de la fonction getdents
Nous avons tenté de détourner la fonction système getdents, qui est responsable de fournir la liste des entrées d’un répertoire, pour filtrer certaines entrées et ainsi cacher des répertoires ou des fichiers spécifiques. L’objectif était de camoufler les répertoires créés pour le fonctionnement du module. Cependant, l'implémentation de ce hook n’a pas été concluante.

Masquage des processus
Une technique similaire a été appliquée au niveau des processus, en tentant de manipuler les appels systèmes liés aux listes de processus getdents appliqué à /proc ou en manipulant directement les structures task_struct via des hooks dans la table sys_call_table. 

#### 5.2 Mécanisme de persistance
Création d'un fichier de configuration d'un service pseudo légitime dans /etc/init.d/network-autostart qui va insérer le module infecté lors du démarrage de la machine.

### 6. **Limites et Améliorations Potentielles**  
#### 6.1 Problèmes rencontrés et solutions apportées 

nous avons démarrer notre projet en utilisant une VM busybox. Mais, nous nous sommes rendu compte qu'un LFS Alpine nous conviendrait mieux. Notamment au niveau de la gestion des droits utilisateurs.

Aucun membres du groupe n'avais de notion avant le cours de conception d'un rootkit et plusieurs membres ne sont pas très à l'aise en programmation alors nous avons dû nous adapter. Nous avons débuté ce projet par l'étude de rootkit déjà existant ainsi que leur fonctionnement.



#### 6.2 Perspectives d’amélioration

- Persistance du Rootkit : Nous avons manqué de temps pour la persistance du rootkit, elle n'est pas parfait et n'est pas comme nous l'aurions voulu. C'est pour cela que nous pourrions l'améliorer.
- Centralisation des modules : Nous n'avons pas réussi à centralisé tous nos modules en un seul Rootkit.
- La dissimulation du Rootkit : La discretion du Rootkit est améliorable.

### 7 Informations sur les comptes (root et utilisateur)
Nous avons deux utilisateur, un administrateur *root* avec tous les droits et un utilisateur *user* 

#### 7.1 Identifiants
root:root
user:user

### 8 Difficultés rencontrés

#### 8.1 Compilation des modules

Dans un premier temps nous pensions compiler les modules sur notre machine de travail pour les envoyé à la VM tel quel. Malheuresement nous nous sommes vite rendu compte que cela pose des problèmes de compatibilité. Nous avons donc crée dans le Makefile une ligne pour compiler automatiquement avec le Kernel de notre VM notre Rootkit.

#### 8.2 f_trace_helper
Nous avons voulu utiliser f_trace_helper, malheuresement nous nous sommes rendu compte que dans les versions récentes de Linux ce n'est plus possible. En effet, cela à été retiré/modifier pour des raisons de sécurité.
Après quelques recherches nous avons trouvé kprobe qui permet de hook relativement facilement des appels systèmes.

### 8.3 kprobes
Plus spécifiquement des difficultés, on été rencontré sur le hook du syscall getdents. Dans le post handler nous ne pouvions pas récupérer le retour dans la fonction avec a fonction regs_return_value() ni en la récupérant directement depuis le registre rax.

#### 8.4 Initiation au développement d'un Rootkit
Aucun membre de notre groupe n'avait de notion de confection d'un Rootkit c'est pour cela que notre difficulté principale à été de comprendre et imaginer notre propre Rootkit.

### 9 **Bilan du projet**

Le développement de notre rootkit Linux s’est avéré être un défi technique et pédagogique enrichissant. Nous avons réussi à concevoir un rootkit fonctionnel répondant aux exigences fixées. De plus, ce projet nous a permis d'approfondir nos connaissances sur les mécanismes internes du noyau Linux.

Sur le plan pédagogique, ce projet nous a permis d’acquérir et de consolider plusieurs compétences :
- Compréhension du noyau Linux
- Résolution de problèmes
- Documentation et communication
