# Transformation XOR d’une image RAW en C

Prérequis : GCC (ou un compilateur C11) et Python 3 pour les tests.

```sh
gcc -std=c11 -Wall -Wextra -Werror main.c -o image
./image source.raw sortie.raw 256 256 42
./image sortie.raw original.raw 256 256 42
python3 -m unittest discover -s tests -v
```

Le fichier contient exactement hauteur × largeur octets (un octet par pixel). Sans argument, le programme demande les paramètres. Le même XOR permet de récupérer l’original. Les dimensions et la clé sont contrôlées ; une erreur de lecture ou d’écriture entraîne un code de sortie non nul. Limite : 100 millions de pixels. Il s’agit d’un exercice XOR à clé d’un octet, pas d’un chiffrement sûr.

Les tests vérifient l’aller-retour binaire et les erreurs de fichier, dimensions, clé et destination.

## Validation

Les tests couvrent les scénarios décrits ci-dessous. Ils vérifient la version corrigée ; ils ne garantissent pas tous les usages possibles.
