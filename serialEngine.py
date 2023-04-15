import serial
from stockfish import Stockfish
import time
from inputimeout import inputimeout


def setDifficulty():
    print("setting difficulty")
    difficulty = ser.read_until(expected=serial.CR, size=2)
    if(difficulty == b'1\r'):
        ser.write(b'ack')
        stockfish.set_depth(5)
        stockfish.update_engine_parameters({"Skill Level": -9, "Minimum Thinking Time": 50})
    elif(difficulty == b'2\r'):
        ser.write(b'ack')
        stockfish.set_depth(5)
        stockfish.update_engine_parameters({"Skill Level": -5, "Minimum Thinking Time": 100})
    elif(difficulty == b'3\r'):
        ser.write(b'ack')
        stockfish.set_depth(5)
        stockfish.update_engine_parameters({"Skill Level": -1, "Minimum Thinking Time": 150})            
    elif(difficulty == b'4\r'):
        ser.write(b'ack')
        stockfish.set_depth(5)
        stockfish.update_engine_parameters({"Skill Level": 3, "Minimum Thinking Time": 200})
    elif(difficulty == b'5\r'):
        ser.write(b'ack')
        stockfish.set_depth(5)
        stockfish.update_engine_parameters({"Skill Level": 7, "Minimum Thinking Time": 300})
    elif(difficulty == b'6\r'):
        ser.write(b'ack')
        stockfish.set_depth(8)
        stockfish.update_engine_parameters({"Skill Level": 11, "Minimum Thinking Time": 400})
    elif(difficulty == b'7\r'):
        ser.write(b'ack')
        stockfish.set_depth(13)
        stockfish.update_engine_parameters({"Skill Level": 16, "Minimum Thinking Time": 500})
    elif(difficulty == b'8\r'):
        ser.write(b'ack')
        stockfish.set_depth(22)
        stockfish.update_engine_parameters({"Skill Level": 20, "Minimum Thinking Time": 1000})
    else:
        print("nck\n")
        ser.write(b'nck')

    print(bytes(str(stockfish.get_parameters()), 'ascii'))

def setupBoard():
    fen = ser.read_until(expected=serial.CR, size=100).decode('ascii')[:-1]
    print(fen)
    if(stockfish.is_fen_valid(fen)):
        ser.write(b'ack')
        stockfish.set_fen_position(fen)
    else:
        ser.write(b'invalid position\r')

def setupDefault():
    stockfish.set_fen_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1")

def checkMove():
    move = ser.read_until(expected=serial.CR, size=10).decode('ascii')[:-1]
    ser.write(b'ack')
    ser.write(bytes(str(stockfish.is_move_correct(move)), 'ascii'))

def getBoardState():
    ser.write(bytes(stockfish.get_board_visual(), 'ascii'))

# always check move first
def makeMove():
    move = ser.read_until(expected=serial.CR, size=10).decode('ascii')[:-1]
    ser.write(b'ack')
    stockfish.make_moves_from_current_position([move.lower()])

def computerMove():
    move = stockfish.get_best_move()
    print(move.upper())
    ser.write(bytes(stockfish.get_best_move().upper(),'ascii'))
    stockfish.make_moves_from_current_position([move])

def isGameOver():
    eval = stockfish.get_evaluation()
    if(eval['type'] == 'mate' and eval['value'] == 0):
        ser.write(b'True\r')
    else:
        ser.write(b'False\r')

def getHumanMove():
    done = False
    move = ""
    while(not done):
        move = inputimeout(prompt='Name your best friend:', timeout=3)
        if(stockfish.is_move_correct(move) == False):
            print("choose a valid move")
        else:
            done = True
    stockfish.make_moves_from_current_position([move])
    ser.write(bytes(move, 'ascii'))
        



stockfish = Stockfish(path="/Users/Jeremy/Documents/W23/373/finalProject/AutomatedChess/stockfish/stockfish.exe")

ser = serial.Serial('COM12', 9600)

while True:
    request = ser.read_until(expected=serial.CR, size=50)
    print(request)
    if(request == b'difficulty\r'):
        ser.write(b'ack')
        setDifficulty()
    elif(request == b'default setup\r'):
        ser.write(b'ack')
        setupDefault()
    elif(request == b'setup\r'):
        ser.write(b'ack')
        setupBoard()
    elif(request == b'board state\r'):
        ser.write(b'ack')
        getBoardState()
    elif(request == b'make move\r'):
        ser.write(b'ack')
        makeMove()
    elif(request == b'check move\r'):
        ser.write(b'ack')
        checkMove()
    elif(request == b'computer move\r'):
        ser.write(b'ack')
        computerMove()
    elif(request == b'human move\r'):
        ser.write(b'ack')
        getHumanMove()
    elif(request == b'is game over\r'):
        ser.write(b'ack')
        isGameOver()
    elif(request == b'exit\r'):
        ser.write(b'ack')
        exit()
    else:
        ser.write(b'nck')