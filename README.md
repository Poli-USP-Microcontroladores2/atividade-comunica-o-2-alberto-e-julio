# PSI-Microcontroladores2-Aula10
Atividade: Comunicação UART

# Projeto UART – Atividade em Duplas (Echo Bot + Async API)

## 1. Informações Gerais

* Dupla:

  * Integrante 1: Alberto Galhego Neto - 17019141
  * Integrante 2: Júlio Cesar Braga Parro - 16879560

* Objetivo: implementar, testar e documentar aplicações de comunicação UART baseadas nos exemplos oficiais “echo_bot” e “async_api”, utilizando desenvolvimento orientado a testes, diagramas de sequência D2 e registro de evidências.

---

# 2. Estrutura Esperada do Repositório

```
README.md
src/

docs/
  evidence/
  sequence-diagrams/

```

---

# 3. Etapa 1 – Echo Bot (UART Polling/Interrupt)

## 3.1 Descrição do Funcionamento

- O Echo Bot utiliza o UART para receber uma mensagem enviada pelo usuário via serial monitor e retorna a mesma mensagem copiada, na forma "Echo: MENSAGEM". 
- Ao ser iniciado, o echo bot exibe no terminal as mensagens "Hello! I'm your echo bot." e "Tell me something and press enter:". 
- Em seguida, o usuário pode digitar qualquer texto no monitor serial e pressionar Enter, o programa então detecta o fim da linha, interpreta o que foi digitado e envia de volta a mesma mensagem na forma “Echo: MENSAGEM”, mostrando-a no terminal. Esse processo de retorno de mensagens pode se repetir continuamente.

- Link usado como referência:
[https://docs.zephyrproject.org/latest/samples/drivers/uart/echo_bot/README.html](https://docs.zephyrproject.org/latest/samples/drivers/uart/echo_bot/README.html)

## 3.2 Casos de Teste Planejados (TDD)

### CT1 – Eco básico

* Entrada: "Hello, World!"
* Saída esperada: "Echo: Hello, World!"
* Critério de Aceitação: O echo bot deve retornar a mesma mensagem que o usuário envia pelo serial monitor, na forma "Echo: MENSAGEM", sem haver conflito de caracteres, ou seja, retornar "Echo: Hello, World!".

### CT2 – Linha vazia

* Entrada: "" (Apenas um ENTER)
* Saída esperada: "Echo:"
* Critério de Aceitação: O echo bot deve retornar a mesma mensagem que o usuário envia pelo serial monitor, na forma "Echo: MENSAGEM", ou seja, "Echo:"

### CT3 – Linha longa

* Entrada: "Oi, echo bot, estou testando o seu funcionamento com uma mensagem longa"
* Saída esperada: "Echo: Oi, echo bot, estou testando o"
* Critério de Aceitação: O echo bot deve retornar o começo da mensagem da mesma forma que foi enviada, sem houver alteração de caracteres, mas de forma incompleta, devido ao limite de 30 caracteres que ele pode retornar. Os espaços também são considerados como caracteres.

### CT4 – Caracteres especiais e acentos

* Entrada: "# Olá, çê @ ã"
* Saída esperada: "Echo: # Olá, çê @ ã"
* Critério de Aceitação: O echo bot deve retornar a mesma mensagem que o usuário envia pelo serial monitor, na forma "Echo: MENSAGEM", sem haver conflito de caracteres. Pode ser que a mensagem seja retornada na linha logo abaixo da linha com "Echo:".

## 3.3 Implementação

* Arquivo(s) modificados: Modificou-se os arquivos main.c e prj.conf, colocando os códigos coletados do código de exemplo.
* Justificativa das alterações: Criou-se um novo projeto no Platform IO, com as configurações básicas copiadas de aulas anterriores, e inseriu-se o main.c e prj.conf coletados do exemplo do link de referência. O prj.conf configura a comunicação serial e configura o UART. Dessa forma, o código funcionou normalmente, sem necessitar de maiores alterações.

## 3.4 Evidências de Funcionamento

Evidências salvas em `Etapa_1/docs/evidence/`.

```
[Link para o log dos testes](Etapa_1\docs\evidence\Testes_Etapa1.png)
```

Trechos ilustrativos do output:

- Teste CT1:
```
Hello! I'm your echo bot. Tell me something and press enter:
Echo: Hello World!
```

- Teste CT2:
```
Hello! I'm your echo bot. Tell me something and press enter:
Echo:
```

- Teste CT3:
```
Hello! I'm your echo bot. Tell me something and press enter:
Echo: Oi, echo bot, estou testando o
```

- Teste CT4:
```
Hello! I'm your echo bot. Tell me something and press enter:
Echo: # Olá, çê @ ã
```

## 3.5 Diagramas de Sequência D2

Arquivos dos diagramas (diagrama completo e o código-base para geração do diagrama) adicionados em `Etapa_1/docs/sequence-diagrams/`.

---

# 4. Etapa 2 – Async API (Transmissão/Recepção Assíncrona)

## 4.1 Descrição do Funcionamento

- O programa alterna continuamente entre transmissão e recepção UART a cada 5 segundos: em cada ciclo, ele envia de 1 a 4 pacotes de texto com identificadores de loop e pacote, tratando interrupções de transmissão e fila de envio. Ao final da fase de TX, ele alterna o estado do receptor — habilitando o RX para receber dados assíncronos e mostrar no log o conteúdo recebido, ou desabilitando-o no ciclo seguinte — repetindo esse processo indefinidamente enquanto registra no log cada troca de modo e evento UART.

- Link usado como referência:
[https://docs.zephyrproject.org/latest/samples/drivers/uart/async_api/README.html](https://docs.zephyrproject.org/latest/samples/drivers/uart/async_api/README.html)

## 4.2 Casos de Teste Planejados (TDD)

### CT1 – Transmissão de pacotes a cada 5s

### CT2 – Recepção

### CT3 – Verificação de timing dos 5s

(Adicionar mais casos se necessário.)

## 4.3 Implementação

* Arquivos modificados:
* Motivos/Justificativas:

## 4.4 Evidências de Funcionamento

Salvar em `docs/evidence/async_api/`.

Exemplo:

```
Loop 0:
Sending 3 packets (packet size: 5)
Packet: 0
Packet: 1
Packet: 2
```

Ou:

```
RX is now enabled
UART callback: RX_RDY
Data (HEX): 48 65 6C 6C 6F
Data (ASCII): Hello
```

## 4.5 Diagramas de Sequência D2

Vide material de referência: https://d2lang.com/tour/sequence-diagrams/

Adicionar arquivos (diagrama completo e o código-base para geração do diagrama) em `docs/sequence-diagrams/`.

---

# 5. Conclusões da Dupla

* O que deu certo: Na etapa 1, o código do Echo Bot, que utiliza a implementação por interrupção, funcionou corretamente sem haver necessidade de muitas mudanças. Os testes funcionaram corretamente e a implementação e verificação foi simples. 
* O que foi mais desafiador: Na etapa 2, o código que utiliza a implementação por Async API não funciona corretamente na placa FRDM KL25Z que utilizamos. Portanto, houve a necessidade de adaptar o código por interrupção (Echo Bot) para realizar as tarefas feitas pelo código async, de modo a diferenciar momentos em que há apenas TX ou apenas RX.