#include <analogWrite.h>  //biblioteca para leitura da entrada analogia

//definição das variáveis com inicialização de valores ou portas

const int trigPin = 23;  
//trig: sinal enviado 
const int echoPin = 22;
//echo: sinal retornado após bater no objeto
byte motorPin = 18; //controlar o pwm
int counter=0; //contador para dar um primeiro impulso no motor
float potenciometroPin = 34;  //pino para conexão do potenciometro

//define sound speed in cm/uS
#define SOUND_SPEED 0.034

//declaraçao de variáveis 

long duration;
float distanceCm;
float distanceReal;
float alturaDesejada;
double powerOut;

void setup() {
  //inicio do monitor serial e pinos de entrada e saida 
  Serial.begin(115200); // Inicia comunicação serial 
  pinMode(trigPin, OUTPUT); //declara trigPin como um Output
  pinMode(echoPin, INPUT); // declara echoPin como Input
  pinMode(motorPin, OUTPUT);// declara motorPin como Output
}

void loop() {
  //leitura da altura desejada de acordo com o potenciometro
  alturaDesejada = analogRead(potenciometroPin);
  delay(100);
  //chama a função de distancia 
  calculeDistancia();
  /*
   * Essa subtração abaixo é realizada por conta de que com a bolinha bem proxima ao sensor 
   * a altura mostrada no Monitor serial é em torno de 0 cm, logo a altura próximo do cooler 
   * seria de 27 cm.Portanto, tivemos que fazer esta alteração para que o 0 cm ficasse no eixo 
   * oposto do sensor. Onde elá iniciará efetivamente seu percurso
   * 
   * observação: 27 é a altura efetiva que a bolinha poderá se movimentar no nosso caso
  */
  distanceReal = 27 - distanceCm;
  //laço para dar uma alavancada no motor 
  if(counter==0){
    analogWrite(motorPin,230);
    delay(5000);
    counter = counter + 1;  
  }
  else{
    /* Os valores abaixo vieram dos seguintes locais:
     *  255 = valor máximo recebido do pwm
     *  4095 = valor máximo recebido pelo potenciometro
     *  alturaDesejada: será lida pelo potenciometro
     */
    powerOut = (255*alturaDesejada)/4095;
    //liga o cooler com a potencia que foi recebida 
    analogWrite(motorPin, powerOut);
    //e mostra no Monitor Serial 
    Serial.print("Altura:");
    Serial.println(distanceReal);
  }
}
void calculeDistancia(){
  digitalWrite(trigPin, LOW); // Limpa o trigPin
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // Aciona o trigPin para HIGH por 10 microssecondos
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);// leitura do echoPin, returns the sound wave travel time in microseconds
  distanceCm = duration * SOUND_SPEED/2; // Calcula a distância
  
  // Para printar a distancia no Serial Monitor, porém como faremos mais mudanças 
  //printaremos a altura em outra parte, mas você pode usar esta para teste 
  // caso queira 
  
  //Serial.print("Distance (cm): ");
  //Serial.println(distanceCm);   

  //Serial.print("Potenciometro:");
  //Serial.println(alturaDesejada);
  
  }
  
