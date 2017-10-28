FAIRE TOURNER LES DIFFERENTS PROGRAMMES :

Un makefile permet de compiler tous les programmes, soit individuellement soit tous d'un coup avec la commande make.

dijkstra :
Ce programme implémente la version séquentielle naïve de dijkstra.
Pour lancer le programme sur un des fichiers rcsp faire : "./dijkstra rcspX.txt"
Lancer le programme de la manière suivant "./dijkstra » le fait tourner sur un graphe créé pour l'exemple, contenu dans le fichier Graph.cpp.

dijkstra_opti :
Ce programme implémente la version séquentielle améliorée de dijkstra (en terme de complexité) avec l'utilisation des tas.
Pour lancer le programme sur un des fichiers rcsp faire : "./dijkstra_opti rcspX.txt"
Lancer le programme de la manière suivant "./dijkstra_opti" le fait tourner sur un graphe créé pour l'exemple, contenu dans le fichier Graph.cpp.


dijkstra_opti_p :
Ce programme implémente la version parallèle améliorée de dijkstra (en terme de complexité) avec l'utilisation des tas.
Pour lancer le programme sur un des fichiers rcsp faire : "mpirun -np N ./dijkstra_opti_p rcspX.txt"
Lancer le programme de la manière suivant "mpirun s./dijkstra_opti_p" le fait tourner sur un graphe créé pour l'exemple, contenu dans le fichier Graph.cpp.


shortest_constraints : 
Ce programme implémente une première façon de résoudre le problème du plus court chemin avec contraintes de manière séquentielle.
Pour lancer le programme sur un des fichiers rcsp faire : "./shortest_constraints rcspX.txt"
Lancer le programme de la manière suivant "./shortest_constraints" le fait tourner sur un graphe créé pour l'exemple, contenu dans le fichier Graph.cpp.


shortest_constraints_p : 
Ce programme implémente une première façon de résoudre le problème du plus court chemin avec contraintes de manière parallèle.
Pour lancer le programme sur un des fichiers rcsp faire : "mpirun -np N ./dijkstra_opti_p rcspX.txt"
Lancer le programme de la manière suivant "mpirun -np N ./dijkstra_opti_p" le fait tourner sur un graphe créé pour l'exemple, contenu dans le fichier Graph.cpp.
Ne pas mettre plus de processeurs qu'il n'y a de sommets dans le graphe (4 sommets dans le graphe par défaut). 

k_shortest :
Ce programme implémente une façon de résoudre le problème des k plus courts chemins.
Pour lancer le programme sur un des fichiers rcsp et afficher les j plus courts chemins faire : "./k_shortest j rcspX.txt"
Attention, ne pas choisir j excessivement grand : le programme ne gère pas le cas où il existe moins de j chemins entre s et t

k_shortest_p :
Ce programme implémente une façon de résoudre le problème des k plus courts chemins.
Pour lancer le programme sur un des fichiers rcsp et afficher les j plus courts chemins faire : "./k_shortest j rcspX.txt"
Attention, ne pas choisir j excessivement grand : le programme ne gère pas le cas où il existe moins de j chemins entre s et t
Une erreur inconnue empêche l'exécution du fichier

