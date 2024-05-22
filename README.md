# Couverture-de-graphe-par-collection-de-graphe

Etant donné un graphe G (un arbre) et une famille F de chaînes,
on veut déterminer si une F-couverture existe et ensuite trouver une F-couverture 
de poids minimum.

## Pour quels graphes ?
Avec le programme python drawingraph.py, on peut dessiner le graphe que l'ont veut couvrir et les graphes de la famille.
Pour avoir de bons résultats, il faut dessiner un arbre et de multiples chaînes.
En écrivant 'make', le programme python se lance puis retranscrit les graphe en fichier textes permettant d'effectuer 
les algorithmes d'existence et d'optimisation.

## Existence
Dans parcours.c, on effectue un parcours en largeur sur l'arbre pour déterminer l'excentricité de chaque feuille.
Ensuite, on récupère l'excentricité minimale. Si la chaîne la plus courte de la famille F est de longueur
inférieure ou égale à l'excentricité minimale, alors l'arbre est F-couvrable.

## Optimisation de poids de la F-couverture
Dans parcours.c on définit plusieurs type de chaîne (type 1, type 2 et findlightest) qui sont repérées dans l'arbre 
grace à des parcours en largeur. Dans opti_arbre.c, on utilise ces types pour trouver une F-couverture minimale.
Cette méthode nous permet de tomber régulièrement sur une bonne solution.

## Rapport
Vous trouverez en pdf, le rapport avec des démonstrations et la définition des algorithmes utilisés.