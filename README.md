# Projeto Final Redes de Computadores 2024/01

VOu colocar uma descrição mais legal aqui do que tem que fazer

# Como executar o projeto:
Para rodar o projeto é necessário algumas configurações para que este compile corretamente:
<ol>
  <li>Instalar o Arduino IDE e conectar a sua placa ESP32 no computador para que seja feita a instalação das bibliotecas específicas para o chip. Caso ele não identifique a placa, é necessário selecionar manualmente a placa ESP32 Dev Module com a porta conectada</li>
  <li>No menu lateral da esquerda no Arduino IDE, selecionar a terceira opção (Gerenciador de bibliotecas) e pesquisar pela biblioteca "PubSubClient" de Nick O'Leary. Instale-a</li>
  <li>Na barra de ferramentas, ir na aba Ferramentas (Tools), procurar pela opção Partition Scheme e dentro desta opção selecionar o valor "Huge App"</li>
  <li>Com essas configurações, o projeto deve executar corretamente no Arduino IDE</li>
</ol>
