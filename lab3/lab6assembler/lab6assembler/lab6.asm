    .686
    .model flat, stdcall
    option casemap: none
    include \masm32\include\windows.inc
include \masm32\include\kernel32.inc
include \masm32\include\masm32.inc
include \masm32\include\msvcrt.inc
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\masm32.lib
includelib \masm32\lib\msvcrt.lib
 
    .data
    ; заголовок таблицы
    msgTable1 BYTE 13,10,10,' ', 218, 18 DUP(196),     194,18 DUP(196),       194, 18 DUP(196), 194, 18 DUP(196), 191,0
msgTable db 13, 10," | Argument         ", "| 1/(x+1)^2        ", "| Series exp. sum  ", "| Members count    ", "|", 13, 10,0
msgTable2 BYTE 195, 18 DUP(196), 197, 18 DUP(196), 197, 18 DUP(196), 197, 18 DUP(196), 180,13,10, 0
msgTable3 BYTE 192, 18 DUP(196), 193, 18 DUP(196), 193, 18 DUP(196), 193, 18 DUP(196), 217, 0
 
    ;сообщения
    msgStart db "Lab #6", 13, 10
msgFun   db "Function: 1/(x+1)^2", 13, 10, 13, 10
wall    db  '|', 0
 
msgXn   db "Initial absciss : 0.1", 0
msgXk   db "Boundary absciss : 0.9", 0
msgDx   db "Step : 0.1", 0
msgEps  db "Tolerance : 0.001", 0
crlf    db  13, 10, 0
    ; форматы преобразования
    fmtF db "%lf", 0
    fmtD db "%d", 0
    buff db 20 dup (0)
    zero dq 0
 
    .data?
 
    hIn dd ?
    hOut dd ?
    lpWrite dd ?
    Xn dq ?
    Xk dq ?
    function dq ?
    Sr dq ?
    len dq ?
    delta dq ?
    epsilon dq ?
	temp dd ?
 
    .code
 
