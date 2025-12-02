*****Campus Navigation System Using Graphs*****

**Overview**
This campus navigation system helps users find the shortest route between campus buildings by using a graph-based map and Dijkstra’s algorithm.  
The program displays all building names and their connections (with distances in meters), and prompts the user to select source and destination by number.  
It then computes and displays step-by-step directions along the optimal route.

**Features**
1. Displays a numbered list of all campus buildings.
2. Shows a visual ASCII-art representation of all connections and their distances.
3. Prompts user for source and destination **by number** (easy and error-proof).
4. Calculates and displays the shortest path by actual distance using Dijkstra’s algorithm.
5. Provides step-by-step directions and the total travel distance.

**Data Structures Used**
1. **Graph (Adjacency List):** Represents campus map and connections.
2. **Linked List:** Used in the adjacency list nodes.
3. **Arrays:** For storing building names and pathfinding data.
4. **Structs:** For nodes and overall graph structure.

**Campus Map**
```
  Campus Map (schematic, not to scale):

  [Gate] ---- [Library] ---- [Lab] ---- [Cafeteria]
      \           |
      \          |
        [Admin] ---+

  Connections (distance in meters):
  Gate       <-> Library    : 100
  Gate       <-> Admin      : 200
  Library    <-> Lab        : 30
  Library    <-> Admin      : 50
  Lab        <-> Cafeteria  : 80
  Admin      <-> Cafeteria  : 120

```

**Sample Output**
```
  =========================================
          Campus Navigation System
  =========================================
  1. Display campus buildings
  2. Display campus map and connections
  3. Find shortest path between two locations
  4. Exit
  Enter your choice: 3

  Campus Locations (Buildings):
  1. Gate
  2. Library
  3. Admin
  4. Lab
  5. Cafeteria

  Enter the number for your start location: 1
  Enter the number for your end location: 5

  Directions to reach Cafeteria from Gate:
  1. Move from Gate to Library (100 meters)
  2. Move from Library to Lab (30 meters)
  3. Move from Lab to Cafeteria (80 meters)

  Total distance: 210 meters

```

**Author**
1. Harshitha G
2. Harshika N
3. Harshitha S
4. Haripriya S M
