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
  <li>Instalar o Arduino IDE e conectar a sua placa ESP32 no computador para que seja feita a instalação das bibliotecas específicas para o chip. Caso ele não identifique a placa, é necessário selecionar manualmente a placa ESP32 Dev Module com a porta conectada</li>
  <li>No menu lateral da esquerda no Arduino IDE, selecionar a terceira opção (Gerenciador de bibliotecas) e pesquisar pela biblioteca "PubSubClient" de Nick O'Leary. Instale-a.
      ![Config Biblioteca](https://github.com/mateusahschulz/projeto-final-redes/blob/main/assets/Config%20Biblioteca.gif)
  </li>
  <li>Na barra de ferramentas, ir na aba Ferramentas (Tools), procurar pela opção Partition Scheme e dentro desta opção selecionar o valor "Huge App"
      ![Config HugeApp](https://github.com/mateusahschulz/projeto-final-redes/blob/main/assets/Config%HugeApp.gif)
  </li>
  <li>Com essas configurações, o projeto deve executar corretamente no Arduino IDE</li>
</ol>
