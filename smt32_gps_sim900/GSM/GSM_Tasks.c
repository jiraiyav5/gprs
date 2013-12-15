/*
¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~
¨~                              -Ò×Í¨µç×ÓÔ­´´´úÂë-                                                      ¨~
¨~ ×÷Õß£ºÄª½¨ÁÖ                                                                                      ¨~
¨~   QQ: 1695627698                                                                               ¨~
¨~ ÍúÍú£ºÒ»Ò×Í¨µç×Ó¿Æ¼¼Ò»                                                                           ¨~
¨~ ÓÊÏä£ºmojianlin2011@126.com                                                                      ¨~
¨~ ÌÔ±¦×¨Âôµê£ºetmcu.taobao.com                                                                      ¨~
¨~ ÉùÃ÷:´Ë³ÌÐòÊÇÎªÒ×Í¨ÎÞÏß¼¼Êõ¿ª·¢°å¶øÐ´µÄ£¬ÇëÎðÓÃÓÚÉÌÒµÄ¿µÄ£¬ÈôÒªÒýÓÃÇë¸½ÉÏÎÒµÄÁªÏµ·½Ê½£¡          ¨~
¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~*/
#include "includes.h"
#include "GSM.h"
#include "GSM_TypeDefine.h"
//extern char app_buf[10] ;
extern com_app_t com_app;
                                                                                                
#if GSM_MODULE == DEF_ENABLED                                                                                      

static   OS_STK      GSM_TaskStk[GSM_TASK_STK_SIZE];
//static   OS_EVENT   *GSM_Sem;

extern  INT8U    GPSinf[100];
extern  const unsigned char GPSimage[153600];
extern  GSM_RX_STATE  GSM_RX_nState;    //´®¿ÚÊý¾Ý´¦Àí×´Ì¬
extern  INT8U      GSM_pCommand[GSM_MAX_CMD_LEN];    //
extern  INT16U    Test_counter;
extern  INT8U     Flag_CMT;
extern  INT8U     Flag_CMGR;
extern  INT8U     Flag_DATA;
extern  GSM_CMT     CMT;
extern  GSM_CSCA    CSCA;
extern  GSM_IPD     IPD;
extern  GSM_CLIP    CLIP;
extern  GSM_GSMLOC  GSMLOC;

extern  INT8U      GSM_pCommand[GSM_MAX_CMD_LEN];    //
extern  INT8U     GSM_pData[GSM_MAX_DATA_LEN];    //

