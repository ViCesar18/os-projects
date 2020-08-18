import socket
import threading
import random
import time
import matplotlib.pyplot as plt

def request (ip, porta):
    # Declaração do socket
    s = socket.socket()

    # Conexão com ip/porta
    s.connect ((ip, porta))

    # Geração aleatória do número a ser mandado na thread
    value_request = random.randint (1,1000)
    s.send (str(value_request).encode('utf-8'))

    # Obtenção do resultado da conexão
    result = s.recv (1024).decode('utf-8')
    print (f"Quantidade de elementos ate {value_request}: {result}")

    # Encerra a conexão
    s.close()

# Input do IP
entrada_ip = str (input ("IP: "))
if (entrada_ip == ""):
    endereco = socket.gethostname()
else:
    endereco = entrada_ip

# Input da porta
entrada_porta = int (input ("Porta: "))

# Declaração da lista de threads
lista_threads = []

# Criação e inicio das threads
for i in range (20):
    lista_threads.append (threading.Thread (target=request, args=(endereco, entrada_porta)))

for t in lista_threads:
    t.start()
    t.join()

lista_threads = []
tempos = []
n_threads = []

j = 20
while(j <= 200):
    inicio = time.time()

    # Criação e inicio das threads
    for i in range (j):
        lista_threads.append (threading.Thread (target=request, args=(endereco, entrada_porta)))

    for t in lista_threads:
        t.start()
        t.join()
    
    fim = time.time()

    tempos.append(fim - inicio)
    n_threads.append(j)
    
    lista_threads = []
    j += 20

plt.bar(n_threads, tempos)  #Cria o gráfico de barra (primeiro param é x, segundo é y)
plt.xticks(n_threads)
plt.xlabel('Threads (clientes)')
plt.ylabel('Tempo (s)')
plt.title('Quantidade de Clientes X Tempo de Atendimento')
plt.show()
