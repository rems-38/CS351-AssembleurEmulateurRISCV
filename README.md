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

## Rendu 3

* Listez tous les éléments matériels auxquels vous pouvez penser dont l'émulateur doit reproduire le comportement, et déduisez-en une liste de toutes les tâches individuelles de l'émulateur.

  - Processeur (CPU) :
    * Décodez et exécutez chaque instruction RISC-V.
    * Mettez à jour les registres (x0 à x31) en fonction du résultat de l'instruction.
    * Gérez le PC (Program Counter) pour passer à l'instruction suivante.

  - Mémoire :
    * Gérez l'espace d'adressage de la mémoire.
    * Chargez le programme en mémoire à partir du fichier hexadécimal.
    * Permettez la lecture et l'écriture dans la mémoire.

  - Registres :
    * Initialisez les registres avec des valeurs par défaut.
    * Mettez à jour les registres en fonction des instructions exécutées.
    
  - Gestionnaire d'exceptions et de gestion des erreurs :
    * Gérez les exceptions et les erreurs, par exemple les divisions par zéro, les dépassements de capacité, etc.
    
* Quelle fonction de la bibliothèque standard pouvez-vous utiliser pour lire les valeurs listées dans le fichier `.hex` sans vous casser la tête ? (Indice : ces valeurs ont été écrites avec `fprintf()`.)

  - Pour ne pas nous casser la tête, nous pouvons directement utiliser `fscanf()`, cela nous permet de récupérer directement les blocs de données qui nous intéresse en les séparant comme il faut. C'est ce qui nous semble le plus logique au vu du fait qu'on écrit avec un `fprintf()` avec un format spécifique, ici `%08x`.

* Décrivez comment vous allez répartir les tâches de l'émulateur en différents fichiers, ou ne pas les répartir et tout faire dans le même fichier. Expliquez les avantages de votre choix.

  - Nous considérons 2 tâches principales pour l'émulateur :
    * l'exécution globale du programme;
    * l'exécution d'une instruction
  
    Dans notre fichier [function.c](emulator/function.c) nous allons donc coder ces deux fonctions `execute_instr` et `emulate_prog`. Nous séparons ceci du fichier [main.c](emulator/main.c) afin de ne pas le surcharger.

* Aviez-vous réussi à listé toutes les tâches dans la première question ? Rétrospectivement, y a-t-il des tâches dont vous aviez sous-estimé ou sur-estimé la complexité ?

  - Pour les tâches listées lors de la première question, nous trouvons que nous les avons à peu près toutes écrites. Il y a seulement le gestionnaire d'erreur que nous n'avons pas fait.
  
  - Pour ce qui concerne la complexité de celles-ci, nous savions que certaines tâches seraient plus simples que d'autre mais en globalité, nous avons fait le boulot assez facilité.


* Avez-vous compris le fonctionnement de chaque instruction à partir de la
documentation fournie ? Si non, quels sont les points obscurs ?

  - Oui, toutes les instructions avaient déjà bien été comprise lors de la partie assembleur mais nous en avons eu ici un regard différent car avant, nous ne nous étions pas concentré sur ce qu'elle faisait vraiment.
  
  - Les instructions dont nous avons eu le plus de mal à comprendre (même si ça ne nous a pas été trop compliqué non plus), ce sont les instructions `ld`/`sd`. Nous avons eu des doutes sur comment réaliser les accès en mémoire car nous avions peur d'avoir des conflits avec notre programme en lui-même, qui se trouve également dans la mémoire. Finalement, nous avons décidé que ça relevait de la responsabilité de l'utilisateur quand il écrirait son programme.

* Quels exemples de programmes avez-vous choisi pour tester le calcul ? Les
comparaisons et sauts ? La mémoire ?

  - Pour le calcul, nous avons fait des programmes simples d'addition, soustractions etc...

  - Pour tester les comparaisons et les sauts, nous avons fait des petites boucles, des programmes basique qui répètent une même opération en décrémentant une valeur, tant que cette valeuur ne vérifie pas une condition on continue.

  - Et enfin, pour la mémoire, nous avons simplement fait du stockage de registres dans la pile, en modifiant pour la suite le registre sauvegardé et en récupérant sa valeur initiale, pour montrer que la sauvegarde en mémoire fonctionne correctement.

* Reste-t-il des bugs que vous avez découverts et pas corrigés ?

  - A priori, nous n'avons pas trouvé de problème apparent. Nous avons réaliser différents tests qui nous permettent de tester toutes les instructions demandées. Nos tests reviennent tous justes.

* D'autres remarques sur votre programme ?

  - Nous avons pensé à optimiser notre code. En effet, on peut voir que lors de la lecture des instructions, nous venons à chaque fois la traduire en "Instruction". Nous avons remarqué que lorsque nous entrons dans une boucle, notre programme va venir traduire plusieurs fois une même instruction. Nous avons donc pensé que nous pourrions optimiser en créer un tableau qui garde le programme en traduction "Instruction" ce qui permettrait de ne pas perdre de temps à chaque boucle.

* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :**
  - [x] Implémenté l'émulation de toutes les instructions gérées par le rendu 2.
  - [x] Implémenté l'émulation de toutes les instructions.
  - [x] Tous vos tests qui passent.
  - [x] Vérifié que vous tests couvrent toutes les instructions émulées.
  - [x] Testé les cas particuliers : valeurs négatives, overflows...
  - [x] Testé les cas d'erreur : division par zéro, sauts invalides... _(pas demandé par le sujet)_
  - [ ] Un port fonctionnel de DOOM pour votre émulateur.

* Des retours sur le projet en général ?

  - C'est un projet que nous trouvons très intéressant. En effet, il nous permet de faire un lien entre nos différents cours. Cela rajoute de l'intérêt pour celui-ci. 
  
  - Le petit hic que nous pouvons relevé est le manque de claireté et de précision du sujet. Nous avons du comprendre certains détails seuls ou avec l'aide d'autres binômes. 

  - Nous sommes un peu frustré du peu d'instructions que nous avons dû implémenter. En effet, quand on veux tester des programmes, nous pouvons effectuer uniquement des tâches basiques et c'est donc un peu frustrant. Cependant, ça donne encore plus envie de continuer ce projet après le rendu 3, afin d'implémenter plus de choses !