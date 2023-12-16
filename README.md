# Projet RISC-V CS351

Auteurs : Rémi MAZZONE et Calista ROUX

## Rendu 1

* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :
  - [x] Vérifié que `make test` se plaint sur le nombre d'instructions et pas
      sur l'existence des fichiers de sortie.
  - [x] Vu sur Chamilo que les soumissions se font avec `make tar`.


## Rendu 2

* Comment avez-vous choisi de programmer l'analyse de texte (dont la lecture
des opérandes entières) ? Comment identifiez-vous les noms de registres, des noms "jolis", des entiers ?

  - Pour l'analyse du texte, nous lisons lignes par lignes. Nous avons la fonction `parse_string` qui vient enlever les virgules, les parenthèses et qui vient mettre chaque élément de notre ligne dans un tableau. Par exemple, si nous avons `add a0, a1, a2`, la fonction nous donne un tableau avec `["add", "a0, "a1", "a2"]`.
  
  - Pour l'identification des noms des registres, nous avons, dans notre fichier [const.h](assembler/const.h#L15), fait un tableau (`registres`) avec tous les noms "jolis" des registres. Nous avons créé la fonction `find_register` qui va comparer ce tableau avec notre nom de registre ou simplement le comparer avec la chaine de caractère des noms de registre.
  
  - Pour la différenciation des noms de registres des valeurs immédiates, nous avons créé le tableau `instr_format` qui nous permet de savoir, selon le type d'instruction, les différents éléments qu'elle contient. Par conséquent, on connaît le nombre de registres que l'on a ainsi que leur place et de même pour les valeurs immédiates avec le nombre de "séparation", leur taille... 


* Avez-vous vu des motifs récurrents émerger ? Avez-vous "factorisé" ces motifs
pour éviter de les répéter ? Si non, serait-ce difficile ?

  - Oui nous avons remarqué des motifs récurrents. Nous pouvons citer quelques exemples comme l'`opcode` ou le `funct3`  qui sont au même endroit pour 4 types d'instructions sur 5 (cf. [function.c - l.152](assembler/function.c#L152)). Cependant ce sont les seules instructions que nous avons gérées de manière séparée.

  - Cependant, le tableau mentionné précédemment (`instr_format`) contient une configuration complète des formats d'instructions. En parcourant ce tableau on positionne donc tous les éléments directement au bon endroit. Nous avons donc une factorisation du processus car la valeur hexa peut être trouvée en _quasiment_ une seule boucle `for`.

* Comment avez-vous procédé pour écrire les tests ? Étes-vous confiant·e·s que
toutes les instructions gérées et tous les types d'arguments sont couverts ?

  - Pour nos tests, nous avons créé deux fichiers tests : [full-test.s](assembler/full-test.s) et [.error_gestion.s](assembler/.error_gestion.s). Dans [full-test.s](assembler/full-test.s), nous avons tous les tests possibles et **compilables**. Cela, nous permet de voir si toutes les instructions fonctionnent. Nous avons aussi créé des instructions avec des valeurs immédiates négatives.
  
  - Pour la gestion des erreurs comme : la mauvaise écriture des instructions, des noms de registres invalides ou à la mauvaise place; ces tests se trouvent dans le fichier [.error_gestion.s](assembler/.error_gestion.s) (qui commence par un `.` de manière à ne pas être pris en compte par le `make test`).
  
  - Pour savoir si nos tests fonctionnent, nous les avons entré sur [ce site](https://luplab.gitlab.io/rvcodecjs/) qui permet de connaître les valeurs hexadécimales et binaires des instructions. Nous avons stocké les valeurs dans le fichier en question en tant que commentaire. Nous les comparons ensuite avec nos résultats.

* Quelle a été votre expérience avec l'utilisation et la compréhension de la
documentation fournie sur l'encodage des instructions RISC-V ?

  - Notre expérience avec la documentation fournie a été plutôt bonne. Comme toute documentation, il est nécessaire de prendre un temps pour comprendre comment elle fonctionne mais nous avons trouvé qu'elle était plutôt claire et bien expliquée.

  - Le principe n'est pas très compliqué : il suffit de placer les bons éléments au bon endroit ! La seule implémentation qui a demandé un peu plus de réflexion a été pour les valeurs immédiates car les bits sont séparés.

  - Cependant cela n'a pas été longtemps une difficulté car grâce à notre fonction `write_output`, nous pouvons écrire un nombre de bits choisi à l'endroit où l'on souhaite parmi les 32. Tout repose donc sur de la configuration, c'est une des choses qui a été le plus long à mettre en place car il fallait que ça soit assez général mais pour autant spécifique, afin que tous les cas possibles soit traité. On a donc bien réfléchi afin de créer nos tableaux de la manière la plus efficace à notre sens.
   
* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :
  - [X] Implémenté la traduction pour toutes les instructions de la documentation
  - [X] Pris en compte les cas particuliers comme les valeurs immédiates négatives et le bit manquant dans l'encodage de `jal`
  - [X] Écrit du code de gestion d'erreur pour que le programme ait une réaction propre si le code assembleur est invalide _(pas demandé par le sujet)_

<!-- 
## Rendu 3

Questions à remplir _avant_ de programmer l'émulateur (10 lignes sont conseillées à chaque fois pour bien y réfléchir) :

* Listez tous les éléments matériels auxquels vous pouvez penser dont l'émulateur doit reproduire le comportement, et déduisez-en une liste de toutes les tâches individuelles de l'émulateur.

[COMPLÉTER ICI]

* Quelle fonction de la bibliothèque standard pouvez-vous utiliser pour lire les valeurs listées dans le fichier `.hex` sans vous casser la tête ? (Indice : ces valeurs ont été écrites avec `fprintf()`.)

[COMPLÉTER ICI]

* Décrivez comment vous allez répartir les tâches de l'émulateur en différents fichiers, ou ne pas les répartir et tout faire dans le même fichier. Expliquez les avantages de votre choix.

[COMPLÉTER ICI]

Questions à remplir _après_ avoir programmé l'émulateur :

* Aviez-vous réussi à listé toutes les tâches dans la première question ? Rétrospectivement, y a-t-il des tâches dont vous aviez sous-estimé ou sur-estimé la complexité ?

[COMPLÉTER ICI]

* Avez-vous compris le fonctionnement de chaque instruction à partir de la
documentation fournie ? Si non, quels sont les points obscurs ?

[COMPLÉTER ICI]

* Quels exemples de programmes avez-vous choisi pour tester le calcul ? Les
comparaisons et sauts ? La mémoire ?

[COMPLÉTER ICI]

* Reste-t-il des bugs que vous avez découverts et pas corrigés ?

[COMPLÉTER ICI]

* D'autres remarques sur votre programme ?

[COMPLÉTER ICI]

* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :**
  - [ ] Implémenté l'émulation de toutes les instructions gérées par le rendu 2.
  - [ ] Implémenté l'émulation de toutes les instructions.
  - [ ] Tous vos tests qui passent.
  - [ ] Vérifié que vous tests couvrent toutes les instructions émulées.
  - [ ] Testé les cas particuliers : valeurs négatives, overflows...
  - [ ] Testé les cas d'erreur : division par zéro, sauts invalides... _(pas demandé par le sujet)_
  - [ ] Un port fonctionnel de DOOM pour votre émulateur.

* Des retours sur le projet en général ?

[COMPLÉTER ICI]
-->