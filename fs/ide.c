/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	u_int read_op = 0;

	while (offset_begin + offset < offset_end) {
		// Your code here
		// error occurred, then panic.
		u_int cur_offset = offset_begin + offset;
		if (syscall_write_dev((u_int)(&diskno), 0x13000010, 4))
			user_panic("error occurred during the read of the IDE disk.");
		if (syscall_write_dev((u_int)(&cur_offset), 0x13000000, 4))
			user_panic("error occurred during the read of the IDE disk.");
		if (syscall_write_dev((u_int)(&read_op), 0x13000020, 4))
			user_panic("error occurred during the read of the IDE disk.");
		u_int read_status = 0;
		if (syscall_read_dev((u_int)(&read_status), 0x13000030, 4))
			user_panic("error occurred during the read of the IDE disk.");
		if (read_status == 0)
			user_panic("error occurred during the read of the IDE disk.");
		if (syscall_read_dev((u_int)(dst + offset), 0x13004000, 512))
			user_panic("error occurred during the read of the IDE disk.");
		offset += 0x200;
	}
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
	// Your code here
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	u_int write_op = 1;

	// DO NOT DELETE WRITEF !!!
	writef("diskno: %d\n", diskno);

	while (offset_begin + offset < offset_end) {
		// copy data from source array to disk buffer.	
		// if error occur, then panic.
		u_int cur_offset = offset_begin + offset;
		if (syscall_write_dev((u_int)(&diskno), 0x13000010, 4))
			user_panic("error occured during the write of the IDE disk");
		if (syscall_write_dev((u_int)(&cur_offset), 0x13000000, 4))
			user_panic("error occured during the write of the IDE disk");
		if (syscall_write_dev((u_int)(src + offset), 0x13004000, 512))
			user_panic("error occured during the write of the IDE disk");
		if (syscall_write_dev((u_int)(&write_op), 0x13000020, 4))
			user_panic("error occured during the write of the IDE disk");
		u_int write_status = 0;
		if (syscall_read_dev((u_int)(&write_status), 0x13000030, 4))
			user_panic("error occured during the write of the IDE disk");
		if (write_status == 0)
			user_panic("error occured during the write of the IDE disk");
		offset += 0x200;
	}
}
