Integrantes: Jaime Rodríguez 201704072-6 Alejandro Fernández 20170450-3
Ejecución: make.uno
1 Entrega final: Finalmente se procedió a crear 3 procesos hijos utilizando fork, y establecer una comunicación
entre los procesos (padre e hijos) con los pipes, donde se creo un pipe por cada hijo. Básicamente el padre le envía un mensaje al hijo y el hijo a otro, 
y así sucecivamente generando un ciclo, donde la comunicación entre procesos respeta un sentido dado entre ellos.
Luego se procedió a relacionar los procesos hijos y el padre con los avances 1 y 2 (navegación entre carpetas y lógica del juego),
para poder aplicar el ciclo generado por los fork y los pipe con la lógica del juego.
La gran dificultad de esta parte fue poder relacionar los procesos hijos con la lógica del juego, ya que hacer que los hijos respeten la lógica del juego,
y poder comunicarse entre ellos mientras el juego se esta ejecutando, es sumámente difícil.

2.Consideraciones: No es recomendable revisar el código por malas practicas de poner todo el código junto :(
