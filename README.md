The purpose of this project is for testing my neural network library ([LearNN](https://github.com/VynDev/LearNN)) and my basic 2D environment based engine ([EnvGine](https://github.com/VynDev/EnvGine)), they're both in developement and for learning purpose so their version in this repo might be outdated.  

# Introduction

![Snake](https://github.com/VynDev/Fruit-Seeker/blob/main/Images/FruitSeeker.png)

Fruit Seeker is basically a *Snake* game, but the snake can't grow. Why ? Because I needed to make is simplier so the AI learns faster.  
There is 2 modes: **Normal** and **Genetic Algorithm**, I won't go through the first one because everybody knows how Snake works.

# Genetic Algorithm

![Snake - Genetic Algorithm](https://github.com/VynDev/Fruit-Seeker/blob/main/Images/GeneticAlgorithm2.gif)

We have a **population of 1000 AI** (but you can see only the best of them on the gif above).  
They all play with the same map/fruit configuration, it means that the position where the fruits will appear is the same
for each environment and each game, so there is no "lucky win".  

Once all games are finished, the snakes will reproduce between themself and the children will take the place of the worst snakes, we now have a new generation.  
Let's see how how everythings work.

## Snake environment detection

![Snake](https://github.com/VynDev/Fruit-Seeker/blob/main/Images/FruitSeeker-Sensors.png)

The snake has 8 sensors:

- 4 sensors for the distance between the walls.
- 4 sensors for the distance from the fruit, or a huge value if the fruit isn't in the direction of the sensor.

## Score

The snake wins 1 point per fruit + a small amount of point (like 0.00001) for the travel time.  
Giving a small amount of point for the snake's travel time allows us to find more "solution" as the snake has
more chance to have the fruit in his sensors.

## Reproduction

In the scenario, **40%** of the current population will be replaced by the "elites", it means that the best snakes will crossover between themself, then **35%**  will be replaced by random crossover, any snake has a chances, the remaining snakes will be replaced by newly randomized snakes.  
Of course the best snakes (elite) will stay in the next generation to prevent regression.

### Crossover

Here's how 2 snakes crossover: for each weight of the neural network, the child has 50% chance to get one of the "mother" and 50% to get of the "father", then there is between 1% and 2% chnce to have a mutation of the weight (basically, randomized or shifted).
