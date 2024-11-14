# Projet_C_ING1

## Instructions pour tester le code dans le terminal Windows

**Il faut avoir installer le compilateur de code C, MinGW**


1 - Accédez au dossier du projet

Ouvrez l'Explorateur de fichiers et allez dans le répertoire où vous avez stocké le projet.

2 - Ouvrir le terminal

Cliquez sur la barre d'adresse dans l'explorateur de fichiers, tapez cmd, puis appuyez sur Entrée. Cela ouvrira le terminal dans le dossier actuel.

3 -Compiler le code source

Dans le terminal, tapez la commande suivante pour compiler les fichiers source :

 ```bash
Copier le code
gcc -o Projet.exe traductor.c draw_functions.c
```

Cela génère un fichier exécutable Projet.exe à partir des fichiers traductor.c et draw_functions.c.

4 - Exécuter le programme

Une fois la compilation terminée, vous pouvez exécuter le programme en utilisant la commande suivante :

```bash
Copier le code
Projet.exe input.draw output.py

```
Cette commande lance le programme avec le fichier de test input.draw comme entrée et génère un fichier output.py.

5 - Vérifier les fichiers générés (optionnel)

Le fichier output.py sera créé dans le même répertoire. Vous pouvez l'ouvrir pour vérifier les résultats.

6 - Créer un environnement virtuel Python

Créez un environnement virtuel avec la commande suivante :

```bash
Copier le code
python -m venv venv
```
Cela va créer un dossier venv contenant l'environnement virtuel.

7 - Activer l'environnement virtuel

Pour activer l'environnement virtuel, utilisez la commande suivante selon votre système d'exploitation :

```bash
Copier le code
.\venv\Scripts\activate
```
Une fois l'environnement virtuel activé, le prompt du terminal changera pour indiquer que l'environnement virtuel est actif.

8 - Installer matplotlib

Avec l'environnement virtuel activé, vous pouvez installer matplotlib, une bibliothèque nécessaire pour afficher les graphiques dans output.py. Utilisez la commande suivante :

```bash
Copier le code
pip install matplotlib
```
9 - Exécuter le fichier output.py

Maintenant que matplotlib est installé, vous pouvez exécuter le fichier output.py en utilisant Python :

```bash
Copier le code
python output.py
````
Cela devrait exécuter le code Python généré et afficher  les graphiques.

