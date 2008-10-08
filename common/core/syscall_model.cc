#include "syscall_model.h"
#include "sys/syscall.h"

bool called_open = false;

SyscallMdl::SyscallMdl()
{
}

void SyscallMdl::runExit(int rank, CONTEXT *ctx, SYSCALL_STANDARD syscall_standard)
{
   //if only the code below worked in enter...
   //int return_addr = PIN_GetContextReg(ctx, REG_INST_PTR);
   //return_addr += 2;
   //PIN_SetContextReg(ctx, REG_INST_PTR, return_addr);
   //PIN_ExecuteAt(ctx);

   if(called_open)
   {
      PIN_SetContextReg(ctx, REG_EAX, 0x8);
      called_open = false;
   }
}

void SyscallMdl::runEnter(int rank, CONTEXT *ctx, SYSCALL_STANDARD syscall_standard)
{

   int syscall_number = PIN_GetSyscallNumber(ctx, syscall_standard);

   switch(syscall_number)
   {
      case SYS_open:
      {
         char *path = (char *)PIN_GetSyscallArgument(ctx, syscall_standard, 0);
         if(!strcmp(path,"./input"))
         {
            called_open = true;
            cout << "open(" << path << ")" << endl;

            // safer than letting the original syscall go
            PIN_SetSyscallNumber(ctx, syscall_standard, SYS_getpid);
         }

         break;
      }
      // case SYS_write:
      // {
      //    cout << "write(";
      //    cout << (int)PIN_GetSyscallArgument(ctx, syscall_standard, 0) << ", \"";
      //    cout << (char *)PIN_GetSyscallArgument(ctx, syscall_standard, 1) << "\")" << endl;
      //    break;
      // }
      // case SYS_close:
      // {
      //    cout << "close(" << (int)PIN_GetSyscallArgument(ctx, syscall_standard, 0) << ")" << endl;
      //    break;
      // }
      // case SYS_exit:
      //    cout << "exit()" << endl;
      //    break;
      case -1:
         break;
      default:
//         cout << "SysCall: " << (int)syscall_number << endl;
         break;
   }
}

