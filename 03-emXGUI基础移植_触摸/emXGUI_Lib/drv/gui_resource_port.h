#ifndef __GUI_RESOURCE_PORT_H
#define	__GUI_RESOURCE_PORT_H


#include "stm32h7xx.h"


#include	"emXGUI.h"
#include	"gui_drv_cfg.h"
//#include "backend_res_mgr.h"
#ifdef __cplusplus
extern "C" {
#endif  
  
/* Ŀ¼��Ϣ���� */
typedef struct 
{
	char 	name[40];  /* ��Դ������ */
	u32  	size;      /* ��Դ�Ĵ�С */ 
	u32 	offset;    /* ��Դ����ڻ���ַ��ƫ�� */
}CatalogTypeDef;


/* ���ڱ���Ƿ�����Դ�ļ��޷��ҵ� */
extern BOOL res_not_found_flag;

BOOL RES_DevInit(void);
U32 RES_DevGetID(void);
BOOL RES_DevWrite(u8 *buf,u32 addr,u32 size);
BOOL RES_DevRead(u8 *buf,u32 addr,u32 size);
int RES_DevEraseSector(u32 addr);
void RES_DevTest(void);

s32 RES_GetOffset(const char *res_name);
s32 RES_GetInfo_AbsAddr(const char *res_name, CatalogTypeDef *dir);

BOOL FS_Load_Content(char *file_name, char** buf, u32* size); 
BOOL RES_Load_Content(char *file_name, char** buf,u32* size);
BOOL PIC_JPEG_Draw_FS(HDC hdc, int x, int y, char *file_name);
 /**
  * @brief  �ͷŶ�̬�����buf�ռ�
  * @retval ��
  */
static __inline void RES_Release_Content(char **buf)
{
    /* �ͷſռ� */
    GUI_VMEM_Free(*buf);
}

#ifdef	__cplusplus
}
#endif
#endif /* __GUI_RESOURCE_PORT_H */
