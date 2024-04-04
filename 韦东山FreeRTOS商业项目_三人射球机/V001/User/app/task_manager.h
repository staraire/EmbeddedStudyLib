
#ifndef __task_manager_h__
#define __task_manager_h__

/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//task_start.c
extern void do_create_start_task(void);

//task_listen.c
extern void do_create_listen_task(void);
extern TaskHandle_t* get_task_listen_Handle(void);

//task_player1.c
extern void do_create_player1_task(void);
extern TaskHandle_t* get_task_player1_Handle(void);

//task_player2.c
extern void do_create_player2_task(void);
extern TaskHandle_t* get_task_player2_Handle(void);

//task_player3.c
extern void do_create_player3_task(void);
extern TaskHandle_t* get_task_player3_Handle(void);

//task_player4.c
extern void do_create_player4_task(void);
extern TaskHandle_t* get_task_player4_Handle(void);



//task_host.c
extern void do_create_host_task(void);
extern TaskHandle_t* get_task_host_Handle(void);


//task_ui.c
extern void do_create_ui_task(void);
extern TaskHandle_t* get_task_ui_Handle(void);

#endif

