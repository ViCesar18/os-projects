import socket
from threading import Thread

def fibonacci(num):
    i = 1
    termo1 = 0
    termo2 = 1
    prox_termo = 0

    while prox_termo <= num:
        i += 1
        prox_termo = termo1 + termo2
        termo1 = termo2
        termo2 = prox_termo

    return i

def servidor(socket_servidor):
    while True:
        client_socket, addr = socket_servidor.accept()
        print(f'Conexão estabelecida de ({addr})')

        num = int(client_socket.recv(1024).decode('utf-8'))
        client_socket.send(str(fibonacci(num)).encode('utf-8'))

        client_socket.close()


porta = int(input('Porta: '))

socket_servidor = socket.socket()
socket_servidor.bind(('0.0.0.0', porta))
socket_servidor.listen()

lista_threads = []

for i in range(100):
    lista_threads.append(Thread(target=servidor, args=[socket_servidor]))

for t in lista_threads:
    t.start()
    t.join()
