BACKPROPAGATION

Compilar:

g++ -std=c++11 perceptron_multilayer.cpp

Ejecutar:

./a.out <n_inputs> <n_outputs> dataset/and.dat <layer_1> <function_1> <layer_2> <function_2> ... <layer_n> <function_n>

Donde:
- n_inputs es el numero de caracteristicas de entrada
- n_outputs es el numero de caracteristicas de salida
- layer_n es el numero de neuronas de la capa n
- function_n es la funcion de la capa n (valores 0, 1)  // 0 -> sigmoidal, 1 -> tangente, etc

./a.out 2 1 dataset/xor.dat 3 0 1 0


Salidas en consola:

Al terminar de entrenar se muestra los pesos por cada capa, mas las salidad de cada una de ellas, y el numero de epocas necesarias para el entrenamiento de XOR.

...

 >> error: 0.10393668996599197

 >> error: 0.10323454073130127

 >> error: 0.10254036219859233

 >> error: 0.1018540378604996

 >> error: 0.10117545265712716

 >> error: 0.10050449299748151

 >> error: 0.09984104677754459

-- Weights --
Layer 1
Neuron 1
1.1434905408166254 4.856676853251187 -2.4980977333077874
Neuron 2
-1.1142972330587033 -0.10328510936015553 2.901247084692144
Neuron 3
-0.9091349688767458 5.527397580712898 4.218284658392803
Layer 2
Neuron 1
0.003014760770455052 -3.682020182272181 -3.5243473579007625 5.525670105968346

-- Outs --
0.11182311376932376 0.8241350871761415 0.733129784303966
0.2915981913418381

 EPOCH: 1289


STARCRAFT CONFIGURATION