.text
.align 4
.globl ARM_MoveImage
.globl ARM_ClearImage
.globl ARM_ClearImage_Fast
.globl convert_frame_buffer_bgr555_to_rgb555

@arguments:
@ u8* src               r0
@ u8* dst               r1
@ u32 src_advance       r2
@ u32 w0                r3
@ u32 h1                sp+12
ARM_MoveImage:
	stmfd sp!,{r4-r6}

    ldr r4, [sp, #12]

    VertLoopMoveImage:
            mov r5, r3
            HorzLoopMoveImage:
                    ldrh r6, [r0], #2
					strh r6, [r1], #2
					subs r5, r5, #1
            bne HorzLoopMoveImage
            add r0, r0, r2
			add r1, r1, r2
            subs r4, r4, #1
    bne VertLoopMoveImage

	ldmfd sp!,{r4-r6}
	bx lr

@arguments:
@ u8* dst               r0
@ u32 src_advance       r1
@ u32 rgb				r2
@ u32 w0                r3
@ u32 h1                sp+8
ARM_ClearImage:
	stmfd sp!,{r4-r5}

    ldr r4, [sp, #8]

    VertLoopClearImage:
            mov r5, r3
            HorzLoopClearImage:
					strh r2, [r0], #2
					subs r5, r5, #1
            bne HorzLoopClearImage
            add r0, r0, r1
            subs r4, r4, #1
    bne VertLoopClearImage

	ldmfd sp!,{r4-r5}
	bx lr


@arguments:
@ u8* dst               r0
@ u32 src_advance       r1
@ u32 rgb				r2 -> r4-r11
@ u32 w0                r3 -> r2
@ u32 h1                sp+36 -> r12
ARM_ClearImage_Fast:
    stmfd sp!,{r4-r8,r10-r12,lr}
    ldr r12, [sp, #36]

	mov r4, r2
	mov r5, r2
	mov r6, r2
	mov r7, r2
	mov r8, r2
	mov r10, r2
	mov r11, r2
	mov lr, r2

    VertLoopClearImageFast:
            mov r2, r3
			bic  r0,r0,#3 @ stupid alignment
            HorzLoopClearImageFast:
					stmia r0!,{r4-r8,r10-r11,lr} @ 16 pixels
					subs r2, r2, #16
            bne HorzLoopClearImageFast
            add r0, r0, r1
            subs r12, r12, #1
    bne VertLoopClearImageFast

	ldmfd sp!,{r4-r8,r10-r12,lr}
	bx lr
