import serial
from stockfish import Stockfish

def setDifficulty():
    difficulty = ser.read_until(expected=serial.CR, size=2)
    if(difficulty == b'1\r'):
        ser.write(b'ack\r')
        stockfish.set_depth(5)
        stockfish.update_engine_parameters({"Skill Level": -9, "Minimum Thinking Time": 50})
    elif(difficulty == b'2\r'):
        ser.write(b'ack\r')
        stockfish.set_depth(5)
        stockfish.update_engine_parameters({"Skill Level": -5, "Minimum Thinking Time": 100})
    elif(difficulty == b'3\r'):
        ser.write(b'ack\r')
        stockfish.set_depth(5)
        stockfish.update_engine_parameters({"Skill Level": -1, "Minimum Thinking Time": 150})            
    elif(difficulty == b'4\r'):
        ser.write(b'ack\r')
        stockfish.set_depth(5)
        stockfish.update_engine_parameters({"Skill Level": 3, "Minimum Thinking Time": 200})
    elif(difficulty == b'5\r'):
        ser.write(b'ack\r')
        stockfish.set_depth(5)
        stockfish.update_engine_parameters({"Skill Level": 7, "Minimum Thinking Time": 300})
    elif(difficulty == b'6\r'):
        ser.write(b'ack\r')
        stockfish.set_depth(8)
        stockfish.update_engine_parameters({"Skill Level": 11, "Minimum Thinking Time": 400})
    elif(difficulty == b'7\r'):
        ser.write(b'ack\r')
        stockfish.set_depth(13)
        stockfish.update_engine_parameters({"Skill Level": 16, "Minimum Thinking Time": 500})
    elif(difficulty == b'8\r'):
        ser.write(b'ack\r')
        stockfish.set_depth(22)
        stockfish.update_engine_parameters({"Skill Level": 20, "Minimum Thinking Time": 1000})
    else:
        ser.write(b'nack\r')

    # ser.write(bytes(str(stockfish.get_parameters()), 'ascii'))

def setupBoard():
    fen = ser.read_until(expected=serial.CR, size=100).decode('ascii')[:-1]
    print(fen)
    if(stockfish.is_fen_valid(fen)):
        ser.write(b'ack\r')
        stockfish.set_fen_position(fen)
    else:
        ser.write(b'invalid position\r')

def setupDefault():
    stockfish.set_fen_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1")

def checkMove():
    move = ser.read_until(expected=serial.CR, size=10).decode('ascii')[:-1]
    ser.write(b'ack\r')
    ser.write(bytes(str(stockfish.is_move_correct(move)), 'ascii'))

def getBoardState():
    ser.write(bytes(stockfish.get_board_visual(), 'ascii'))

# always check move first
def makeMove():
    move = ser.read_until(expected=serial.CR, size=10).decode('ascii')[:-1]
    ser.write(b'ack\r')
    stockfish.make_moves_from_current_position([move.lower()])

def computerMove():
    move = stockfish.get_best_move()
    ser.write(bytes(stockfish.get_best_move(),'ascii'))
    ser.write(b'\r')
    stockfish.make_moves_from_current_position([move])

def isGameOver():
    eval = stockfish.get_evaluation()
    if(eval['type'] == 'mate' and eval['value'] == 0):
        ser.write(b'True\r')
    else:
        ser.write(b'False\r')


stockfish = Stockfish(path="/Users/Jeremy/Documents/W23/373/finalProject/AutomatedChess/stockfish/stockfish.exe")

ser = serial.Serial('COM10', 9600)

while True:
    request = ser.read_until(expected=serial.CR, size=50)
    if(request == b'difficulty\r'):
        ser.write(b'ack\r')
        setDifficulty()
    elif(request == b'default setup\r'):
        ser.write(b'ack\r')
        setupDefault()
    elif(request == b'setup\r'):
        ser.write(b'ack\r')
        setupBoard()
    elif(request == b'board state\r'):
        ser.write(b'ack\r')
        getBoardState()
    elif(request == b'make move\r'):
        ser.write(b'ack\r')
        makeMove()
    elif(request == b'check move\r'):
        ser.write(b'ack\r')
        checkMove()
    elif(request == b'computer move\r'):
        ser.write(b'ack\r')
        computerMove()
    elif(request == b'is game over\r'):
        ser.write(b'ack\r')
        isGameOver()
    elif(request == b'exit\r'):
        ser.write(b'ack\r')
        exit()
    else:
        ser.write(b'nack\r')