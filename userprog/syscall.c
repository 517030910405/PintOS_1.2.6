#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include <string.h>
#include "devices/shutdown.h"
#include "threads/vaddr.h"
static void syscall_handler (struct intr_frame *);
static struct lock filesys_lock;
bool syscall_valid_vaddr(const void *vaddr, bool write);
bool syscall_check_user_buffer(const char* ustr, int size, bool write);
void
syscall_init (void) 
{
	//TODO:
	lock_init(&filesys_lock);
	list_init(&file_list);
	//:TODO
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}
//TODO:
inline void file_list_push_back(struct file_handle * handle){
	list_push_back(&file_list, &(handle -> elem));
}
void
syscall_exit_with_return_value(struct intr_frame *itf, int return_value) {
	printf ("%s: exit(%d)\n", thread_current()->name,return_value);
	struct thread *thread_running = thread_current();
	thread_running->return_value = return_value;
	itf->eax = return_value;
	thread_exit();
}
//:TODO
static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	// if (syscall_valid_vaddr((void *)(f -> esp),false)==false){
		// printf("NONONO!!!\n\n");
		// syscall_exit_with_return_value(f,-1);
	// }
	//CHANGE
	if (!syscall_check_user_buffer(f->esp, 4, false))
	syscall_exit_with_return_value(f, -1);
	
	int64_t esp = f -> esp;
	uint64_t * sp = (uint64_t*) esp;
	int sign = *sp;
	int *argv1 = f -> esp +4;
	int *argv2 = f -> esp +8;
	int *argv3 = f -> esp +12;
	//printf ("system call! %d\n",sign);
	switch (sign){
		case SYS_EXIT:
		case SYS_EXEC:
		case SYS_WAIT:
		case SYS_TELL:
		case SYS_CLOSE:
		case SYS_REMOVE:
		case SYS_OPEN:
		case SYS_FILESIZE:
		if (!syscall_check_user_buffer(argv1, 4, false))
		syscall_exit_with_return_value(f, -1);
		break;

		case SYS_CREATE:
		case SYS_SEEK:
		if (!syscall_check_user_buffer(argv1, 8, false))
		syscall_exit_with_return_value(f, -1);
		break;

		case SYS_READ:
		case SYS_WRITE:
		if (!syscall_check_user_buffer(argv1, 12, false))
		syscall_exit_with_return_value(f, -1);
		break;

		default: break;
	}
	switch (sign) {
		case SYS_HALT:
			syscall_halt(f);
			break;
		case SYS_EXIT:
			syscall_exit(f, *argv1);
			break;
		case SYS_EXEC:
			syscall_exec(f, *((char **)argv1));
			break;
		case SYS_WAIT:
			syscall_wait(f, *argv1);
			break;
		case SYS_CREATE:
			syscall_create(f, *((char**)argv1), *argv2);
			break;
		case SYS_REMOVE:
			syscall_remove(f, *((char**)argv1));
			break;
		case SYS_OPEN:
			syscall_open(f, *((char**)argv1));
			break;
		case SYS_FILESIZE:
			syscall_filesize(f, *argv1);
			break;
		case SYS_READ:
			syscall_read(f, *argv1, *((char**)argv2), *argv3);
			break;
		case SYS_WRITE:
			//printf("%d",*(int*)argv1);
			syscall_write(f, *argv1, *((char**)argv2), *argv3);
			break;
		case SYS_SEEK:
			syscall_seek(f, *argv1, *argv2);
			break;
		case SYS_TELL:
			syscall_tell(f, *argv1);
			break;
		case SYS_CLOSE:
			syscall_close(f, *argv1);
			break;
		default:
			ASSERT (0==1);
			break;
		//end of switch
	}
	//thread_exit ();
  
}


//TODO:

bool syscall_valid_vaddr(const void *vaddr, bool write){
	if (vaddr==NULL) return false;
	if (!is_user_vaddr(vaddr)) return false;
	//and check if it is user's , TODO check Virtual Memory 
	if (pagedir_get_page(thread_current()->pagedir, vaddr)==NULL) return false;
	return true;
}

