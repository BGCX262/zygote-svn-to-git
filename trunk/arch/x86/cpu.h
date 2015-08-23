#ifndef __CPU_H__
#define __CPU_H_

//! initialize the processors
extern int i86_cpu_initialize ();

//! shutdown the processors
extern void i86_cpu_shutdown ();

//! get cpu vender
extern char* i86_cpu_get_vender ();


#endif //__CPU_H__
