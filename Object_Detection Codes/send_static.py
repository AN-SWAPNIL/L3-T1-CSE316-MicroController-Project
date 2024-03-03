
import serial.tools.list_ports

serialIns = serial.Serial()

val = 'COM3'

serialIns.baudrate = 9600
serialIns.port = val
serialIns.open()

# serialIns.write('nothing'.encode('utf-8'))

while True:
    command = input("Enter Fruit: ")
    serialIns.write(command.encode('utf-8'))

