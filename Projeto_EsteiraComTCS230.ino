//********************************************************
//Programa para identificacao de cores com sensor TCS230
//Loja Eletrogate

//Desenvolvedor : Amwar Hachouche /Vitor Vidal
//********************************************************
//Pinos de conexao do modulo TCS230  
const int s0 = 9;  
const int s1 = 10;  
const int s2 = 13;
const int s3 = 12;  
const int out = 11;   
int RedLed = 2;  
int GreenLed = 4;  
int BlueLed = 7;  
int colorIsRed = 1;
int colorIsBlue = 2;
int colorIsGreen = 3;
int colorIsWhite = 3;

//Contador
int CountR;
int CountB;
int CountG;
int ultimaCor=0;
int NoColor =0;
    
//Variaveis que armazenam o valor das cores  
int red = 0;  
int green = 0;  
int blue = 0;  

//Variaveis usadas na calibracao
int mediaR;
int mediaB;
int mediaG;

int mediaRR;
int mediaBR;
int mediaGR;

int mediaRG;
int mediaBG;
int mediaGG;

int mediaRB;
int mediaBB;
int mediaGB;

    
void setup()   
{  
  CountR = CountG = CountB = 0;
  //configura as entradas e saidas
  pinMode(s0, OUTPUT);  
  pinMode(s1, OUTPUT);  
  pinMode(s2, OUTPUT);  
  pinMode(s3, OUTPUT);  
  pinMode(out, INPUT);  
  pinMode(RedLed, OUTPUT);  
  pinMode(GreenLed, OUTPUT);  
  pinMode(BlueLed, OUTPUT); 
  //Seleciona a escala de frequncia para 20% 
  digitalWrite(s0, HIGH);  
  digitalWrite(s1, LOW);  
  //Inicializa a porta serial
  Serial.begin(9600);  
  delay(500);
  //chama funcao para calibrar o sensor
  calibraSensor();
}  
    
void loop() 
{  
  int CorIdentificada;

  //chama funçao para ler o sensor
  Readcolor();
  //imprimi valores na tela
 /* Serial.print(" Leitura do sensor por filtro: "); 
  Serial.println(); 
  Serial.print(" Azul : ");  
  Serial.print(blue, DEC); 
  Serial.print(" Vermelho :");  
  Serial.print(red, DEC);  
  Serial.print(" Verde : ");  
  Serial.print(green, DEC); 
  Serial.println();  */


  CorIdentificada = IdentificaCor();

  //Verifica se a cor vermelha foi detectada  
  if (CorIdentificada == colorIsRed)  
  {  
       if(ultimaCor != colorIsRed)
         CountR++;
   
       Serial.print(" Cor Identificada Vermelho: "); 
       Serial.print(CountR,'DEC'); 
       digitalWrite(RedLed, HIGH); //Acende o led vermelho  
       digitalWrite(GreenLed, LOW);  
       digitalWrite(BlueLed, LOW);  
       delay(10);
       ultimaCor = colorIsRed;
  }  
  
  //Verifica se a cor azul foi detectada  
  else if (CorIdentificada == colorIsBlue)   
  {  
       if(ultimaCor != colorIsBlue)
         CountB++;
       Serial.print(" Cor Identificada: Azul - Contador:");  
       Serial.print(CountB,'DEC');
       digitalWrite(RedLed, LOW);  
       digitalWrite(GreenLed, LOW);  
       digitalWrite(BlueLed, HIGH); //Acende o led azul  
       delay(10);
       ultimaCor = colorIsBlue;
  }  

  //Verifica se a cor verde foi detectada  
  else if (CorIdentificada == colorIsGreen)  
  {         
        if(ultimaCor != colorIsGreen)
             CountG++;
             
       Serial.print(" Cor Identificada: Verde Contador: ");  
       Serial.print(CountG,'DEC');
       digitalWrite(RedLed, LOW);  
       digitalWrite(GreenLed, HIGH); //Acende o led verde  
       digitalWrite(BlueLed, LOW);
       delay(10);   
       ultimaCor = colorIsGreen; 
  }  
  
  else
  {
    ultimaCor = NoColor;
    Serial.print(" Cor Identificada: Nenhuma ");  
  }
  
  
  Serial.println(); 
  Serial.println();  
  digitalWrite(RedLed, LOW);  
  digitalWrite(GreenLed, LOW); 
  digitalWrite(BlueLed, LOW); 
  //delay(10);   
  
}  
    
void Readcolor()  
{  
  //le saida com filtro vermelho
  digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);  
  delay(50);
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  
  
  //Le saida com filtro azul
  digitalWrite(s3, HIGH);  
  delay(50);
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  
  //le saida com filtro verde
  digitalWrite(s2, HIGH);  
  delay(50);
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  
  /*
  Serial.print(" Leitura do sensor por filtro: "); 
  Serial.println(); 
  Serial.print(" Azul : ");  
  Serial.print(blue, DEC); 
  Serial.print(" Vermelho :");  
  Serial.print(red, DEC);  
  Serial.print(" Verde : ");  
  Serial.print(green, DEC); 
  Serial.println();  */
}

