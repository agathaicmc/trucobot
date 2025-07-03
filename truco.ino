/*
 --- pinout do leitor nfc ---
 RST          3 / qualquer PWM, configurar
 SDA(SS)      5 / qualquer PWM, configurar
 MOSI         11
 MISO         12 
 SCK          13
 --- pinout dos servos ---
 servo1       7
 servo2       8
 servo3       9
 servo4       10
 */

#include <SPI.h>
#include <Servo.h>
#include <MFRC522.h>

#define RST_PIN 3         // configuravel, pwm
#define SS_PIN 5         // configuravel, pwm

MFRC522 mfrc522(SS_PIN, RST_PIN);  // instancia para leitor NFC
Servo servo1, servo2, servo3, servo4;// instancias para os 4 servos

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(50);				// delay p inicializaçao do leitor
  servo1.attach(7,500,2500);  // conectar e inicializar todos os servos na posiçao 0 (pins 7,8,9,10)
  servo2.attach(8,500,2500);
  servo3.attach(9,500,2500);
  servo4.attach(10,500,2500);
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  servo4.write(0);
  Serial.println("");
	mfrc522.PCD_DumpVersionToSerial();	// mostrar versao do leitor (se vir algo estranho, provavelmente inicializou errado; reiniciar arduino)
	Serial.println(F("Esperando ler carta..."));
}

byte Card[16][4]; //matriz para armazenar os dados do ultimo NFC lido
int carta1, carta2, carta3; //variaveis para armazenar as cartas
int pjogador, probo, pontosemjogo; //variaveis para armazenar os pontos

// inicializa a matriz com zeros
void ResetInfo(){
	for (int i=0; i<=15; i++){
		for (int j=0; j<=4; j++){
			Card[i][j]=0;
		}
	}
}

