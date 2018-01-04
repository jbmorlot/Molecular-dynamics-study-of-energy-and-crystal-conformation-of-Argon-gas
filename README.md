## Description du programme
Le programme est découpé en 2 parties: Une partie qui va gérer la simulation et les mesures, ./dynamique_mol et une autre qui va lancer la simulation plusieurs fois en changeant les variables de la simulation, ./remote.

Les fichiers associés sont les suivants:
./dynamique_mol -> main.cpp toolbox.h/.cpp vecteur.h/.cpp world.h/.cpp  particule.h/.cpp defines.h/.cpp
./remote -> Remote_control.cpp

Le programme ./remote permet de lancer 2 simulations distincts:
*La simulation 1: Etude du système à energie constante
*La simulation 2: Etude des cristaux de l'Argon (<a href="https://github.com/jbmorlot/Molecular-dynamics-study-of-energy-and-crystal-conformation-of-Argon-gas/blob/master/Resulats%20Simulation/Simulation2_Etude_reseaux_cristallins_argon/exemple_crystal_Argon.jpg">Crystal structure image</a>)

Une fois la simulation lancée, un dossier sera crée dans lequel sera enregistré toutes les figures:
-> Distribution radiale, vitesse, temperature, position(<- fichier texte pour JMOL), energie... et un fichier appelé resume_simulations.txt dans lequel est écrit les variables de chaques simulations exectuées ainsi que si elles ont divergées ou pas.

Pour changer les paramètres des simulations, il suffit de modifier le fichier Remote_control.cpp. La majorite des parametres sont definit au debut du fichier mais certains sont redefini apres en fonction de la simulation choisie.

Les scripts en bash sont la pour appeler gnuplot et dessiner les courbes.

## Compilation
make

## Purger le dossier:
make clean  (Attention: cette opération supprime aussi les dossiers simulation 1 et 2 avec les résultats)

## Execution de la simulation:
./remote 1 -> Pour executer la simulation 1
./remote 2 -> Pour executer la simulation 2

## Les Simulations:
### Simulation 1:Etude du système à energie constante:
On thermalise le systeme jusqu'a ce que la temperature de la simulation reste autour de la temperature attendue pendant un assez grand 
nombre d'itérations.
Puis on lasse évoluer la simulation et on commence à prendre des mesures: Histogrammes distribution radiale, vitesse, pression, position...

### Simulation 2: Etude des cristaux de l'Argon
On thermalise le systeme pendant un nombre d'itérations fixé (suffisement grand).
On descend ensuite la temperature de 5K en utilisant la fonction de thermalisation 1 seule fois. On attend quelques iterations.
On recommence à descendre la temperature de 5K, et ceci jusqu'à atteidre une température de 0.1K.
Une fois cette température atteinte on laisse le système évoluer et on enregistre la position des particule ainsi que l'énergie de la structure cristalline.
On repete cette operation un grand nombre de fois avec des positions initiales differentes et on classe les structure par ordre d'energie decroissante dans le fichier energie_final_classement.txt

## Description des différents fichiers:
toolbox.h/.cpp: Fichiers contenant toutes les fonctions permettant de faire des mesures ainsi que quelques petites fonctions utiles
particule.h/.cpp: fichiers contenant la classe Particule, définissant chaque particules comme un objet.
world.h.cpp: Fichiers contenant la classe World, définissant l'environement de la simulation comme un objet.
vecteur.h/.cpp: Fichiers contennt la classe vecteur.
defines.h/.cpp: Fichiers contenant toutes les varibles globales de la simulation.



