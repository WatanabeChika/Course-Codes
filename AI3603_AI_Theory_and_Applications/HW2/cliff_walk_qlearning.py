# -*- coding:utf-8 -*-
# Train Q-Learning in cliff-walking environment
import math, os, time, sys
import numpy as np
import random
import gym
from agent import QLearningAgent
import matplotlib.pyplot as plt
from matplotlib import animation
##### START CODING HERE #####
# This code block is optional. You can import other libraries or define your utility functions if necessary.

def gif_generate(frames):
    patch = plt.imshow(frames[0])
    plt.axis('off')
    def animate(i):
        patch.set_data(frames[i])

    anim = animation.FuncAnimation(plt.gcf(), animate, frames = len(frames), interval=1)
    anim.save('video/final_result_qlearning.mp4', writer='ffmpeg', fps=5)

##### END CODING HERE #####

# construct the environment
env = gym.make("CliffWalking-v0")
# get the size of action space 
num_actions = env.action_space.n
all_actions = np.arange(num_actions)
# set random seed and make the result reproducible
RANDOM_SEED = 0
env.seed(RANDOM_SEED)
random.seed(RANDOM_SEED) 
np.random.seed(RANDOM_SEED) 

##### START CODING HERE #####

# construct the intelligent agent.
agent = QLearningAgent(all_actions)

# variables for visualization
episode_rewards = []
episode_numbers = []
epsilon_values = []
frames = []

# start training
for episode in range(500):
    # record the reward in an episode
    episode_reward = 0
    # reset env
    s = env.reset()
    # render env. You can remove all render() to turn off the GUI to accelerate training.
    env.render()
    # agent interacts with the environment
    for iter in range(500):
        # choose an action
        a = agent.choose_action(s)
        s_, r, isdone, info = env.step(a)
        env.render()
        # note the track in the last episode
        if episode == 499:
            frames.append(env.render(mode = 'rgb_array'))
        # update the episode reward
        episode_reward += r
        print(f"{s} {a} {s_} {r} {isdone}")
        # agent learns from experience
        agent.learn(r, s, a, s_)
        s = s_
        if isdone:
            #time.sleep(0.1)
            break
    # generate the gif of track movement
    if episode == 499:
        gif_generate(frames)
    # note the visualization variables
    if episode_reward > -300:
        episode_rewards.append(episode_reward)
        episode_numbers.append(episode)
        epsilon_values.append(agent.epsilon)
    if episode % 100 == 0:
        print('episode:', episode, 'episode_reward:', episode_reward, 'epsilon:', agent.epsilon)  
print('\ntraining over\n')   

# close the render window after training.
env.close()

# Plot the episode reward during the training process.
fig, ax1 = plt.subplots(figsize=(8, 4))
ax1.plot(episode_numbers, episode_rewards, label='Episode Reward', color='tab:blue')
ax1.set_xlabel('Episode')
ax1.set_ylabel('Episode Reward', color='tab:blue')
ax1.tick_params(axis='y', labelcolor='tab:blue')

ax2 = ax1.twinx()
ax2.plot(episode_numbers, epsilon_values, label='Epsilon Value', color='tab:orange')
ax2.set_ylabel('Epsilon Value', color='tab:orange')
ax2.tick_params(axis='y', labelcolor='tab:orange')

plt.title('Episode Reward And Epsilon Value During Training')
lines, labels = ax1.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
ax2.legend(lines + lines2, labels + labels2, loc='center right')

plt.grid()
plt.tight_layout()
plt.savefig('combined_plot_qlearning.png')

##### END CODING HERE #####


