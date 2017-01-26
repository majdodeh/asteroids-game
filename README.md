LIFAMI - Applications en mathématiques et informatique
Université Claude Bernard Lyon 1

ODEH Majd

Asteroids Game
Made using Grapic 0.4.33
http://liris.cnrs.fr/alexandre.meyer/grapic/html/index.html
<img src="https://raw.githubusercontent.com/majdodeh/asteroids-game/master/Screenshots/00_Start_screen.png">
<img src="https://raw.githubusercontent.com/majdodeh/asteroids-game/master/Screenshots/01_Levels_screen.png">
<img src="https://raw.githubusercontent.com/majdodeh/asteroids-game/master/Screenshots/02_Game_screen.png">
<img src="https://raw.githubusercontent.com/majdodeh/asteroids-game/master/Screenshots/03_Game_over_screen.png">
------------------------------------------------------------

Game principle

The Asteroids game is composed of a saucer at the bottom of the screen, Asteroids in movements in the middle of the screen and a moon at the top of the screen.
The goal of Asteroids game is control the flying saucer to reach the moon at the top of the screen avoiding the crash into an asteroid in the asteroids field.
When the game starts, the score is 0 and the player has 3 lives, if the saucer reached the moon the player get 1 score point and if the saucer crashed into an asteroid the player will lose 1 life, and if the player run out of lives then the game will end. 

------------------------------------------------------------

How it works

To play this game you need to use the mouse (mouse position and mouse left click), you can control the saucer by moving mouse position and when you click on mouse left click you give the saucer a force toward mouse position (a vector between the mouse position and the saucer position), and while you are not clicking left mouse click the gravity force toward the bottom is applied.

------------------------------------------------------------

Forces applied to the flying saucer

1. Gravity force toward bottom of game screen.
2.  Asteroids gravitational force (related with asteroid's radius).
3. Force toward mouse position (while clicking mouse left click and it's related with the distance between mouse position and saucer position).

------------------------------------------------------------

Asteroids

Asteroids move in a random directions (left and right) and each asteroid has a random position and radius, these variables are given when the World is initialized.
Each time an asteroid cross the screen border (left and right), it gets a new position and radius.

------------------------------------------------------------

Game screens & controls

Asteroids game contains 4 different screens:
1. Game welcome screen: (Press SPACE to skip this screen).
2. Levels screen: there are 3 game levels (Press A for Easy Mode) (Press B for Medium Mode) (Press C for Hard Mode).
3. Game screen: (use mouse position and mouse left click).
4. Game over screen: (Press SPACE to restart the game) (Press Q to quit the game).

------------------------------------------------------------

Images

I used 7 images (.PNG) in this game:

asteroid.png
background.png
explosion.png
moon.png
soucoupe.png
soucoupe_feu.png
star.png

------------------------------------------------------------

Libraries

- Grapic.h
- string.h
- time.h
- math.h
- sstream