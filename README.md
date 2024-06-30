# Projeto Final Redes de Computadores 2024/01

<p>
O desafio do projeto consiste em desenvolver um localizador de objetos em uma distância curta de
10 metros usando Bluetooth Low Energy com monitoramento IoT e com o protocolo MQTT.
</p>
<p>
Deverá ser desenvolvida uma aplicação que permita a localização baseada na proximidade de um
objeto dentro do raio do Raio Signal Strenght Indicator (RSSI) gerado pelo campo de alcance do
Bluetooth Low Energy fornecido pelo microcontrolador ESP32.
</p>

# Como executar o projeto:
Para rodar o projeto é necessário algumas configurações para que este compile corretamente:
<ol>
  <li>Com o projeto baixado, é necessário instalar o Arduíno IDE.</li>
  <li>Após instalar, abra o arquivo localizado ./trab-final-redes/trab-final-redes.ino</li>
  <li>Com o arquivo aberto, conecte a sua placa ESP32 no computador para que seja feita a instalação das bibliotecas específicas para o chip. Caso ele não identifique a placa, é necessário selecionar manualmente a placa ESP32 Dev Module com a porta conectada</li>
  <li>No menu lateral da esquerda no Arduino IDE, selecionar a terceira opção (Gerenciador de bibliotecas) e pesquisar pela biblioteca "PubSubClient" de Nick O'Leary. Instale-a.
      ![Config Biblioteca](https://github.com/mateusahschulz/projeto-final-redes/blob/main/assets/Config%20Biblioteca.gif)
  </li>
  <li>Na barra de ferramentas, ir na aba Ferramentas (Tools), procurar pela opção Partition Scheme e dentro desta opção selecionar o valor "Huge App"
      ![Config HugeApp](https://github.com/mateusahschulz/projeto-final-redes/blob/main/assets/Config%HugeApp.gif)
  </li>
  <li>No código-fonte, mudar a variável "mqttBroker" para o IP da sua máquina que é possível achar via terminal com o comando ipconfig. Na função "connectWifi()" é necessário mudar as variávies "wifiName" e "wifiPassword" para respectivas informações de seu WIFI. Vai ser necessário alterar também a variável "ClientID" na função "connectMQTT()", mas mais adiante.
  </li>
</ol>

Com as informações acima, o código-fonte rodará, mas ainda é necessário configurar um broker MQTT para que se consiga fazer as publicações com a distância e outras informações dos dispositivos com sinal captado.
Para realizar estas configurações é necessário:
<ol>
  <li>
    Baixar o Mosquitto MQTT Broker disponível em https://www.mosquitto.org/download/.
  </li>
  <li>Na pasta que em foi baixado, criar um arquivo "nome-do-arquivo".conf, com as seguintes informações:</li>
    allow_anonymous true
    listener 1883
  <li>Com este arquivo criado, abrir o terminal como administrador e rodar o seguinte comando na pasta em que o mosquitto está instalado: "./mosquitto.exe -c mosquitto-teste.conf"</li>
<ol>
  <li>
    Baixar o Mosquitto MQTT Broker disponível em https://www.mosquitto.org/download/.
  </li>
  <li>Após instalar o Mosquitto Broker, baixar a interface de cliente do MQTT, chamada MQTTX.</li>
  <li>Com o MQTTX instalado, criar uma nova conexão, com nome de sua escolha, guardar a informação do ClientID e trocar a variável ClientID no código-fonte por essa informação. Por fim, colocar o mesmo IP configurado no código-fonte no host.</li>
  <li>Após a conexão estar criada e conectada, adicionar uma nova inscrição com o tópico definido na variável "topic" no código-fonte.</li>
</ol>
<p>Com todas essas configurações feitas, é possível executar o programa no Arduíno IDE e analisar os resultados no MQTTX.</p>
