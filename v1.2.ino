char datas[] = "t36080000000025001100t03D80000000000000001t36480000000003005B24t31880000000000000000t34580000000000000000t36080000000000002800t3608000000B500001A01t36080000000000140004t36080000000047130502t32180000000000009B00t3218002000000403768Et36386700004F0080791Et42B800000000009B0675t420800000000000000AFt03D8000000000000000Ct31D80000000000000004t3938000000003C000000t31880000000000000003t42B80100000000FA0395t3638C010002F00000000";
//从上位机发送到arduino
char buffer[20][25];
char bufferguard;
char datasguard;
char id[20][3];
int DLC[20];
char databuffer[20][16];
char receiveBuffer[25];

int numdata = 0;
char flag = 1;
boolean reset_flag = false;
//int state = 1;

void setup(){
  Serial.begin(9600);
  //while(Serial.read() >= 0){}
}

void depart()
{
    for(int i=0;i<20;i++)
    {
        
        for(int j=0;j<20;j++)
        {
            buffer[i][j]=datas[i*20+j];
            
            //Serial.print(buffer);
        }
    }
    for(int i=0;i<20;i++)
    {
        for(int j=1;j<3;j++)
        {
            id[i][j]=buffer[i][j];
        }
        DLC[i]=int(buffer[i][4])-48;

       // Serial.print(DLC[i])
        
        for(int j=0;j<16;j++)
        {
            databuffer[i][j]=buffer[i][j+5];
        }
        
    }
}

/***
void depart()
{
    for(int i=0;i<20;i++)
    {
        datasguard=datas[4];
        for(int j=datasguard-5;j<datasguard*2+5;j++)
        {
            buffer[i][j]=datas[j];
        }
    }
}
***/

void send()
{
    for(int i=0;i<20;i++)
    {
        for(int j=0;j<DLC[i];j+=2)
        {
            //Serial.print(databuffer[i][j]);
            Serial.println(databuffer[i][j]+databuffer[i][j+1]);
        }
    
    }
}


void receive(){
  if(Serial.available()>0)
  {
    delay(100);
    //numdata = Serial.readBytes(receiveBuffer,21);
    Serial.readBytes(receiveBuffer,21);
    Serial.println(receiveBuffer);
   /* if(receiveBuffer[0] != 'C')
    {
      state == 0;
      Serial.println("state变成false");
      }*/

//if(state == 1)
//{
    //CANtool装置的版本信息
    //if(receiveBuffer[0] == 'V' && receiveBuffer.length() == 1) 
        if(receiveBuffer[0] == 'V') 
       {
          Serial.println("SV2.5-HV2.0");
          delay(100);
       }

     //CanTool装置Return: \r for Open OK, \BEL for Failure.
     //It works only after power up or if controller is in reset mode after command “C”. The following bit rates are available.

        if(receiveBuffer[0] == 'O' && receiveBuffer[1] == '1')
        {
          if(reset_flag)
          {
             Serial.println('\r');
             delay(100);
           }
           else
           {
             Serial.println("BEL");
            }
         }


//设置CAN总线的通信速率
        if(receiveBuffer[0] == 'S')
        {
            CAN_speed(receiveBuffer);
        }

  


        //Return: \r for Close OK, \BEL for Failure.It works only if the controller was set to Operation mode with “O1\r” command before.
        //if(receive_date == 'C')
      //  {
          
         // }

    //}  
  }
  while(Serial.read() >= 0){}
  for(int i=0;i<25;i++){
    receiveBuffer[i]='\0';
  }





  
}
void CAN_speed(char receiveBuffer[])
{

      switch(int(receiveBuffer[1])-48)
      {
        case 0:Serial.println("CAN总线的通信速率为：10Kbit");break;
        case 1:Serial.println("CAN总线的通信速率为：20Kbit");break;
        case 2:Serial.println("CAN总线的通信速率为：50Kbit");break;
        case 3:Serial.println("CAN总线的通信速率为：100Kbit");break;
        case 4:Serial.println("CAN总线的通信速率为：125Kbit");break;
        case 5:Serial.println("CAN总线的通信速率为：250Kbit");break;
        case 6:Serial.println("CAN总线的通信速率为：500Kbit");break;
        case 7:Serial.println("CAN总线的通信速率为：800Kbit");break;
        case 8:Serial.println("CAN总线的通信速率为：1MKbit");break;
        default:Serial.println("error");break;
        }
  }


void loop()
{

  depart();
  receive();
  
  //send();
  reset_flag = true;

}
