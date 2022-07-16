# ButtonRace
*Learning numbers for the very small - fast paced arcade action for the others*

This version was built upon another game available on the web which I tried in the first place:

The basic design is a board with five buttons each for two players. When the game starts a single random button is lit which is then intended to be hit by the player. Once pressed another button is randomised/lit by the µC and so on. Each correct press of a button increases the player’s score by one. The player who first reaches `maxscore` wins the game.

I got inspired to extent the basic game with additional features. The game itself is great for spending evenings with friends with a glass of wine or two. Once I saw my 2 1/2 year old girl playing with the prototype I had the idea why not to create an additional game mode for learning the numbers from 1 to 10, together with a friend. Then it began.

Current features are:

Standard mode: 
*2 player game from 0 to 100
*penalty of -1 when wrong button was pressed
*no limits in speed or rounds
*true random numbers

Kid mode: 
*2 player game from 1 to 10
*no penalty
