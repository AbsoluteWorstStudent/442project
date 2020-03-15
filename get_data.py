import bluetooth # needed for bluetooth communication
import thingspeak # needed for thingspeak
import time     #for delay
 
bluetooth_addr = "00:14:03:05:59:8F" # The address from the HC-06 sensor
bluetooth_port = 1 # Channel 1 for RFCOMM
bluetoothSocket = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
bluetoothSocket.connect((bluetooth_addr,bluetooth_port))

#thingspeak information
channel_id = 997565 # channel ID from your Thingspeak channel
key = "45O0RZQ5E187G5D5" # obtain from Thingspeak
url = 'https://api.thinkspeak.com/update' # default URL to update Thingspeak
ts = thingspeak.Channel(channel_id, key, url)


print("Attempting Data Retrieval")
while 1:
    try:
        received_data = bluetoothSocket.recv(1024)
        temperature = int.from_bytes(received_data,byteorder='big')
        time.sleep(0.5)
        lux = int.from_bytes(received_data,byteorder='big')
        print("Data Retrieved")
        print("Current Temperature: %d" % temperature)
        print("Current Lux: %d" %lux)
       # thingspeak_field1 = {"field1": temperature}
       # ts.update(thingspeak_field1) # update temp value in thingspeak
       # thingspeak_field2 = {"field2": lux}
       # ts.update(thingspeak_field2) # update lux value in thingspeak
       # print("Thingspeak Updated")
        
    except KeyboardInterrupt:
        print("keyboard interrupt detected")
        break
bluetoothSocket.close()
