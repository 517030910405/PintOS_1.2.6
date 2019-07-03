#include <filesys/file.h>
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include <string.h>
#include <list.h>
#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);
struct file_handle{
	int fd;
	struct file * opened_file;
	struct list_elem elem;
};
static struct list file_list;
void syscall_halt(struct intr_frame *f);
void syscall_exit(struct intr_frame *f,int status);
void syscall_exec(struct intr_frame *f,const char * cmd_line);
void syscall_wait(struct intr_frame *f,int pid);
bool syscall_create(struct intr_frame * itf, const char* name, int initial_size);
bool syscall_remove(struct intr_frame *itf, const char* name);
bool syscall_open(struct intr_frame *infr, const char * name);
int syscall_filesize(struct intr_frame *itf, int fd);
bool syscall_read(struct intr_frame *itf, int fd, const char *buffer, int size);
bool syscall_write(struct intr_frame *itf, int fd, const char *buffer, int size);
bool syscall_seek(struct intr_frame *itf, int fd, int offset);
int syscall_tell(struct intr_frame *itf, int fd);
bool syscall_close(struct intr_frame * itf, int fd);
void syscall_exit_with_return_value(struct intr_frame *itf, int return_value);
#endif /* userprog/syscall.h */
