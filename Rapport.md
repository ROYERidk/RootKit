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

| Nom     | Tâche                                                                                                                  | Rôle | Travail fournis |
| ------- | ---------------------------------------------------------------------------------------------------------------------- | ---- | --------------- |
| Pierre  | Création de l'environnement de developpement, rédaction des parties communes du rapport et création de la présentation |      | %               |
| Manu    | Création d'un module reverse shell, camouflage et persistance du Rootkit                                               |      | %               |
| Anthony | Création d'un module de reverse shell, camouflage et persistance du Rootkit                                            |      | %               |
| Théo    | Création d'un module de keylogger                                                                                      |      | %               |

### 3. Concepts et Architecture du Rootkit  
#### 3.1 Fonctionnalités

(à developper)
- Reverse shell
- misc divice
- keylogger

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
Manu ? Anthony ?

#### 5.2 Mécanisme de persistance
Manu ? Anthony ?

### 6. **Limites et Améliorations Potentielles**  
#### 6.1 Problèmes rencontrés et solutions apportées 

nous avons démarrer notre projet en utilisant une VM busybox. Mais, nous nous sommes rendu compte qu'un LFS Alpine nous conviendrait mieux. Notamment au niveau de la gestion des droits utilisateurs.

Aucun membres du groupe n'avais de notion avant le cours de conception d'un rootkit et plusieurs membres ne sont pas très à l'aise en programmation alors nous avons dû nous adapter. Nous avons débuté ce projet par l'étude de rootkit déjà existant ainsi que leur fonctionnement.

#### 6.2 Perspectives d’amélioration

Nous sommes conscient que le module de reverse shell est visible de *tel ou tel manière* et nous aurions pu le camoufler de *tel manière*.

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

### 8 **Bilan du projet**

Le développement de notre rootkit Linux s’est avéré être un défi technique et pédagogique enrichissant. Nous avons réussi à concevoir un rootkit fonctionnel répondant aux exigences fixées. De plus, ce projet nous a permis d'approfondir nos connaissances sur les mécanismes internes du noyau Linux.

Sur le plan pédagogique, ce projet nous a permis d’acquérir et de consolider plusieurs compétences :
- Compréhension du noyau Linux
- Résolution de problèmes
- Documentation et communication