;Очистка буфера
    ClearBuff proc
    mov ecx, sizeof buff/4
    lea edi, buff
    xor eax, eax
    cld
    ; заполняем нулями в цикле
    @@:
    stosd
    loop @b
    ret
    ClearBuff endp
 
    ;________________________________________________
    Init proc
    ;вывод заголовка и считывание начальных значений
    ;и преобразование строк в float
    ;________________________________________________
    pusha
    ; хендлы консоли
    invoke GetStdHandle,STD_INPUT_HANDLE
    mov hIn,eax
    invoke GetStdHandle,STD_OUTPUT_HANDLE
    mov hOut,eax
 
    ku:
    ; вывести сообщения
    invoke WriteConsole, hOut, addr msgStart, sizeof msgStart, addr lpWrite, 0 ;Лабораторная работа
    invoke WriteConsole, hOut, addr msgFun, sizeof msgFun, addr lpWrite, 0;заданная Функция
    invoke WriteConsole, hOut, addr msgXn, sizeof msgXn, addr lpWrite, 0;начальный х

    invoke WriteConsole, hOut, addr msgXk, sizeof msgXk, addr lpWrite, 0;конечный х

    invoke WriteConsole, hOut, addr msgDx, sizeof msgDx, addr lpWrite, 0 ;шаг

    invoke WriteConsole, hOut, addr msgEps, sizeof msgEps, addr lpWrite, 0

    ; заголовок таблицы
        invoke WriteConsole, hOut, addr msgTable1, sizeof msgTable1, addr lpWrite, 0
        invoke WriteConsole, hOut, addr msgTable, sizeof msgTable, addr lpWrite, 0
        invoke WriteConsole, hOut, addr msgTable2, sizeof msgTable2, addr lpWrite, 0
 
    popa
    ret
    Init endp
 
   
 
    ;________________________________________________
    Print proc
    ;Вывод таблицы
    ;________________________________________________
    call ClearBuff ; чистим буфер
    invoke FloatToStr, Xn, offset buff; конвертируем результат в строку
    invoke WriteConsole, hOut, addr wall, sizeof wall, addr lpWrite, 0
    invoke WriteConsole, hOut, addr buff, sizeof buff-3, addr lpWrite, 0
    call ClearBuff
    invoke FloatToStr, function, offset buff
    invoke WriteConsole, hOut, addr wall, sizeof wall, addr lpWrite, 0
    invoke WriteConsole, hOut, addr buff, sizeof buff-3, addr lpWrite, 0
    call ClearBuff
    invoke FloatToStr, Sr, offset buff
    invoke WriteConsole, hOut, addr wall, sizeof wall, addr lpWrite, 0
    invoke WriteConsole, hOut, addr buff, sizeof buff-3, addr lpWrite, 0
    call ClearBuff
    invoke FloatToStr, len, offset buff
    invoke WriteConsole, hOut, addr wall, sizeof wall, addr lpWrite, 0
    invoke WriteConsole, hOut, addr buff, sizeof buff-3, addr lpWrite, 0
    invoke WriteConsole, hOut, addr wall, sizeof wall, addr lpWrite, 0
    invoke WriteConsole, hOut, addr crlf, sizeof crlf, addr lpWrite, 0
    ret
    Print endp
 
    ;________________________________________________
    Fx proc X: dword, T: dword
    ;Вычислние функции в точке
    ;Аргументы:
    ;X- аргумент функции
    ;Т- сама Функция
    ;Возвращает:
    ;ничего, записывает все в переменную function
    ;________________________________________________
    pusha
 
    finit
    mov eax, X
    fld1                  ;1
    fld qword ptr [eax]   ;1 x
    fld1                  ;1 x 1
    faddp st(1), st       ;1 (x+1)
    fld qword ptr [eax]   ;1 x+1  x
    fld1                  ;1 x+1  x 1
    faddp st(1), st       ;1 (x+1) (x+1)
    fmulp st(1), st       ;1 (x+1)^2
    fdivp st(1), st       ;1/(x+1)^2
    mov eax, T
    fstp qword ptr [eax]
 
    popa
    ret 6
    Fx endp
 
 
    ;________________________________________________
    Compare proc X: dword, Y: dword
    ; Сравнение вещественных чисел
    ; Аргументы:
    ; Х и У - сравниваемые числа
    ; Возвращаемое значение:
    ; eax = 0 если X < Y
    ; eax = 1 если X > Y
    ;________________________________________________
 
    pusha
 
    mov eax, Y
    fld qword ptr [eax]
    mov eax, X
    fld qword ptr [eax]
    ; проводим сравнение
    fcompp
    fstsw ax ; запираем флаги - Сохранить значение регистра SW в AX
    sahf
    jna @f ; переход если меньше
    popa
    mov eax, 1 ;если X > Y
    jmp @ext
    @@:
    mov eax, 0 ;если X < Y
    @ext:
 
    ret 8
    Compare endp
 
    ;________________________________________________
    Series proc X: dword, S: dword, F: dword, e: dword, Len: dword
    ;Вычислние значения ряда
    ;Аргументы:
    ;X-значение аргумента функции
    ;S-сумма ряда
    ;F-значение функции в точке X
    ;e-точность
    ;Len-кол-во слагаемых
    ;________________________________________________
    local pow: dword
    local i: dword
    local sign:dword
    local tmp:dword
    local tmp2:dword
    local tmp3:dword
    pusha
    finit
    ; вычисляем число членов ряда в 0
    mov i,1
    fild i ;загрузка целого числа на вершину стека
    mov eax, S
    fstp qword ptr [eax];S=1
    mov i,1
    mov eax, S;берем текущий член ряда (изначально 1)
    fld qword ptr [eax] ;загрузка вещественного числа в стек
    @begin:
    ;записываем (n+1) в tmp и (n) в pow
    mov eax, i
    mov pow, eax
    inc eax
    mov tmp, eax
    ;находим x^(n)
    @@:
    ; берем X
    mov eax, X
    fld qword ptr [eax]       ;s x
    ; возводим в степень
    cmp pow, 1 ;если в 1ой или ниже степени, пропускаем все это
    jle @f
    fld st
    mov ecx, pow
    sub ecx,2
    cmp ecx, 0
    je @@powpop
    @pow2:
    fmul st(1), st
    loop @pow2
    @@powpop:
    fmulp st(1), st
    @@:
    ;(-1)^n
    mov eax, i
 
    test eax,1
    jnz Odd
    jmp NoOdd
    Odd:
    fchs
    NoOdd:
    fild tmp            ;s (-1)^n*x^(n)  (n+1)
    fmulp st(1), st     ;s (-1)^n*x^(n)*(n+1)
    faddp st(1),st ; сложение с предыдущим членом ряда
    inc i ; увеличим число членов ряда на 1
    mov ebx, F
    fld qword ptr [ebx]    ;s f
    fsub st, st(1)         ;s  diff
    ; сравнение с epsilon
    fabs                   ;s  abs(diff)
    mov edx, e             
    fld qword ptr [edx]    ;s  abs(diff), e
    fcompp                 ;s
    fstsw ax
    sahf
     ;если меньше, то продолжить
    JNA @begin
    fild i
    mov eax, Len; забираем число членов ряда
    fstp qword ptr [eax]
    mov eax, S
    fstp qword ptr [eax]
    ; забираем результат
    popa
    ret 20
    Series endp

    start proc
 
    call Init 

	mov eax, 1
	mov temp, eax
	fild temp
	mov eax, 10
	mov temp, eax
	fild temp 
	fdivp
	fstp Xn

	mov eax, 9
	mov temp, eax
	fild temp
	mov eax, 10
	mov temp, eax
	fild temp 
	fdivp
	fstp Xk

	mov eax, 1
	mov temp, eax
	fild temp
	mov eax, 10
	mov temp, eax
	fild temp 
	fdivp
	fstp delta
	
	mov eax, 1
	mov temp, eax
	fild temp
	mov eax, 1000
	mov temp, eax
	fild temp 
	fdivp
	fstp epsilon


    @@:
 
    ; находим f(x)
    push offset function
    push offset Xn
    call Fx
    ; находим через ряд
 
    push offset len
    push offset epsilon
    push offset function
    push offset Sr
    push offset Xn
    call Series
    call Print ; выводим результаты
    fld Xn; увеличим X на delta
    fadd delta
    fstp Xn
    push offset Xk
    push offset Xn
    call Compare ; сравниваем начальный и конечный X
    cmp eax, 0
    je @b ; продолжать,если меньше
 
    invoke WriteConsole, hOut, addr msgTable3, sizeof msgTable3, addr lpWrite, 0
    invoke ReadConsole, hIn, addr buff, 1, addr lpWrite, 0
    invoke ExitProcess, 0
    start endp
	end start