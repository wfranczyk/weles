[BITS 32]
begin:
mov eax, 0xb8000
mov [eax], BYTE 'B'
mov eax, 0xb8001
mov [eax], BYTE 0x1b
hang:
  jmp hang

times 4048 db 0

