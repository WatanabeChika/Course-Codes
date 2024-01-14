# -*- coding:utf-8 -*-
import math, os, time, sys
import numpy as np
import gym
##### START CODING HERE #####
# This code block is optional. You can import other libraries or define your utility functions if necessary.

##### END CODING HERE #####

# ------------------------------------------------------------------------------------------- #

class SarsaAgent(object):
    ##### START CODING HERE #####
    def __init__(self, all_actions):
        """initialize the agent. Maybe more function inputs are needed."""
        self.all_actions = all_actions
        self.epsilon = 1
        # learning rate
        self.alpha = 0.3
        # reward decay
        self.gamma = 0.9
        # epsilon decay schema
        self.epsilon_decay = 0.999
        # q_table
        self.q_table = np.zeros((48, 4))

    def choose_action(self, observation):
        """choose action with epsilon-greedy algorithm."""
        if np.random.uniform() < self.epsilon:
            action = np.random.choice(self.all_actions)
        else:
            action = np.argmax(self.q_table[observation])
        return action
    
    def learn(self, r, s, a, s_, a_):
        """learn from experience"""
        # update epsilon
        self.epsilon *= self.epsilon_decay
        # update q_table
        q_origin = self.q_table[s][a]
        q_perdict = self.q_table[s_][a_]
        self.q_table[s][a] += self.alpha * (r + self.gamma * q_perdict - q_origin)
        return False
    
    def your_function(self, params):
        """You can add other functions as you wish."""
        return None

    ##### END CODING HERE #####


class QLearningAgent(object):
    ##### START CODING HERE #####
    def __init__(self, all_actions):
        """initialize the agent. Maybe more function inputs are needed."""
        self.all_actions = all_actions
        self.epsilon = 1
        # learning rate
        self.alpha = 0.3
        # reward decay
        self.gamma = 0.9
        # epsilon decay schema
        self.epsilon_decay = 0.999
        # q_table
        self.q_table = np.zeros((48, 4))

    def choose_action(self, observation):
        """choose action with epsilon-greedy algorithm."""
        if np.random.uniform() < self.epsilon:
            action = np.random.choice(self.all_actions)
        else:
            action = np.argmax(self.q_table[observation])
        return action
    
    def learn(self, r, s, a, s_):
        """learn from experience"""
        # update epsilon
        self.epsilon *= self.epsilon_decay
        # update q_table
        q_origin = self.q_table[s][a]
        q_perdict = np.max(self.q_table[s_])
        self.q_table[s][a] += self.alpha * (r + self.gamma * q_perdict - q_origin)
        return False
    
    def your_function(self, params):
        """You can add other functions as you wish."""
        return None

    ##### END CODING HERE #####
