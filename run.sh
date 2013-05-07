unbuffer ./classify_sound.out | java -cp VowelToSokobanSignal/bin/ VowelParser | java -cp Sokoban/bin game.Sokoban Sokoban level6.sokoban