// preenche a matriz com dados lidos de um tag
void ReadInfo(){

	ResetInfo();
  
  MFRC522::StatusCode status;

  byte buffer[18]; // 16 (data) + 2 (CRC)

  for (byte page=0; page<=15; page+=4){

    byte byteCount = sizeof(buffer);
    status= mfrc522.MIFARE_Read(page,buffer,&byteCount);
    if (status != mfrc522.STATUS_OK) {
      Serial.print(F("MIFARE_Read() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

    //      [page][index]
    //       0-15   0-3
    //  Card [16]   [4]
    //
    //      0-3   page 0
    //      4-7   page 1
    //      8-11  page 2
    //     12-15  page 3
    //  buffer[16+2]

    int i_=0;
    for (int i=page; i<=page+3; i++){
      for (int j=0; j<=3; j++){
        Card[i][j]=buffer[4*i_ + j];
      }
      i_++;
    }
  }
  
  //  This is to stop the card from sending the info over and over again
  mfrc522.PICC_HaltA();
}

// mostra os dados armazenados na matriz
void ShowInfo(){

  ReadInfo();

  Serial.println("--------------------------");
  for (int i=0; i<16; i++){
    for (int j=0; j<4; j++){
      Serial.print(Card[i][j],DEC);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println("--------------------------");
}

//mexe servo1
void jogar1(){
  servo1.write(150);
  delay(100);
  servo1.write(0);
}

//mexe servo2
void jogar2(){
  servo2.write(150);
  delay(100);
  servo2.write(0);
}

//mexe servo3
void jogar3(){
  servo3.write(150);
  delay(100);
  servo3.write(0);
}

//funcao hardcoded que recebe o numero de uma carta e retorna uma string com seu nome
String nomear(int numcarta){
  if(numcarta == 1) return("Quatro de ouros");
  if(numcarta == 2) return("Quatro de espadas");
  if(numcarta == 3) return("Quatro de copas");
  if(numcarta == 4) return("Cinco de copas");
  if(numcarta == 5) return("Cinco de espadas");
  if(numcarta == 6) return("Cinco de ouros");
  if(numcarta == 7) return("Cinco de paus");
  if(numcarta == 8) return("Seis de copas");
  if(numcarta == 9) return("Seis de ouros");
  if(numcarta == 10) return("Seis de paus");
  if(numcarta == 11) return("Seis de espadas");
  if(numcarta == 12) return("Sete de espadas");
  if(numcarta == 13) return("Sete de copas");
  if(numcarta == 14) return("Rainha de paus");
  if(numcarta == 15) return("Rainha de ouros");
  if(numcarta == 16) return("Rainha de espadas");
  if(numcarta == 17) return("Rainha de copas");
  if(numcarta == 18) return("Valete de espadas");
  if(numcarta == 19) return("Valete de paus");
  if(numcarta == 20) return("Valete de copas");
  if(numcarta == 21) return("Valete de ouros");
  if(numcarta == 22) return("Rei de ouros");
  if(numcarta == 23) return("Rei de paus");
  if(numcarta == 24) return("Rei de copas");
  if(numcarta == 25) return("Rei de espadas");
  if(numcarta == 26) return("As de copas");
  if(numcarta == 27) return("As de paus");
  if(numcarta == 28) return("As de ouros");
  if(numcarta == 29) return("Dois de ouros"); 
  if(numcarta == 30) return("Dois de paus");
  if(numcarta == 31) return("Dois de espadas");
  if(numcarta == 32) return("Dois de copas");
  if(numcarta == 33) return("Tres de ouros");
  if(numcarta == 34) return("Tres de espadas");
  if(numcarta == 35) return("Tres de copas");
  if(numcarta == 36) return("Tres de paus");
  if(numcarta == 37) return("Pica fumo");
  if(numcarta == 38) return("Espadilha");
  if(numcarta == 39) return("Copas");
  if(numcarta == 40) return("Zap");
  return("erro ao obter nome da carta");
}

//funcao hardcoded que recebe o hex de uma carta e retorna o valor de sua força no truco mineiro. volta -1 em caso de erro
int forca(int numcarta){
  if(numcarta == 1) return(1); // quatro de ouros
  if(numcarta == 2) return(1); // quatro de espadas
  if(numcarta == 3) return(1); // quatro de copas
  if(numcarta == 4) return(2); // cinco de copas
  if(numcarta == 5) return(2); // cinco de espadas
  if(numcarta == 6) return(2); // cinco de ouros
  if(numcarta == 7) return(2); // cinco de paus 
  if(numcarta == 8) return(3); // seis de copas 
  if(numcarta == 9) return(3); // seis de ouros
  if(numcarta == 10) return(3); // seis de paus
  if(numcarta == 11) return(3); // seis de espadas
  if(numcarta == 12) return(4); // sete de espadas
  if(numcarta == 13) return(4); // sete de copas
  if(numcarta == 14) return(5); // rainha de paus
  if(numcarta == 15) return(5); // rainha de ouros
  if(numcarta == 16) return(5); // rainha de espadas
  if(numcarta == 17) return(5); // rainha de copas
  if(numcarta == 18) return(6); // valete de espadas
  if(numcarta == 19) return(6); // valete de paus
  if(numcarta == 20) return(6); // valete de copas
  if(numcarta == 21) return(6); // valete de ouros
  if(numcarta == 22) return(7); // rei de ouros
  if(numcarta == 23) return(7); // rei de paus
  if(numcarta == 24) return(7); // rei de copas
  if(numcarta == 25) return(7); // rei de espadas
  if(numcarta == 26) return(8); // as de copas
  if(numcarta == 27) return(8); // as de paus
  if(numcarta == 28) return(8); // as de ouros
  if(numcarta == 29) return(9); // dois de ouros 
  if(numcarta == 30) return(9); // dois de paus
  if(numcarta == 31) return(9); // dois de espadas
  if(numcarta == 32) return(9); // dois de copas
  if(numcarta == 33) return(10); // tres de ouros
  if(numcarta == 34) return(10); // tres de espadas
  if(numcarta == 35) return(10); // tres de copas
  if(numcarta == 36) return(10); // tres de paus
  if(numcarta == 37) return(12); // pica fumo
  if(numcarta == 38) return(14); // espadilha
  if(numcarta == 39) return(16); // copas
  if(numcarta == 40) return(18); // zap
  return(-1);
}

//funcao que espera uma tag nfc ser aproximada do leitor, e volta o valor na posiçao (1,5)
int lercarta(){
  	// entrar em idle até aproximar um novo cartao nfc
  while( ! mfrc522.PICC_IsNewCardPresent()) {
		delay(500);
	}
  while(Card[5][0] == 0){
    ReadInfo();
    delay(50);
  }
  int valor = Card[5][0];
  ResetInfo();
  return valor;
}

//funcao que recebe uma carta e printa seu numero, forca e nome
void printarcarta(int carta){
  int f = forca(carta);
  Serial.print("A carta eh: ");
  Serial.println(carta, DEC);
  Serial.print("Sua força eh: ");
  Serial.println(f, DEC);
  Serial.print("Seu nome eh: ");
  Serial.println(nomear(carta));
}

void novarodada() {
    // cada rodada vale 1 ponto
    // (estou assumindo que é isso que "pontosemjogo" significa)
    // se for outra coisa então sinta-se livre pra criar outra variável rsrs
    pontosemjogo = 1;

    // 1 - primeiro, o robô tira 3 cartas aleatórias para si
    carta1 = random(1, 41);
    carta2 = random(1, 41);
    carta3 = random(1, 41);

    // aqui a gente coloca num vetor as forças das cartas na mão do robô
    int cartas[3] = { forca(carta1), forca(carta2), forca(carta3) };
    bool usada[3] = { false, false, false };

    // 2 - oponente começa: aguardar leitura da carta do oponente
    int cartaOponente1 = lercarta();
    int forcaOponente1 = forca(cartaOponente1);

    // 3 - checar se robô tem alguma carta mais forte
    int MenorMaisForte = -1;     
    int MenorForcaAcima = 1000;
    // (setei com esses valores pra não correr riscos)
    // (eu ia botar menorforcaacima = 100000, mas lembrei que tamo economizando memória)

    // loopzinho de cria para agora sim checar se o robô tem uma carta mais forte
    for (int i = 0; i < 3; i++) {
        if (!usada[i] && cartas[i] > forcaOponente1 && cartas[i] < MenorForcaAcima) {
            MenorForcaAcima = cartas[i];
            MenorMaisForte = i;
        }
    }

    if (MenorMaisForte >= 0) {
        // 4 - robô joga a carta de menor força que ainda vença o oponente (se possível)
        switch (MenorMaisForte) {
            case 0: jogar1(); break;
            case 1: jogar2(); break;
            case 2: jogar3(); break;
        }
        usada[MenorMaisForte] = true;

        // 5 - agora o robô joga a carta mais forte que sobrar
        int MaisForte = -1;
        int maiorForca = -1;
        for (int i = 0; i < 3; i++) {
            if (!usada[i] && cartas[i] > maiorForca) {
                maiorForca = cartas[i];
                MaisForte = i;
            }
        }
        // switch pq ngm merece else if else if else if
        switch (MaisForte) {
            case 0: jogar1(); break;
            case 1: jogar2(); break;
            case 2: jogar3(); break;
        }
        usada[MaisForte] = true;

        // 6 - agora a gente aguarda próxima jogada do oponente
        int cartaOponente2 = lercarta();
        int forcaOponente2 = forca(cartaOponente2);

        // 7 - comparar com a segunda carta do robô
        if (forcaOponente2 < maiorForca) {
            // robô ganha a rodada ebaaa
            pjogador += pontosemjogo;
        } else {
            // senão
            // 8 - a gente espera a jogada do cara
            int cartaOponente3 = lercarta();
            int forcaOponente3 = forca(cartaOponente3);

            // 9 - robô joga sua última carta
            int Ultima = -1;
            for (int i = 0; i < 3; i++) {
                if (!usada[i]) {
                    Ultima = i;
                    break;
                }
            }
            // vamo ver num switch qual carta jogar
            switch (Ultima) {
                case 0: jogar1(); break;
                case 1: jogar2(); break;
                case 2: jogar3(); break;
            }
            int forcaUltima = cartas[Ultima];

            // se a força da última carta for maior que a força da carta do oponente
            if (forcaUltima > forcaOponente3) {
              // robo ganha ponto (tô supondo que é assim que o probo original era pra funfar)
                probo += pontosemjogo;
            } else {
              // jogador ganha ponto (tô supondo que é assim que o pjogador original era pra funfar)
                pjogador += pontosemjogo;
            }
        }
    } else {
        // se não houver carta mais forte na primeira jogada:
        // dps vejo oq fazer nessa caraia, já foi tramposo escrever só um caso mano
        // vsf como a Miya fez tudo isso T-T
        // eu escrevi como se fosse em linguagem C, inclusive, temq ver se vai compilar
    }
}


void loop() {
	//carta1 = lercarta();
	//printarcarta(carta1);
	//carta2 = lercarta();
	//printarcarta(carta2);
	//carta3 = lercarta();
	//printarcarta(carta3);
  pjogador = 0;
  probo = 0;

  while(pjogador < 12 || probo < 12){
    novarodada();
  }

  Serial.println("FIM DO JOGO. Iniciando novo jogo em 10 segundos...");
  delay(10000);
}

