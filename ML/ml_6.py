import random
import numpy as np


ALPHA = 0.4
GAMMA = 0.9
EPSILON = 1.0     
MIN_EPSILON = 0.05
DECAY = 0.99995   
EPISODES = 50000  

print("Alpha =", ALPHA, "| Gamma =", GAMMA, "| Epsilon =", EPSILON)


def initialize_board():
    return [[0, 0, 0], [0, 0, 0], [0, 0, 0]]

def print_board(board, show_numbers=False):
    if show_numbers:
        nums = [[str(3*i+j+1) for j in range(3)] for i in range(3)]
        for row in nums:
            print(" | ".join(row))
            print("-" * 5)
    else:
        symbols = {0: " ", 1: "X", -1: "O"}
        for row in board:
            print(" | ".join(symbols[cell] for cell in row))
            print("-" * 5)

def check_winner(board):
    for i in range(3):
        if abs(sum(board[i])) == 3: 
            return True
        if abs(sum([board[j][i] for j in range(3)])) == 3: 
            return True
    if abs(board[0][0] + board[1][1] + board[2][2]) == 3:
        return True
    if abs(board[0][2] + board[1][1] + board[2][0]) == 3:
        return True
    return False

def board_full(board):
    return all(cell != 0 for row in board for cell in row)


Q_table = {}

def get_state(board):
    return str(board)

def initialize_Q(state):
    if state not in Q_table:
        Q_table[state] = np.zeros(9) 

def action_to_coords(action):
    return divmod(action, 3)

def choose_action(state, epsilon):
    initialize_Q(state)
    if random.random() < epsilon:  
        return random.randint(0, 8)
    else: 
        return np.argmax(Q_table[state])

def update_Q(state, action, reward, new_state):
    initialize_Q(new_state)
    current_q = Q_table[state][action]
    max_future_q = np.max(Q_table[new_state])
    Q_table[state][action] = current_q + ALPHA * (reward + GAMMA * max_future_q - current_q)

def random_move(board, player):
    empty = [(i, j) for i in range(3) for j in range(3) if board[i][j] == 0]
    if empty:
        row, col = random.choice(empty)
        board[row][col] = player


for episode in range(EPISODES):
    board = initialize_board()
    game_over = False
    turn = 1  
    last_O_state = None
    last_O_action = None

    while not game_over:
        state = get_state(board)

        if turn == -1: 
            action = choose_action(state, EPSILON)
            row, col = action_to_coords(action)
            if board[row][col] == 0:
                board[row][col] = -1
                new_state = get_state(board)

                last_O_state = state
                last_O_action = action

                if check_winner(board):
                    reward = 1
                    update_Q(last_O_state, last_O_action, reward, new_state)
                    game_over = True
                elif board_full(board):
                    reward = 0
                    update_Q(last_O_state, last_O_action, reward, new_state)
                    game_over = True
                else:
                    reward = 0  
                    update_Q(last_O_state, last_O_action, reward, new_state)

            else:
               
                reward = -0.5
                update_Q(state, action, reward, state)
                random_move(board, -1)

        else:  
            random_move(board, 1)
            if check_winner(board):
              
                if last_O_state is not None:
                    reward = -1
                    update_Q(last_O_state, last_O_action, reward, get_state(board))
                game_over = True
            elif board_full(board):
               
                if last_O_state is not None:
                    reward = 0
                    update_Q(last_O_state, last_O_action, reward, get_state(board))
                game_over = True

        turn *= -1  

   
    if EPSILON > MIN_EPSILON:
        EPSILON *= DECAY

print("Training finished ")


def play_vs_computer():
    board = initialize_board()
    game_over = False
    print("\nTic-Tac-Toe: You are X, Computer is O")
    print("\nBoard positions (1-9):")
    print_board(board, show_numbers=True)

    while not game_over:
   
        valid = False
        while not valid:
            try:
                move = int(input("Enter your move (1-9): ")) - 1
                row, col = divmod(move, 3)
                if 0 <= row < 3 and 0 <= col < 3 and board[row][col] == 0:
                    board[row][col] = 1
                    valid = True
                else:
                    print("Invalid move, try again.")
            except:
                print("Enter a number between 1-9.")

        print("\nYour move:")
        print_board(board)

        if check_winner(board):
            print(" You win!")
            break
        if board_full(board):
            print("It's a draw ")
            break

     
        state = get_state(board)
        action = choose_action(state, 0)  
        row, col = action_to_coords(action)
        if board[row][col] == 0:
            board[row][col] = -1
        else:
            random_move(board, -1)

        print("\nComputer move:")
        print_board(board)

        if check_winner(board):
            print(" Computer wins!")
            break
        if board_full(board):
            print("It's a draw ")
            break


play_vs_computer()
