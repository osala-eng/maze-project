# The Maze Project 

The goal of this project is to create a game in 3D using raycasting !

![Game](./assets/img1.gif)

# 3D Raycasting
3D raycasting is a technique used in computer graphics and game development to render 3D environments in real-time. It involves casting rays from a player's viewpoint and detecting intersections with objects in the environment to determine what is visible on the screen.

In 3D raycasting, the player's viewpoint is represented by a position in 3D space, and rays are cast from this position in a cone or pyramid shape to cover the player's field of view. The rays are then checked for intersections with objects in the environment, such as walls, floors, and ceilings.

When an intersection is detected, the distance from the player's position to the intersected object is calculated, and this distance is used to determine the height of the object on the screen. This allows the renderer to create the illusion of depth and perspective in a 3D environment, even though the screen is 2D.

3D raycasting was first popularized by the game "Wolfenstein 3D" in the early 1990s, and it has since been used in many other games, including "Doom" and "Quake". It is still used today in some games and simulations, although newer techniques such as ray tracing have largely replaced it in high-end applications.
