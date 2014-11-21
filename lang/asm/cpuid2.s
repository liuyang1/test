# cpuid2.s View the CPUID Vendor ID stirng using C library calls
.section .data
output:
	.asciz	"The Processor Vendor ID is '%s'\n"

# declare var
.section .bss
	.lcomm	buffer,12

.section .text
.global _start
_start:
	movl	$0,%eax
	cpuid
	movl	$buffer,%edi
	movl	%ebx,(%edi)
	movl	%edx,4(%edi)
	movl	%ecx,8(%edi)
# push param by reverse order to call PRINTF
	pushl	$buffer
	pushl	$output
	call	printf
# clear stack param,move back 8 byte
	addl	$8,%esp
# push 0,and call EXIT
	pushl	$0
	call	exit