static  void    GSM_InitOS(void);
static  void    GSM_Task(void *p_arg);
void GSM_START(void);
/*
¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~
¨~                              GSM³õÊ¼»¯                                                              ¨~
¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~*/
void  GSM_Init (void)
{
    GSM_RX_nState = RX_STATE_IDLE;
    if(GSM_ON1_OFF0()==0)GSM_START(); //Æô¶¯GSMÄ£¿é    ×¢£ºÈí¼þµ÷ÊÔÆÚ¼ä×îºÃÊÇÓÃÊÖ¶¯µÄ·½Ê½
    GSM_InitOS(); 
    GSM_InitTarget();
}
/*
¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~
¨~                              Æô¶¯GSMÄ£¿é                                                             ¨~
¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~*/
void GSM_START(void)
{
    INT8U t=0;
    //¸øGSMÄ£¿éµÄµçÔ´¿ØÖÆ½ÅÒ»¸öÏÂ½µÑØÆô¶¯Ä£¿é¡£
    GSM_POWER_KEY_HIGH();
    IWDG_ReloadCounter();//Î¹¹·
    OSTimeDlyHMSM(0, 0, 0,500);
    GSM_POWER_KEY_LOW();
    t=3;
    while(t--)
    {
        IWDG_ReloadCounter();//Î¹¹·
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
    GSM_POWER_KEY_HIGH();
    OSTimeDlyHMSM(0, 0, 0,500);
}
/*
¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~
¨~                             ´´½¨GSMÈÎÎñ                                                               ¨~
¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~*/
static  void  GSM_InitOS (void)
{
    INT8U  err;

    //GSM_Sem = OSSemCreate(0);
    //OSEventNameSet(GSM_Sem, (INT8U *)"GSM Signal", &err);

    (void)OSTaskCreateExt(GSM_Task,
                        (void *)0,                                  
                        &GSM_TaskStk[GSM_TASK_STK_SIZE - 1],         
                        GSM_TASK_PRIO,                              
                        GSM_TASK_PRIO,
                        &GSM_TaskStk[0],                             
                        GSM_TASK_STK_SIZE,
                        (void *)0,                                  
                        OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); 

#if OS_TASK_NAME_EN > 0
    OSTaskNameSet(GSM_TASK_PRIO, (INT8U *)"uC/GSM Task", &err);
#endif
}
/*
¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~
¨~                              GSMÈÎÎñ                                                                 ¨~
¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~¨~*/
static  void  GSM_Task (void *p_arg)
{
    //INT8U      err;
    INT16U  i=0;
    char s[2];

    #ifdef  GSM_SUPPORT_LBS
    INT16U  LBS_REFRESH_COUNT=0;
    #endif
    INT8U  INIT_FLAG=1,TP=0, buf_cnt = 0;
    ADXL345_Init();
    Key_Number_Init();

    if(GSM_ON1_OFF0()==0) { GSM_START(); } 
    
    while(1) 
    {
        IWDG_ReloadCounter();//Î¹¹·
        OSTimeDlyHMSM(0, 0, 0,100);
    
        //if(GSM_ON1_OFF0()==0) {GSM_START();}
             
        #ifdef GSM_SUPPORT_LBS
        LBS_REFRESH_COUNT++;     
        if(INIT_FLAG==0 && LBS_REFRESH_COUNT>=300) {//30Ãë¸üÐÂÒ»´Î»ùÕ¾¶¨Î»¾­Î³¶È
            LBS_REFRESH_COUNT=0;
            GSM_LBS_Refresh(); 
        }     
        #endif     
        if(i==150 && INIT_FLAG) {
            LCD_SHOW_CN15x16_String(0,100,"³õÊ¼»¯SIM900Ä£¿é...");
            GSM_Configuration();
            GSM_GPRS_TCP_Connect("zillionlin.vicp.cc","11657");
            INIT_FLAG=0;
            //counter=0;
        }//¿ª»úºó15Ãë£¬ÅäÖÃGSMÄ£¿é
        //////////////////////////////////////////////////////
        if(INIT_FLAG == 0)  {
            while(buf_cnt < 3)  {
                    if(com_app.buf_ok & (1<<buf_cnt))  {
                        sprintf(s, "%x",buf_cnt);
                        GSM_GPRS_SEND(s);
                        GSM_GPRS_SEND(com_app.app_buf[buf_cnt]);
                        com_app.buf_ok &= ~(1<<buf_cnt);
                        com_app.app_buf[buf_cnt][0] = '\0';
                    }
                    buf_cnt++;
            }
            buf_cnt = 0;
       }
        ///////////////////////////////////////////////////////
        TP=Get_Temperature();//¼ÆËãÎÂ¶È

#if (TFT_MODULE == DEF_ENABLED)

        if(Flag_DATA)  {//Ë¢ÐÂÆÁÄ» 
            TFT_ShowPicture(0,0,240,320,GPSimage);//ÏÔÊ¾±³¾°Í¼Æ¬
            TFT_ShowString(0,80,GSM_pCommand);
            TFT_ShowString(0,100,GSM_pData);
            Flag_DATA=0;
        }
        #ifdef GSM_SUPPORT_LBS 
        if(GSMLOC.isNew)/*ÏÔÊ¾»ùÕ¾¶¨Î»½á¹û*/  {
            LCD_SHOW_CN15x16_String(0,195,"»ùÕ¾¶¨Î»½á¹û£º");
            LCD_SHOW_CN15x16_String(0,215,"Î³¶È:");
            TFT_ShowString(35,215,GSMLOC.Latitude); 
            LCD_SHOW_CN15x16_String(120,215,"¾­¶È:");
            TFT_ShowString(155,215,GSMLOC.Longitude);
            //GSMLOC.isNew=0;             
        } 
        #endif    
        ADXL345_Test();//ÏÔÊ¾ÖØÁ¦¼ÓËÙ¶È´«¸ÐÆ÷ÄÚ²¿¹Ø¼ü¼Ä´æÆ÷ÄÚÈÝ
        //TFT_ShowNum(100,260,ADC_Result[ADC_TEMP]);//ÏÔÊ¾ADC×ª»»½á¹û
        //TFT_ShowString(0,260,"Temperature=");
        LCD_SHOW_CN15x16_String(0,280,"NTCÎÂ¶È:");        
        TFT_ShowNum(65,280,TP); //ÏÔÊ¾¼ÆËã³öÀ´µÄÎÂ¶ÈÖµ
        LCD_SHOW_CN15x16_String(110,280,"¼ÆÊýÆ÷:");        
        TFT_ShowNum(170,280,i++);//ÏÔÊ¾±¾ÈÎÎñÔËÐÐ´ÎÊý¼ÆÊýÆ÷
        LCD_SHOW_CN15x16_String(0,300,"GPSÊ±¼ä:");        
        // TFT_ShowNum(70,300,Test_counter);//ÏÔÊ¾´®¿Ú½ÓÊÕµ½µÄ×Ö½ÚÊý
#endif

        ///////////////////////////////////
        if(CMT.isNew)/*ÐÂ¶ÌÐÅ´¦Àí */  {
            CMT.isNew=0;
            //if( strcmp((char *)CMT.OP_CDM, "GPS?") == NULL && CMT.OP_Key==My_Key_Number)//ÐèÒªÃÜÂë
            if( strcmp((char *)CMT.OP_CDM, "GOOGLE") == NULL)/*²»ÐèÒªÃÜÂ**/   { 
                if(GPRMCData.DataValid=='A')    {
                    GSM_Send_TXT_CMG(CMT.PhoneNum,GPSinf);
                }  else  {
                    #ifdef GSM_Module_is_GU900
                    GSM_Send_GB_SMS(CMT.PhoneNum,"ÕýÔÚËÑË÷GPSÐÅÏ¢£¬ÇëÉÔºò...");
                    #else
                    GSM_Send_TXT_CMG(CMT.PhoneNum,"GPS is searching,please wait...!"); 
                    #endif
                }
            }  else if( strcmp((char *)CMT.OP_CDM, "LEDON") == NULL)/*µãÁÁLEDµ**/ {
                BSP_RLED_ON();
            }  else if( strcmp((char *)CMT.OP_CDM, "LEDOFF") == NULL)/*¹ØµôLEDµÆ */  {
                BSP_RLED_OFF();
            }  else if(strcmp((char *)CMT.OP_CDM, "CALLME") == NULL)/*ºô½Ð·¢¶ÌÐÅµÄÈËµÄÊÖ»úºÅÂë*/  {
                GSM_CALL(CMT.PhoneNum);
            }  else if( strcmp((char *)CMT.OP_CDM, "STATUS") == NULL)  {//·¢ËÍÒ»Ìõ°üº¬Ê±¼ä¡¢¾­Î³¶È¡¢ÎÂ¶ÈÐÅÏ¢µÄÖÐÎÄ¶ÌÐÅ
                #ifdef GSM_Module_is_GU900
                GSM_Send_MyStatus(CMT.PhoneNum);
                #else
                GSM_Send_CN_MSG2(CMT.PhoneNum,STATUS_str);
                #endif
            }
            #if (TFT_MODULE     == DEF_ENABLED)
            TFT_ShowString(0,180,CMT.PhoneNum);//ÏÔÊ¾·¢¶ÌÐÅÈËµÄÊÖ»úºÅÂë
            TFT_ShowString(0,200,(INT8U *)CMT.Date_Time);//ÏÔÊ¾·¢ËÍÊ±¼ä
            TFT_ShowString(0,220,CMT.Content);//ÏÔÊ¾¶ÌÐÅÄÚÈÝ
            TFT_ShowString(0,240,CMT.OP_CDM); //ÏÔÊ¾¶ÌÐÅ²Ù×÷ÃüÁî
            TFT_ShowString(50,240,(INT8U *)CMT.OP_Key_ASCII);//ÏÔÊ¾¶ÌÐÅ²Ù×÷ÃüÁîÖÐµÄÃÜÂë(×Ö·ûÐÍµÄ)
            TFT_ShowNum  (120,240,CMT.OP_Key); //ÏÔÊ¾¶ÌÐÅ²Ù×÷ÃüÁîÖÐµÄÃÜÂë(ÊýÖµÐÍµÄ)
            #endif
        }
        //////////////////////////////////////////////
        //ÍøÂçÊý¾Ý±ØÐë´øÊ¶±ðÍ·+IPDºÍÊý¾Ý³¤¶È£¬ÀýÈç£º\r\n+IPD,4:Google Map?\r\n
        if(IPD.isNew)/*ÍøÂçÊý¾Ý¿ØÖÆ */  {
            #if (TFT_MODULE     == DEF_ENABLED)
            TFT_ShowNum(0,200,IPD.DataLen);
            TFT_ShowString(0,220,IPD.Data);
            #endif        
            if( strcmp((char *)IPD.Data, "Google Map?") == NULL)  { 
                if(GPRMCData.DataValid=='A')  {
                    GSM_GPRS_SEND(GPSinf);
                    //GSM_Send_TXT_CMG("15990472896",GPSinf);
                }  else  {
                    GSM_GPRS_SEND("GPS is searching,please wait!");
                    //GSM_Send_TXT_CMG("15990472896","GPS is searching,please wait!");
                }
            }
            IPD.isNew = 0;
        }
        ///////////////////////////////
        if(CLIP.RING) /*À´µç´¦Àí*/ {
            CLIP.RING=0;
            #if (TFT_MODULE == DEF_ENABLED)
            TFT_ShowString(0,180,"RING");
            TFT_ShowNum  (120,240,CLIP.COUNT);
            #endif
            if(CLIP.COUNT>2) {GSM_Answer_CALL(); CLIP.COUNT=0; }  //ÏìÁå2Éùºó×Ô¶¯½ÓÌýµç»°
        }
        if( !Key_Left() )//°´×ó¼ü
        { 
            BSP_RLED_ON();        //µãÁÁLEDµÆ
            TFT_ShowPicture(0,0,240,320,GPSimage);//ÏÔÊ¾±³¾°Í¼Æ¬,¼´ÇåÆÁ
            // GSM_GPRS_UDP_Connect("etmcu.xicp.net","5800");    //Á¬½ÓÔ¶³ÌTCP·þÎñÆ÷
            //GSM_GPRS_TCP_Connect("117.83.54.228","5800");    //Á¬½ÓÔ¶³ÌTCP·þÎñÆ÷
            GSM_GPRS_TCP_Connect("zillionlin.vicp.cc","11657");
        }
        if( !Key_Right() )/*°´ÓÒ¼ü */ {
            BSP_RLED_OFF();    //Ï¨ÃðLEDµÆ
            //GSM_GPRS_SEND("$GPRMC,060525.491,A,3156.9445,N,12025.8057,E,0.59,181.00,200309,,,A*69");//·¢ËÍ²âÊÔÊý¾Ý
            GSM_GPRS_SEND("===================");
            GSM_GPRS_SEND(com_app.app_buf[0]);
        }
        if( !Key_Up() )/*°´ÉÏ¼ü */ {
            GSM_Answer_CALL();    //À´µçÊ±£¬°´ÉÏ¼ü½ÓÌýµç»°
            GSM_GPRS_SEND(com_app.app_buf[1]);
        } 
        if( !Key_Down()  )/*°´ÏÂ¼ü */ {

            USART_SendData(USART1 , com_app.com_cnt);

           // USART_SendData(USART1 , com_app.buf_ok);
            
            GSM_GPRS_SEND(com_app.app_buf[0]);
            //while(!Key_Down());
            //GSM_Hang_UP();        //Í¨»°ÖÐ£¬°´ÏÂ¼ü¹Ò¶Ïµç»°
            //GSM_Send_CN_MSG("8615990472896","ÄãºÃ£¡Hellow World!");//²âÊÔ·¢ËÍÒ»ÌõÖÐÓ¢ÎÄ»ìºÏµÄPDU¶ÌÐÅ
        }
    }//while(1) 
}

#endif //GSM_MODULE == DEF_ENABLED
































