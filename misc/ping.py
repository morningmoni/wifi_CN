from time import sleep
from socket import *
serverName = '192.168.1.152'
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_DGRAM)
# message = input('Input lowercase sentence:')
message = b'123'
while True:
    clientSocket.sendto(message, (serverName, serverPort))
    sleep(0.001)
    # modifiedMessage, serverAddress = clientSocket.recvfrom(2048)
    # print(modifiedMessage)
clientSocket.close()
