#ifndef DEVICE_H_
#define DEVICE_H_


struct deviceParams
{
   char* Data_0; //0
   void (*Data_4)(unsigned); //4
   void (*Data_8)(void); //8
   int (*Data_12)(unsigned); //12
   int Data_16; //16
   //20
};


struct Struct_21b8818c
{
   int Data_0; //0
   int Data_4; //4
   int Data_8; //8
   //12
};


extern int device_init(void);
extern int device_add(unsigned, struct deviceParams*);
extern int func_21b8961c(void);
extern int device_set_use(unsigned, unsigned);
extern int SYS_DeviceGetStatus(unsigned, struct Struct_21b8818c*);
extern int SYS_DeviceWakeup(void);


#endif /*DEVICE_H_*/