void calibraSensor()
{
  int i;
  int controlLoop = true;
  int byteRead;
  int calibrouB = true;
  int calibrouR = true;
  int calibrouG = true;
  
  Serial.write("Voce esta na funcao de calibracao do TCS230"); 
  Serial.println();  
  Serial.write("Digite 1 para vermelho - 2 para azul - 3 para verde - 4 para fechar a calibracao - 5 para reinicializar a calibracao");  
  Serial.println(); 
  delay(100);
  
  
  while(controlLoop)
  {
   
      if (Serial.available())  //verifica se tem dados diponível para leitura
      {
        byteRead = Serial.read(); //le bytwe mais recente no buffer da serial
        Serial.write(byteRead);   //reenvia para o computador o dado recebido
      }
      
      if( (byteRead == '1') && calibrouR)
      { 
        calibrouR = false;
        Serial.print("calibrando cor vermelho");
        Serial.println();
        
        mediaR = mediaB = mediaG = 0;
        for(i=0; i<20; i++)
        {
          Readcolor();
          mediaR +=red;
          mediaB +=blue;
          mediaG += green;
          delay(50);
        }
       
        mediaRR = mediaR/20;
        mediaBR = mediaB/20;
        mediaGR = mediaG/20;
        Serial.print("calibracao cor vermelha finalizada");
        Serial.println();
        Serial.print(mediaRR),DEC;
        Serial.println();
        Serial.print(mediaBR,DEC);
        Serial.println();
        Serial.print(mediaGR,DEC);
        Serial.println();
        
        Serial.write("Digite 1 para vermelho - 2 para azul - 3 para verde - 4 para fechar a calibracao - 5 para reinicializar a calibracao");  
        Serial.println();
      }
      
      if( (byteRead == '2') && calibrouB )
      { 
        calibrouB=false;
        Serial.print("calibrando cor azul");
        Serial.println();
        
        mediaR = mediaB = mediaG = 0;
        for(i=0; i<20; i++)
        {
          Readcolor();
          mediaR += red;
          mediaB += blue;
          mediaG += green;
          delay(50);
        }
       
        mediaRB = mediaR/20;
        mediaBB = mediaB/20;
        mediaGB = mediaG/20;
        Serial.print("calibracao cor azul finalizada");
        Serial.println();
        Serial.print(mediaRB,DEC);
        Serial.println();
        Serial.print(mediaBB,DEC);
        Serial.println();
        Serial.print(mediaGB,DEC);
        Serial.println();
        Serial.write("Digite 1 para vermelho - 2 para azul - 3 para verde - 4 para fechar a calibracao - 5 para reinicializar a calibracao");  
        Serial.println();
      }
      
      
      if( (byteRead == '3') && calibrouG )
      { 
        calibrouG = false;
        Serial.print("calibranado a cor verde");
        Serial.println();
        mediaR = mediaB = mediaG = 0;
        for(i=0; i<20; i++)
        {
          Readcolor();
          mediaR += red;
          mediaB += blue;
          mediaG += green;
          delay(50);
        }
       
        mediaRG = mediaR/20;
        mediaBG = mediaB/20;
        mediaGG = mediaG/20;
        Serial.print("calibracao cor verde finalizada");
        Serial.print(mediaRG,DEC);
        Serial.println();
        Serial.print(mediaBG,DEC);
        Serial.println();
        Serial.print(mediaGG,DEC);
        Serial.println();
        
        Serial.write("Digite 1 para vermelho - 2 para azul - 3 para verde - 4 para fechar a calibracao - 5 para reinicializar a calibracao");  
        Serial.println();
      }
      
      if(byteRead == '4' )
      { 
        controlLoop = false;
      }
      
      if(byteRead == 'g' )
      { 
        calibrouR = true;
        calibrouB = true;
        calibrouG = true;
      }
  }

}

int IdentificaCor()
{
  //verifica se a cor e verde
  if( (blue>(mediaBG-15) && blue<(mediaBG+15)) && (red>(mediaRG-15) && red<(mediaRG+15)) && (green>(mediaGG-15) && green<(mediaGG+15)))
    return colorIsGreen;
    
  //verifica se a cor e azul
  else if((blue>(mediaBB-15) && blue<(mediaBB+15)) && (red>(mediaRB-15) && red<(mediaRB+15)) && (green >(mediaGB-15) && green<(mediaGB+15)) )
    return colorIsBlue;
  
  //Verifica se a cor e vermelha  
  else if((blue>(mediaBR-15) && blue<(mediaBR+15)) && (red>(mediaRR-15) && red<(mediaRR+15)) && (green >(mediaGR-15) && green<(mediaGR+15)) )
    return colorIsRed;
  }
