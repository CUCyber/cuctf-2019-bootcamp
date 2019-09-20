# RE3

Let's start off by trying our usual tricks. When we run the binary nothing seems to happen and when we run strings on the binary we don't get anything interesting.

```
$ ./re3
$ strings ./re3
/lib64/ld-linux-x86-64.so.2
libc.so.6
fflush
putchar
printf
stdout
...
```

This will probably involve more analysis than we're used to. We can start by going into GDB and disassembling main.

```
root@dev ~/c/r/RE3# gdb -q ./re3
pwndbg: loaded 171 commands. Type pwndbg [filter] for a list.
pwndbg: created $rebase, $ida gdb functions (can be used with print/break)
Reading symbols from ./re3...
```

When we disassemble main, we see that there is a call to printFlag. This is very promising! Our only problem is that there's a jump if not equal (`jne`) instruction beforehand.

```
pwndbg> disassemble main
Dump of assembler code for function main:
   0x0804853a <+0>:	lea    ecx,[esp+0x4]
   0x0804853e <+4>:	and    esp,0xfffffff0
   0x08048541 <+7>:	push   DWORD PTR [ecx-0x4]
   0x08048544 <+10>:	push   ebp
   0x08048545 <+11>:	mov    ebp,esp
   0x08048547 <+13>:	push   ecx
   0x08048548 <+14>:	sub    esp,0x14
   0x0804854b <+17>:	mov    DWORD PTR [ebp-0xc],0x0
   0x08048552 <+24>:	cmp    DWORD PTR [ebp-0xc],0x1
   0x08048556 <+28>:	jne    0x804855d <main+35>
   0x08048558 <+30>:	call   0x804846b <printFlag>
   0x0804855d <+35>:	mov    eax,0x0
   0x08048562 <+40>:	add    esp,0x14
   0x08048565 <+43>:	pop    ecx
   0x08048566 <+44>:	pop    ebp
   0x08048567 <+45>:	lea    esp,[ecx-0x4]
   0x0804856a <+48>:	ret    
End of assembler dump.
```

At main+17 the value 0 is moved into `ebp-0xc`, but then the line below (`cmp`) it checks to see if it's equal to 1. We can solve this challenge 2 different ways:

1. Editing the value in memory with GDB

2. Patching the binary

### Method 1 - Editing memory in GDB

Our strategy for editing the memory will be to

1. Break on the comparison
2. Edit the memory to set the value to 1
3. Continue the execution of the program


`main+24` is where the comparison is, so we'll set a breakpoint there:

```
pwndbg> break *main+24
Breakpoint 1 at 0x8048552
```

When we run the program, we hit the breakpoint:

```
pwndbg> r
Starting program: /home/user/re3 

Breakpoint 1, 0x08048552 in main ()
```

Now we need to edit the memory of the integer:

```
pwndbg> x $ebp-0xc
0xffffd5dc:	0x00000000
pwndbg> set {int}0xffffd5dc = 1
pwndbg> x $ebp-0xc
0xffffd5dc:	0x00000001
```

If we continue the program's execution, we'll receive the flag:

```
pwndbg> c
Continuing.
BOOTCAMP{patched_binary}
[Inferior 1 (process 2756981) exited normally]
```

### Method 2 - Binary Patching

Our methodology for this is simple, alter the jump condition to always jump regardless of the value check.

```
root@dev /tmp# radare2 -AAA -w re3
[0x08048370]> pdf@sym.main
/ (fcn) main 49
|   int main (int argc, char **argv, char **envp);
|           ; var uint32_t var_ch @ ebp-0xc
|           ; arg int32_t arg_4h @ esp+0x4
|           ; DATA XREF from entry0 @ 0x8048387
|           0x0804853a      8d4c2404       lea ecx, [arg_4h]
|           0x0804853e      83e4f0         and esp, 0xfffffff0
|           0x08048541      ff71fc         push dword [ecx - 4]
|           0x08048544      55             push ebp
|           0x08048545      89e5           mov ebp, esp
|           0x08048547      51             push ecx
|           0x08048548      83ec14         sub esp, 0x14
|           0x0804854b      c745f4000000.  mov dword [var_ch], 0
|           0x08048552      837df401       cmp dword [var_ch], 1
|       ,=< 0x08048556      7505           jne 0x804855d
|       |   0x08048558      e80effffff     call sym.printFlag
|       |   ; CODE XREF from main @ 0x8048556
|       `-> 0x0804855d      b800000000     mov eax, 0
|           0x08048562      83c414         add esp, 0x14
|           0x08048565      59             pop ecx
|           0x08048566      5d             pop ebp
|           0x08048567      8d61fc         lea esp, [ecx - 4]
\           0x0804856a      c3             ret
```

In radare2 we see that the jne call will jump to `mov eax, 0; ... ret` (`return 0;`) if the check fails. If we simply replace that with `je` it will jump if they values are equal instead of not equal - effectively it will never branch.

```
[0x08048370]> s 0x08048556
[0x08048556]> wa je 0x804855d
Written 2 byte(s) (je 0x804855d) = wx 7405
[0x08048556]> pdf@sym.main
/ (fcn) main 49
|   int main (int argc, char **argv, char **envp);
|           ; var uint32_t var_ch @ ebp-0xc
|           ; arg int32_t arg_4h @ esp+0x4
|           ; DATA XREF from entry0 @ 0x8048387
|           0x0804853a      8d4c2404       lea ecx, [arg_4h]
|           0x0804853e      83e4f0         and esp, 0xfffffff0
|           0x08048541      ff71fc         push dword [ecx - 4]
|           0x08048544      55             push ebp
|           0x08048545      89e5           mov ebp, esp
|           0x08048547      51             push ecx
|           0x08048548      83ec14         sub esp, 0x14
|           0x0804854b      c745f4000000.  mov dword [var_ch], 0
|           0x08048552      837df401       cmp dword [var_ch], 1
|      ,.-> 0x08048556      7405           je 0x804855d
|      |:   0x08048558      a4             movsb byte es:[edi], byte ptr [esi]
..
|      |    ; CODE XREF from main @ 0x8048556
|      `--> 0x0804855d      b800000000     mov eax, 0
|           0x08048562      83c414         add esp, 0x14
|           0x08048565      59             pop ecx
|           0x08048566      5d             pop ebp
|           0x08048567      8d61fc         lea esp, [ecx - 4]
\           0x0804856a      c3             ret
[0x000012a8]> q
```

If we rerun the binary after patching it, we should see the flag:

```
$ ./re3
BOOTCAMP{patched_binary}
```
