#include "syslib.h"

int sys_chrt(proc_ep,  deadline)
endpoint_t proc_ep;
long deadline;
{

  message m;
  m.m2_i1 = proc_ep;/*m_m2.m2i1*/
  m.m2_l1 = deadline;
  return(_kernel_call(SYS_CHRT, &m));
}
/* The kernel call implemented in this file:  
*   m_type: SYS_SETALARM  消息类型：SYS_SETALARM  *
* The parameters for this kernel call are:  
*    m2_l1: ALRM_EXP_TIME  (alarm's expiration time) ALRM_EXP_TIME 时钟警报时间  
*    m2_i2: ALRM_ABS_TIME  (expiration time is absolute?) ALRM_ABS_TIME 消耗时间是绝对还是相对  
*    m2_l1: ALRM_TIME_LEFT  (return seconds left of previous) ALRM_TIME_LEFT：返回还有多少秒剩余  
*/ 
 


