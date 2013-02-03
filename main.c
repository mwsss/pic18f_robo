#include p18f14k50.h
#include delays.h
#include .USBusb.h
#include .USBusb_function_cdc.h
#include HardwareProfile.h
#pragma config CPUDIV = NOCLKDIV
#pragma config USBDIV = OFF
#pragma config FOSC   = HS
#pragma config PLLEN  = ON
#pragma config FCMEN  = OFF
#pragma config IESO   = OFF
#pragma config PWRTEN = OFF
#pragma config BOREN  = OFF
#pragma config BORV   = 30
#pragma config WDTEN  = OFF
#pragma config WDTPS  = 32768
#pragma config MCLRE  = OFF
#pragma config HFOFST = OFF
#pragma config STVREN = ON
#pragma config LVP    = OFF
#pragma config XINST  = OFF
#pragma config BBSIZ  = OFF
#pragma config CP0    = OFF
#pragma config CP1    = OFF
#pragma config CPB    = OFF
#pragma config WRT0   = OFF
#pragma config WRT1   = OFF
#pragma config WRTB   = OFF
#pragma config WRTC   = OFF
#pragma config EBTR0  = OFF
#pragma config EBTR1  = OFF
#pragma config EBTRB  = OFF    
#include GenericTypeDefs.h
#include Compiler.h
#include usb_config.h
#include USBusb_device.h
#include USBusb.h
#include HardwareProfile.h
#pragma udata
char USB_In_Buffer[64];
char USB_Out_Buffer[64];
static void InitializeSystem(void);
void ProcessIO(void);
void USBDeviceTasks(void);
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void USBCBSendResume(void);
void UserInit(void);

#define REMAPPED_RESET_VECTOR_ADDRESS   0x00
#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS 0x08
#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS 0x18
#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS
 void Remapped_High_ISR (void)
 {
      _asm goto YourHighPriorityISRCode _endasm
 }
#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS
 void Remapped_Low_ISR (void)
 {
      _asm goto YourLowPriorityISRCode _endasm
 }

#pragma code
#pragma interrupt YourHighPriorityISRCode
 void YourHighPriorityISRCode()
 {
        #if defined(USB_INTERRUPT)
         USBDeviceTasks();
        #endif
 }
#pragma interruptlow YourLowPriorityISRCode
 void YourLowPriorityISRCode()
 {
 }
#pragma code
void main(void)
{   
    InitializeSystem();
TRISC = 0;
    while(1)
    {
        #if defined(USB_INTERRUPT)
            if(USB_BUS_SENSE && (USBGetDeviceState() == DETACHED_STATE))
            {
                USBDeviceAttach();
            }
        #endif
        #if defined(USB_POLLING)
        USBDeviceTasks(); 
        #endif
        ProcessIO();        
    }
}
static void InitializeSystem(void)
{
        ADCON1 = 0x0F;
    USBDeviceInit();
}
 

void ProcessIO(void)
{   
    BYTE numBytesRead;
	unsigned int iz = 0;
	unsigned int ix = 0;
	unsigned int iy = 0;
    if((USBDeviceState  CONFIGURED_STATE)(USBSuspendControl==1)) return;
    if(USBUSARTIsTxTrfReady())
    {
  numBytesRead = getsUSBUSART(USB_Out_Buffer,64);
  if(numBytesRead != 0)
  {
   BYTE i;
   for(i=0;inumBytesRead;i++)
   {
    switch(USB_Out_Buffer[i])
    {
     case 0x0A
     case 0x0D
      USB_In_Buffer[i] = USB_Out_Buffer[i];
      break;
     default
      USB_In_Buffer[i] = USB_Out_Buffer[i];
      break;
    }
   }
   putUSBUSART(USB_In_Buffer,numBytesRead);
ix=6+USB_In_Buffer[0]-48;0-70 
iy=27;
 for(iz=0;iz30;iz++){
  PORTCbits.RC0 = 1;
Delay1KTCYx(ix);
  PORTCbits.RC0 = 0;
Delay1KTCYx(30);
  Delay1KTCYx(240);
 };

 for(iz=0;iz30;iz++){
  PORTCbits.RC0 = 1;
Delay1KTCYx(iy);
  PORTCbits.RC0 = 0;
Delay1KTCYx(25);
  Delay1KTCYx(240);
 };

  }
 }
    CDCTxService();
}  end ProcessIO
void USBCBSendResume(void)
{
    static WORD delay_count;
    if(USBGetRemoteWakeupStatus() == TRUE) 
    {
        if(USBIsBusSuspended() == TRUE)
        {
            USBMaskInterrupts();
            USBSuspendControl = 0; 
            USBBusIsSuspended = FALSE; 
            delay_count = 3600U;        
            do
            {
                delay_count--;
            }while(delay_count);
            USBResumeControl = 1;
            delay_count = 1800U;
            do
            {
                delay_count--;
            }while(delay_count);
            USBResumeControl = 0; 
            USBUnmaskInterrupts();
        }
    }
}
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void pdata, WORD size)
{
    switch(event)
    {
        case EVENT_TRANSFER
            break;
        case EVENT_CONFIGURED 
            CDCInitEP();
            break;
    
        case EVENT_EP0_REQUEST
            USBCheckCDCRequest();
            break;
      
        case EVENT_TRANSFER_TERMINATED
            break;
        default
            break;
    }      
    return TRUE; 
}