bool syscall_check_user_string(const char* name){
	if (!syscall_valid_vaddr(name,false)) return false;
	int cnt = 0;
	char * str = name;
	int len = strlen(str);
	for (cnt=0;str[cnt]!='\0';++cnt){
		if (((int)(str+cnt)&((1<<4)-1))==0){
			if (!syscall_valid_vaddr(name,false)) return false;
		}
		if (cnt>=4096) return false;
	}
	return true;
}


bool syscall_open(struct intr_frame *infr, const char * name){
	if (!syscall_check_user_string(name)){
		syscall_exit_with_return_value(infr,-1);
		return false;
	}
	lock_acquire(&filesys_lock);
	struct file * tmp_file = filesys_open(name);
	lock_release(&filesys_lock);
	if (tmp_file == NULL){
		infr -> eax = -1;
		return false;
	}
	
	static int fd_size = 3;//0 is read //1 is write //2 is error output
	struct file_handle * handler;
	handler = malloc(sizeof(struct file_handle));
	handler -> opened_file = tmp_file;
	handler -> fd = fd_size ++;
	
	lock_acquire(&filesys_lock);
	file_list_push_back(handler);
	lock_release(&filesys_lock);
	
	infr -> eax = handler -> fd;
	return true;
	//infr -> eax = fh != NULL && is_dirfile();
	//Complete
}
struct file_handle * get_file_handle(int fd){
	struct list_elem *tmp;
	struct file_handle * handle = NULL;
	for (tmp = list_begin(&file_list);tmp!=list_end(&file_list);tmp = list_next(tmp)){
		struct file_handle * tmp_handle = list_entry(tmp, struct file_handle, elem);
		if (tmp_handle -> fd == fd){
			handle = tmp_handle;
			break;
		}
	}
	return handle;
}
bool syscall_close(struct intr_frame * itf, int fd){
	
	struct file_handle * handle = get_file_handle(fd);
	if (handle == NULL) {
		syscall_exit_with_return_value(itf, -1);
		return false;
	}
	
	lock_acquire(&filesys_lock);
	file_close(handle -> opened_file);
	list_remove(&(handle -> elem));
	lock_release(&filesys_lock);
	
	free(handle);
	return true;
}
int syscall_filesize(struct intr_frame *itf, int fd){
	struct file_handle *handle = get_file_handle(fd);
	if (handle==NULL){
		syscall_exit_with_return_value(itf, -1);
		return -1;
	}
	lock_acquire(&filesys_lock);
	itf -> eax = file_length(handle -> opened_file);
	lock_release(&filesys_lock);
	return itf -> eax;
}
bool syscall_seek(struct intr_frame *itf, int fd, int offset){
	struct file_handle * handle = get_file_handle(fd);
	if (handle==NULL){
		syscall_exit_with_return_value(itf, -1);
		return false;		
	}
	lock_acquire(&filesys_lock);
	file_seek(handle -> opened_file, offset);
	lock_release(&filesys_lock);
	return true;
}
int syscall_tell(struct intr_frame *itf, int fd){
	struct file_handle *handle = get_file_handle(fd);
	if (handle==NULL){
		syscall_exit_with_return_value(itf,-1);
		return -1;
	}
	lock_acquire(&filesys_lock);
	itf -> eax = file_tell(handle -> opened_file);
	lock_release(&filesys_lock);
	return itf -> eax;
}
bool syscall_read(struct intr_frame *itf, int fd, const char *buffer, int size){
	//CHANGE
  if (!syscall_check_user_buffer(buffer, size, true))
    syscall_exit_with_return_value(itf, -1);
	
	struct file_handle *handle = get_file_handle(fd);
	if (fd == STDOUT_FILENO) {
		syscall_exit_with_return_value(itf,-1);
		return false;
	}
	char * str = buffer;
	if (fd == STDIN_FILENO) {
		while ((--size)>=0){
			*(str++) = input_getc();
		}
		//We have the size, so we do not put \0 at the end
		return true;
	}
	if (handle==NULL){
		syscall_exit_with_return_value(itf,-1);
		return false;
	}
	lock_acquire(&filesys_lock);
	itf -> eax = file_read(handle -> opened_file, (void *) buffer, size);
	lock_release(&filesys_lock);
	return true;
}
bool syscall_write(struct intr_frame *itf, int fd, const char *buffer, int size){
	//printf("writing!%d\n",fd);
	//CHANGE
  if (!syscall_check_user_buffer(buffer, size, false))
    syscall_exit_with_return_value(itf, -1);
		
	struct file_handle *handle = get_file_handle(fd);
		//putbuf(buffer, size);
		
	if (fd == STDOUT_FILENO){
		//printf("writed!\n");
		putbuf(buffer, size);
		return true;
	}
	if (fd == STDIN_FILENO){
		syscall_exit_with_return_value(itf,-1);
		return false;
	}
	if (handle==NULL){
		//printf("Handle NULL");
		syscall_exit_with_return_value(itf,-1);
		return false;
	}
	lock_acquire(&filesys_lock);
	itf ->eax = file_write(handle -> opened_file, (void *)buffer, size);
	lock_release(&filesys_lock);
	return true;
}
bool syscall_create(struct intr_frame * itf, const char* name, int initial_size){
	if (!syscall_check_user_string(name)){
		syscall_exit_with_return_value(itf,-1);
		return false;
	}
	if (initial_size<0) initial_size = 0;
	lock_acquire(&filesys_lock);
	itf ->eax = filesys_create((char*)name, initial_size);
	lock_release(&filesys_lock);
	return true;
}
bool syscall_remove(struct intr_frame *itf, const char* name){
	if (!syscall_check_user_string(name)){
		syscall_exit_with_return_value(itf,-1);
		return false;
	}
	lock_acquire(&filesys_lock);
	itf -> eax = filesys_remove((char*)name);
	lock_release(&filesys_lock);
}
//:TODO
//LBN
#include "devices/shutdown.h"
void syscall_halt(struct intr_frame *f){
	shutdown_power_off();
}
//终止当前用户程序，将状态返回给内核。
//如果进程的父进程wait为它（见下文），则这是将返回的状态。
//通常，状态 0表示成功，非零值表示错误。
void syscall_exit(struct intr_frame *f,int status){
	syscall_exit_with_return_value(f,status);
}
//运行其名称在cmd_line中给出的可执行文件，传递任何给定的参数，并返回新进程的程序ID（pid）
//如果程序因任何原因无法加载或运行，则必须返回pid -1，否则该pid应该不是有效的pid
//因此，父进程无法返回，exec直到它知道子进程是否成功加载其可执行文件。
//您必须使用适当的同步来确保这一点。
//pid_t exec(const char * cmd_line)
void syscall_exec(struct intr_frame *f,const char * cmd_line){
	if(!syscall_check_user_string(cmd_line))
		syscall_exit_with_return_value(f,-1);
	lock_acquire(&filesys_lock);
	f->eax=(uint32_t)process_execute(cmd_line);
	lock_release(&filesys_lock);
	for (int i=1;i<=100000000;++i);
	//TODO
}
//int wait(pid_t pid)
void syscall_wait(struct intr_frame *f,int pid){
	f->eax=(uint32_t)process_wait(pid);
	for (int i=1;i<=100000000;++i);
	//process_wait TODO
}
//bool create(const char * file,unsigned initial_size)
//创建一个名为file的新文件，最初初始化为initial_size字节。
//如果成功则返回true，否则返回false。
//创建新文件不会打开它：打开新文件是一个单独的操作，需要open系统调用。
// void syscall_create(struct intr_frame*f,const char * file,unsigned initial_size){
	// if(!syscall_check_use_string(file))
		// syscall_exit_with_return_value(f,-1);
	// lock_acquire(&filesys_lock);
	// f->eax=(uint32_t)filesys_create(file);
	// lock_release(&filesys_lock);
// }

//:LBN
bool
syscall_check_user_buffer(const char* ustr, int size, bool write){
	if (!syscall_valid_vaddr(ustr + size - 1, write))
	return false;

	size >>= 12;
	do{
		if (!syscall_valid_vaddr(ustr, write))
		return false;
		ustr += 1 << 12;
	}while(size--);
	return true;
}

