
/*
 * S.A.R.A - Sistema Autônomo para Residências automáticas
 * Projeto CEE | Turma 1D - Ciência da computação
 * Criado por: Leonardo Borim Silva
 */

/**************************************** DEFINIÇÕES ******************************************* */

//Relês controlados pelo Sensor de Som e seus respectivos pinos
#define releUm 7
#define releDois 6

//Relês controlados pelo Módulo Bluetooth e seus respectivos pinos
#define releTres 5
#define releQuatro 4

//Pino de controle do sensor de som
#define sensorSom 3

/**************************************** VARIÁVEIS ******************************************** */

String voz;                 //Variável responsável por receber os dados de voz do usuário em forma de String.
int delayfinal = 100;       //Representação de um tempo em milissegundos, esse tempo é aguardado pelo programa para que se inicie novamente o loop.  
int duracaoPalma = 200;     //Representação de um tempo em milissegundos, é o tempo que dura o som de uma palma, precisa ser calibrado entre 100 e 250. 
int intervaloPalmas = 500;  //Representação de um tempo em milissegundos, é o intervalo máximo permitido entre uma sequência de palmas.  
int quantidadePalmas = 0;   //Quantidade de palmas registradas.
long momentoPalma = 0;      //Marcador usado para a detecção das palmas, será utilizado junto com a função millis. 
long esperaPalmas = 0;      //Marcador usado para contagem dos intervalos de tempo, será utilizado junto com a função millis. 

/************************************* CONFIGURAÇÕES INICIAIS ************************************ */

//OBS: A lógica implementada para essa aplicação foi invertida, ou seja: HIGH == Desligado; LOW == Desligado

//Função para acionar dois relês de uma vez
void allon(){
  digitalWrite(releTres, LOW);
  digitalWrite(releQuatro, LOW);
}

//Função para desligar dois relês de uma vez
void alloff(){
  digitalWrite(releTres, HIGH);
  digitalWrite(releQuatro, HIGH);
}

void setup() {
  //Comando que chama o monitor serial em bits por segundo
  Serial.begin(9600);
  
  // Definição da função de cada pino, se vão receber (INPUT) ou enviar (OUTPUT) informações.
  pinMode(sensorSom,INPUT);
  pinMode(releUm,OUTPUT);
  pinMode(releDois,OUTPUT);
  pinMode(releTres, OUTPUT);
  pinMode(releQuatro, OUTPUT);
 
  // Inicia todos os relés na posição na qual os contatos estão desligados.
  digitalWrite(releUm,HIGH);
  digitalWrite(releDois,HIGH);
  digitalWrite(releTres,HIGH);
  digitalWrite(releQuatro,HIGH);

}
/************************************* INICIO DO LAÇO DE EXECUÇÃO ************************************ */

void loop() {

  /****************************** ACIONAMENTO POR SENSOR DE SOM ************************************* */
  
  //Faz a leitura digital do sensor de som, quando este sensor detecta som ele desliga a porta de entrada, mudando seu estado para LOW e quando não detecta mantem em HIGH.
  int leituraSom = digitalRead(sensorSom);
  
  //Ações quando o sensor detectar som. 
  if (leituraSom == LOW) {
    
     //Marca o momento da palma, soma a quantidade de palmas e aguarda um intervalo para não marcar a mesma palma mais de uma vez. 
     if (momentoPalma == 0) {
        momentoPalma = esperaPalmas = millis();
        quantidadePalmas = quantidadePalmas + 1; 
     } else if ((millis() - momentoPalma) >= duracaoPalma) {
        momentoPalma = 0;
     }
  }

  //Verifica se nenhuma palma mais pode ser dada, e em seguida faz o acionamento dos relés conforme o número de palmas já registrado. 
  if (((millis() - esperaPalmas) > intervaloPalmas) && (quantidadePalmas != 0)) {
    
    if(quantidadePalmas == 1){
       digitalWrite(releUm, !digitalRead(releUm));          //O sinal de exclamação inverte a condição do relé, se estava ligado será desligado e vice versa. 
       }
    if(quantidadePalmas == 2){
       digitalWrite(releDois, !digitalRead(releDois));      //O sinal de exclamação inverte a condição do relé, se estava ligado será desligado e vice versa. 
       }  
    if(quantidadePalmas == 3){
       digitalWrite(releDois, !digitalRead(releDois));      //O sinal de exclamação inverte a condição do relé, se estava ligado será desligado e vice versa. 
       digitalWrite(releUm, !digitalRead(releUm));
       }  
   
     delay(delayfinal);     //Tempo de espera para continuar o programa, importante para evitar efeitos de possiveis detecções truncadas de ecos e reverberações no som. 
     quantidadePalmas = 0;  //Retoma a condição inicial da quantidade de palmas. 
   }

  /************************** ACIONAMENTO POR COMANDO DE VOZ VIA BLUETOOTH ************************** */

  while (Serial.available()){     //Checa se não há nenhum byte restante
      delay(10);                  //Tempo de espera adicionado para estabilização
      char c = Serial.read();     //Entrada de dados pela função serial
      if (c == '#') {
        break;
        }                         //Sai do loop quando # é declarada após a palavra dita
        voz += c;                 //Atalho para voice = voice + c 
    }
  if (voz.length() > 0) {
    Serial.println(voz);
    }

  //-------------Controle de ambos os relês---------------// 
   
  //LIGAR todos os relês chamando a função
  if(voz == "*Ligar todos") {
    allon(); 
    }
   
  //DESLIGAR ambos os relês chamando a função
  else if(voz == "*desligar todos"){
    alloff();
    }
    
  //----------Controle de cada relê separadamente----------//

  //LIGAR um de cada vez:
  else if(voz == "*ligar um") {
    digitalWrite(releTres, LOW);
    }
  else if(voz == "*ligar dois") {
    digitalWrite(releQuatro, LOW);
    }
    
  // DESLIGAR um de cada vez: 
  else if(voz == "*desligar um") {
    digitalWrite(releTres, HIGH);
    }
  else if(voz == "*desligar dois") {
    digitalWrite(releQuatro, HIGH);
    }
  voz="";
}  
