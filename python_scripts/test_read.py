import machine
import time

def parseInfo(id):
    
    print("--------")
    print("RAW DATA: ", id)
    if id[0] == 0x2 and id[13] == 0x3:
        print("Packet starts with 0x2 and ends with 0x3: OK")
    str_id = str(id)[6:16]
    checksum = int(str(id)[16:18], 16)
    print("Tag ID: %s" % str_id)
    
    calculatedChecksum = 0
    hex_chunks = [ int(str_id[i:i+2], 16) for i in range(0, len(str_id), 2) ]
    for n in hex_chunks: calculatedChecksum ^= n
    
    print("Calculated checksum: %s" % ("0x%0.2X" % calculatedChecksum))
    if checksum == calculatedChecksum:
        print("Checksum OK, read successful")
    else:
        print("Checksum error!! (received %s, calculated %s)" % (hex(checksum), hex(calculatedChecksum)))
        return None
    
    print("--------")
    return str_id

def main():
    
    uart = machine.UART(0, baudrate=9600, bits=8, parity=None, stop=1, rx=machine.Pin(1))
    readData = bytes()
    clearToReceive = True
    lastRead = time.ticks_ms()
    
    while True:
        if (not clearToReceive) and (time.ticks_diff(time.ticks_ms(), lastRead) > 2000):
            clearToReceive = True
            print("Clear to receive!")
        
        if (uart.any() >= 14 and clearToReceive):

            readData = uart.read(14)
            tag_id = parseInfo(readData)
            print("remove tag from field")
            readData = ""
            clearToReceive = False
            lastRead = time.ticks_ms() #set last read to now
            uart.read() #flush

main()
        