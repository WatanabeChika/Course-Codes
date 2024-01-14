
'''
Test the model by playing with mcts_pure
'''

from __future__ import print_function
import pickle
from game import Board, Game
from mcts_pure import MCTSPlayer as MCTS_Pure
from mcts_alphaZero import MCTSPlayer
from policy_value_net_pytorch import PolicyValueNet
import time 

def run():
    n = 5
    width, height = 9, 9
    model_file = 'best_policy.model'
    try:
        board = Board(width=width, height=height, n_in_row=n)
        game = Game(board)

        best_policy = PolicyValueNet(width, height, model_file = model_file)
        mcts_player1 = MCTSPlayer(best_policy.policy_value_fn,
                                  c_puct=5,
                                  n_playout=200)
        mcts_player2 = MCTS_Pure(c_puct=5, n_playout=200)

        # set start_player=1 for MCTS_Pure first
        res = 0
        for i in range(10):
            t0 = time.time()
            winner, step = game.start_play(mcts_player1, mcts_player1, start_player=1, is_shown=0)
            print('time: {}'.format(time.time()-t0))
            print('step: {}'.format(step))
            if (winner == 1): res += 1
        print('和MCTS_Pure对战结果：10局中{}胜{}负'.format(res, 10-res))
    except KeyboardInterrupt:
        print('\n\rquit')


if __name__ == '__main__':
    run()