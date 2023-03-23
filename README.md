# Robminton

Le projet a pour but de répondre à l'appel d'offre fait par la FFB (Fédération Française de Badminton). Il est composé de 2 parties, un robot ramasseur de volants ainsi qu'un robot lanceur de volants. Même si ce genre de produits existe déjà sur le marché (surtout côté lanceur) le but est quand même de produire un robot plus performant, open source et à moindre coût. 

Concernant les logiciels à avoir, il est nécessaire d'avoir SolidWorks afin d'utiliser au mieux les fichiers SW des pièces que nous avons modélisés. Il est aussi recommandé d'avoir un IDE (type CodeBlocks) pour tout ce qui est programme en C (calculatrice de paramètres du lanceur). Posséder une machine virtuel avec ROS et un IDE pour la programmation sur ST32 (type CubeIDE). 

Toutes les ressources que nous avons utilisées/produites sont présentes dans le git :
00_doc : contient toute la documentation nécessaire pour bien commencer le projet
01_mdl : contient les modélisation 3D que nous avons faites au cours du projet. Dedans ce sera le dossier SW qui sera le plus utile. Toutes les pièces/assemblages y sont. 
02_src : contient tout les codes sources utilisés pour faire fonctionner les moteurs présents dans nos robots. 
03_elec : contient de la documentation détaillée de toute l'électronique que nous utilisons que ce soit les cartes embarquées ou bien les moteurs. 

Nous avons fait en sorte que le projet parte sur de bonnes bases afin que les prochains groupes puissent travailler sur la partie plus intéressante du projet c'est à dire faire des tests. La modélisation du châssis du robot ramasseur est faite, il ne manque que quelques découpes à faire et des équerres à commander. Les moteurs fonctionnent avec le code que nous avons mis sur le Git il suffira de gérer les 4 moteurs en même temps. Il faudra cependant trouver une solution pour que le robot puisse ramasser tout les volants (plusieurs centaines de volants) rapidment donc sûrement revoir le système de stockage. Niveau lanceur toute la partie théorique à été faite et donc ne sera plus à refaire. Un dispositif pour le lanceur a été mis en place il ne reste qu'à l'implémenter sur une structure (en profilé), un croquis est disponible dans 00_src pour mieux comprendre. 

Equipe : AMADO Daniel, BERBEROVIC Amon, LIENG Antoine, PUSZYNSKI Hugo, VACHEROT Tristan
