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

## Observação

- Por conta de limitações da placa, não foi possível implementar a Async API. Como solução, a atividade foi realizada utilizando interrupt.

## 4.1 Descrição do Funcionamento

- O programa alterna continuamente entre o modo de transmissão e o modo de recepção UART a cada 10 segundos:
	-Modo de Transmissão:
		-O programa envia uma mensagem aleatória a cada 500ms. A mensagem é escolhida entre 3 possibilidades: "Message 1", "Option 2" e "Third Option".
		-Durante esse período, qualquer mensagem recebida é ignorada.
		
	-Modo Recepção:
		-O programa não realiza o envio de mensagens durante esse período.
		-Ao receber "red", "blue" ou "green", a placa pisca brevemente o LED correspondente.
		
- Cada Modo é gerenciado por um Thread diferente, coordenados por um timer, que os controla usando k_thread_resume() e k_thread_suspend().

- Links usados como referências:
[https://docs.zephyrproject.org/latest/samples/drivers/uart/async_api/README.html](https://docs.zephyrproject.org/latest/samples/drivers/uart/async_api/README.html)
[https://docs.zephyrproject.org/latest/samples/drivers/uart/echo_bot/README.html](https://docs.zephyrproject.org/latest/samples/drivers/uart/echo_bot/README.html)

## 4.2 Casos de Teste Planejados (TDD)

### CT1 – Transmissão de pacotes a cada 500ms

* Entrada: -
* Saída esperada: "Teste!"
* Critério de Aceitação: O programa deve enviar a mensagem "Teste!" a cada 500ms.

### CT2 – Transmissão de mensagens aleatórias

* Entrada: -
* Saída esperada: "Message 1", "Option 2" e "Third Option", escolhidas aleatoriamente.
* Critério de Aceitação: O programa deve enviar uma das mensagens possíveis a cada 500ms.

### CT3 – Implementação dos Ciclos

* Entrada: -
* Saída esperada: "Message 1", "Option 2" e "Third Option" APENAS durante o ciclo de transmissão. Durante o período de recebimento, ainda não implementado, não deverá enviar mensagem alguma.
* Critério de Aceitação: O programa deve enviar uma das mensagens possíveis a cada 500ms, mas somente durante o ciclo de transmissão. Deverá trocar o ciclo a cada 10s.

### CT4 – Implementação do Recebimento

* Entrada: Qualquer mensagem
* Saída esperada: Durante o período de transmissão, o mesmo comportamento do teste 3. Durante o ciclo de recebimento, e apenas durante ele, o LED deverá piscar sempre que receber uma mensagem, independente do seu conteúdo.
* Critério de Aceitação: O programa deve enviar uma das mensagens possíveis a cada 500ms, mas somente durante o ciclo de transmissão. Durante o ciclo de recebimento, o programa deverá piscar o led sempre que receber uma mensagem, ignorando mensagens recebidas fora desse período.

### CT5 – Implementação das Cores

* Entrada: "red", "green" ou "blue"
* Saída esperada: Além do comportamento do teste 4, o LED deverá piscar de acordo com a mensagem recebida, piscando o LED azul se receber "blue", o LED verde se receber "green" ou o LED vermelho se "red", devendo ignorar qualquer outra mensagem.
* Critério de Aceitação: O programa envia as mensagens durante o período de transmissão. Durante o período de recepção, pisca os LEDs adequadamente.

## 4.3 Implementação

* Arquivo(s) modificados: Modificou-se os arquivos main.c e prj.conf.
* Motivos/Justificativas: Implementação de funções extras do kernel, que requerem a declaração prévia no prj.conf. Implementação de novas funções. Reestruturação do código.

## 4.4 Evidências de Funcionamento

Evidências salvas em `Etapa_2/docs/evidence/`.

## 4.5 Diagramas de Sequência D2

Arquivos dos diagramas (diagrama completo e o código-base para geração do diagrama) adicionados em `Etapa_2/docs/sequence-diagrams/`.

# 5. Conclusões da Dupla

* O que deu certo: Na etapa 1, o código do Echo Bot, que utiliza a implementação por interrupção, funcionou corretamente sem haver necessidade de muitas mudanças. Os testes funcionaram corretamente e a implementação e verificação foi simples. 
* O que foi mais desafiador: Na etapa 2, o código que utiliza a implementação por Async API não funciona corretamente na placa FRDM KL25Z que utilizamos. Portanto, houve a necessidade de adaptar o código por interrupção (Echo Bot) para realizar as tarefas feitas pelo código async, de modo a diferenciar momentos em que há apenas TX ou apenas RX.