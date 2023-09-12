# ESP32-Display-2432S028
Como utilizar o display touch com ESP32.


# Configurações:

    1 - ESP32 com Display de 240x320.
        - drive do display Touch: ILI9341V - (211-161007
        - Flash interno de 4 MB (32 bits).

    2 - Memória flash externa de 4 MB até 32 Mbit. winbond 25Q32JVSlQ (Não inclusa).
        - A memoria externa utiliza barramento paralelo. 

    3 - LCD Driver ( ILI9341V  ). 
      - Touch Driver: (U3:  método SPI resistivo XPT2046)

    4 - Barramentos de Expansão de IOs 2x.
    - P3: (GND, GPIO_35, GPIO_22, GPIO_21).
    - CN1: (GND, NC, GPIO_27, 3V3).

    5 - SLOT SD (Micro SD).

    6 - LED RGB (MHP5050RGBDT).
        - LED-R - VERMELHO: GPIO_4. 
        - LED-G - VERDE:    GPIO_17
        - LED-B - AZUL:     GPIO_16.

    7 - LED LDR: CDS (GT36516 ): GPIO_34.

    8 - Conexão de alimentação EXT: P1 (VIN, TX, RX, GND).

    9 - Saída de áudio (Amplificador de áudio SC8002B)
    P4: SPEAK(2=VO2,1=VO1) - GPIO26.

    10 - P1: Conector da base da fonte de alimentação.

    11 - Gravador USB: CH340C - AutoBoot. 

    1X - Anexos: 
    Caneta touch, cabo conector externo de 4 pinos, cabo USB.

# Adendo: Erro de design de conexão SPI:

    - A memoria que deve ser conectada ao barramento SPI de conexão U4 é exatamente paralela ao barramento SPI da memoria flash do ESP32, não sendo possível colocar duas memorias em paralelo. Remova uma para colocar a outra.

    - O Flash é duplicado devido a CS, DI e DO comuns entre o Flash interno U2 e o Flash externo U4.

# Layout do Circuito:

- A imagem do circuito completo pode ser acessada [aqui](https://macsbug.wordpress.com/2022/08/17/esp32-2432s028)

- O Esquematico da placa pode ser acessado [aqui]().

- Exemplos de código podem ser acessados [aqui]().

- A base desse conhecimento foi retirada [desse site.](https://macsbug.wordpress.com/2022/08/17/esp32-2432s028/)