# Connect Four Bot

An AI designed to play Connect Four with a user. Uses the Minimax algorithm with an 8-ply game tree.

## Evaluation Function:

My evaluation function determines how many possible winning plays are possible for both the player and AI. If either of the players would win, they would be scored as Infinity or -Infinity. If not, then the function subtracts the player’s number of possible win conditions from the AI’s. Therefore, the AI’s strategy is to remove as many possible winning moves from the enemy while keeping as many winning moves open to itself until it finds a way to win. The search depth is set to a fixed depth of 8 to keep calculation times below 5 seconds.
  
## Algorithm:

The algorithm builds a minimax tree 8 deep, and each turn will expand on it to maintain the depth of 8. Depending on how predictably the player plays, the AI may take very little time to calculate the next move if most the of the tree the player is following is already generated and carried over from the previous turn. Alpha and Beta pruning are implemented when calculating score.

All 69 possible 4-token combinations (win combinations) on a Connect Four board are stored in an array. Game states are represented as 64-bit numbers where the 42 least significant bits represent the 42 locations on the board. Using bitwise operators between the game state number and win combinations, the win check function is done very quickly. The main bottleneck of the program is the expansion of the tree.
