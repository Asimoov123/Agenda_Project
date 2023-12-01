<p align="center"><a href="https://www.efrei.fr/"><img src="https://www.efrei.fr/wp-content/uploads/2022/01/LOGO_EFREI-PRINT_EFREI-WEB.png" alt="Logo de l'EFREI" width="60%" /></a></p>

<h1 align="center">Agenda_Project</h1>
<p align="center">TI301 – Algorithmique et structures de données 2 - Projet Gestion d’un agenda - Projet Efrei L2 ASSD2 2023-2024<br>Réalisé par BRAQUEMART Théo, DOS SANTOS Thomas et PREVOT Romain</p>


# Introduction

Le but de ce projet est de réaliser une application qui permet de gérer un agenda en utilisant une structure de données intermédiaire entre les listes chaînées et les arbres.
Cette structure de données permettra, dans le même temps, de :
1. Conserver un ordre entre ses éléments :<br>
Ainsi, on pourra comparer les éléments deux à deux, et déterminer lequel des deux doit être rangé « avant l’autre » dans cette structure de données.
2. Effectuer les opérations standard (insertion, recherche, suppression) avec une complexité qui reste faible, notamment plus faible qu’avec une liste simplement chaînée.<br>
L’idée est que cette structure permette d’implémenter des traitements utilisant la dichotomie, pour tirer profit du fait que les valeurs sont stockées suivant un critère de tri.
