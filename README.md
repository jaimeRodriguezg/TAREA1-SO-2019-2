Integrantes: Jaime Rodr�guez 201704072-6 Alejandro Fern�ndez 20170450-3
Ejecuci�n: make.uno
1 Entrega final: Finalmente se procedi� a crear 3 procesos hijos utilizando fork, y establecer una comunicaci�n
entre los procesos (padre e hijos) con los pipes, donde se creo un pipe por cada hijo. B�sicamente el padre le env�a un mensaje al hijo y el hijo a otro, 
y as� sucecivamente generando un ciclo, donde la comunicaci�n entre procesos respeta un sentido dado entre ellos.
Luego se procedi� a relacionar los procesos hijos y el padre con los avances 1 y 2 (navegaci�n entre carpetas y l�gica del juego),
para poder aplicar el ciclo generado por los fork y los pipe con la l�gica del juego.
La gran dificultad de esta parte fue poder relacionar los procesos hijos con la l�gica del juego, ya que hacer que los hijos respeten la l�gica del juego,
y poder comunicarse entre ellos mientras el juego se esta ejecutando, es sum�mente dif�cil.

2.Consideraciones: No es recomendable revisar el c�digo por malas practicas de poner todo el c�digo junto :(
