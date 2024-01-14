import sys
import os
import numpy as np
import matplotlib.pyplot as plt

MAP_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'map/map.npy')

### START CODE HERE ###
# This code block is optional. You can define your utility function and class in this block if necessary.

# Get neighbors of a point
def neighbors(pos):
    if pos[0] == 0:
        if pos[1] == 0:
            nlist = [[pos[0]+1, pos[1]], [pos[0], pos[1]+1]]
        elif pos[1] == 119:
            nlist = [[pos[0]+1, pos[1]], [pos[0], pos[1]-1]]
        else:
            nlist = [[pos[0]+1, pos[1]], [pos[0], pos[1]+1], [pos[0], pos[1]-1]]

    elif pos[0] == 119:
        if pos[1] == 0:
            nlist = [[pos[0]-1, pos[1]], [pos[0], pos[1]+1]]
        elif pos[1] == 119:
            nlist = [[pos[0]-1, pos[1]], [pos[0], pos[1]-1]]
        else:
            nlist = [[pos[0]-1, pos[1]], [pos[0], pos[1]+1], [pos[0], pos[1]-1]]

    else:
        if pos[1] == 0:
            nlist = [[pos[0]+1, pos[1]], [pos[0]-1, pos[1]], [pos[0], pos[1]+1]]
        elif pos[1] == 119:
            nlist = [[pos[0]+1, pos[1]], [pos[0]-1, pos[1]], [pos[0], pos[1]-1]]
        else:
            nlist = [[pos[0]+1, pos[1]], [pos[0]-1, pos[1]], [pos[0], pos[1]+1], [pos[0], pos[1]-1]]

    return nlist

# Get index of a point if exists
def point_in_array(point, array):
    for i in array:
        if i[0] == point:
            return array.index(i)
    return -1

# Greedy function
def h(pos):
    return abs(100-pos[0])+abs(100-pos[1])

###  END CODE HERE  ###


def A_star(world_map, start_pos, goal_pos):
    """
    Given map of the world, start position of the robot and the position of the goal, 
    plan a path from start position to the goal using A* algorithm.

    Arguments:
    world_map -- A 120*120 array indicating map, where 0 indicating traversable and 1 indicating obstacles.
    start_pos -- A 2D vector indicating the start position of the robot.
    goal_pos -- A 2D vector indicating the position of the goal.

    Return:
    path -- A N*2 array representing the planned path by A* algorithm.
    """

    ### START CODE HERE ###
    open = [[start_pos, 180]]
    closed = []
    path = [goal_pos]
    
    costmap = np.full((120, 120), 1000, dtype=int)
    parentmap = np.full((120, 120, 2), [0, 0], dtype=int)
    costmap[start_pos[0]][start_pos[1]] = 0
    parentmap[start_pos[0]][start_pos[1]] = [-1, -1]

    while 1:
        current = open.pop(0)[0]
        if current == goal_pos:
            break
        closed.append(current)

        for n in neighbors(current):
            if world_map[n[0]][n[1]] == 1:
                continue
            else:
                cost = costmap[current[0]][current[1]] + 1
                flag = point_in_array(n, open)
                if (flag >= 0) and (cost < costmap[n[0]][n[1]]):
                    open.pop(flag)
                if (n in closed) and (cost < costmap[n[0]][n[1]]):
                    closed.remove(n)
                
                if (n not in closed) and (flag == -1):
                    costmap[n[0]][n[1]] = cost
                    open.append([n, cost + h(n)])
                    open.sort(key=lambda x:x[1])
                    parentmap[n[0]][n[1]] = current
                    
    for i in path:
        last = parentmap[i[0]][i[1]]
        if last[0] == -1 and last[1] == -1:
            break
        path.append(parentmap[i[0]][i[1]])

    ###  END CODE HERE  ###
    return path





if __name__ == '__main__':

    # Get the map of the world representing in a 120*120 array, where 0 indicating traversable and 1 indicating obstacles.
    map = np.load(MAP_PATH)

    # Define goal position of the exploration
    goal_pos = [100, 100]

    # Define start position of the robot.
    start_pos = [10, 10]

    # Plan a path based on map from start position of the robot to the goal.
    path = A_star(map, start_pos, goal_pos)

    # Visualize the map and path.
    obstacles_x, obstacles_y = [], []
    for i in range(120):
        for j in range(120):
            if map[i][j] == 1:
                obstacles_x.append(i)
                obstacles_y.append(j)

    path_x, path_y = [], []
    for path_node in path:
        path_x.append(path_node[0])
        path_y.append(path_node[1])

    plt.plot(path_x, path_y, "-r")
    plt.plot(start_pos[0], start_pos[1], "xr")
    plt.plot(goal_pos[0], goal_pos[1], "xb")
    plt.plot(obstacles_x, obstacles_y, ".k")
    plt.grid(True)
    plt.axis("equal")
    plt.show()