import serial
from stockfish import Stockfish

stockfish = Stockfish(path="/Users/Jeremy/Documents/W23/373/finalProject/AutomatedChess/stockfish/stockfish.exe")

ser = serial.Serial('COM10', 9600)

# ser.write(b'hello')
# print(ser.read_until(expected=serial.LF, size=12))