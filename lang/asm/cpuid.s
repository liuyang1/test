# cpuid.s Sample program to extract the process Vendor ID
.section	.data
output:
	.ascii "The Processor Vendor ID is 'xxxxxxxxxxxx'\n"

.section	.bss

.section	.text

.global _start
_start:
	movl	$0,	%eax
	cpuid

# copy data to registers
	movl $output, %edi
	movl %ebx,    28(%edi)
	movl %edx,    32(%edi)
	movl %ecx,    36(%edi)
	movl $4,      %eax
	movl $1,      %ebx
	movl $output, %ecx
	movl $42,     %edx
	int  $0x80
# exit
	movl $1,      %eax
	movl $0,      %ebx
	int  $0x80
