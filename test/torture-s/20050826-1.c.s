	.text
	.file	"/b/build/slave/linux/build/src/src/work/gcc/gcc/testsuite/gcc.c-torture/execute/20050826-1.c"
	.section	.text.bar,"ax",@progbits
	.hidden	bar
	.globl	bar
	.type	bar,@function
bar:                                    # @bar
	.param  	i32
	.local  	i32
# BB#0:                                 # %entry
	block   	.LBB0_6
	i32.const	$push0=, .L.str
	i32.const	$push1=, 8
	i32.call	$push2=, memcmp, $0, $pop0, $pop1
	br_if   	$pop2, .LBB0_6
# BB#1:                                 # %for.body.preheader
	i32.const	$push3=, 7
	i32.add 	$1=, $0, $pop3
	i32.const	$0=, 0
.LBB0_2:                                # %for.body
                                        # =>This Inner Loop Header: Depth=1
	block   	.LBB0_5
	loop    	.LBB0_4
	i32.add 	$push4=, $1, $0
	i32.load8_u	$push5=, 0($pop4)
	br_if   	$pop5, .LBB0_5
# BB#3:                                 # %for.cond
                                        #   in Loop: Header=BB0_2 Depth=1
	i32.const	$push6=, 1
	i32.add 	$0=, $0, $pop6
	i32.const	$push7=, 2040
	i32.le_u	$push8=, $0, $pop7
	br_if   	$pop8, .LBB0_2
.LBB0_4:                                # %for.end
	return
.LBB0_5:                                # %if.then2
	call    	abort
	unreachable
.LBB0_6:                                # %if.then
	call    	abort
	unreachable
.Lfunc_end0:
	.size	bar, .Lfunc_end0-bar

	.section	.text.foo,"ax",@progbits
	.hidden	foo
	.globl	foo
	.type	foo,@function
foo:                                    # @foo
	.result 	i32
	.local  	i32, i32, i32, i32, i32
# BB#0:                                 # %entry
	i32.const	$0=, a
	i32.const	$1=, 0
	i32.const	$push0=, 2048
	call    	memset, $0, $1, $pop0
	i32.load8_u	$push2=, .L.str.1+4($1)
	i32.store8	$discard=, a+5($1), $pop2
	i32.load8_u	$push3=, .L.str.1+3($1)
	i32.store8	$discard=, a+4($1), $pop3
	i32.load8_u	$push4=, .L.str.1+2($1)
	i32.store8	$discard=, a+3($1), $pop4
	i32.load8_u	$push5=, .L.str.1+1($1)
	i32.store8	$discard=, a+2($1), $pop5
	i32.load8_u	$push6=, .L.str.1($1)
	i32.store8	$discard=, a+1($1), $pop6
	i32.const	$push1=, 1
	i32.store8	$3=, a($1), $pop1
	i32.const	$4=, 8
	i32.store8	$2=, a+6($1), $3
	block   	.LBB1_5
	i32.const	$push7=, .L.str
	i32.call	$push8=, memcmp, $0, $pop7, $4
	br_if   	$pop8, .LBB1_5
.LBB1_1:                                # %for.cond.i
                                        # =>This Inner Loop Header: Depth=1
	block   	.LBB1_4
	loop    	.LBB1_3
	i32.const	$push9=, -7
	i32.add 	$push10=, $4, $pop9
	i32.const	$push11=, 2040
	i32.gt_u	$push12=, $pop10, $pop11
	br_if   	$pop12, .LBB1_4
# BB#2:                                 # %for.cond.i.for.body.i_crit_edge
                                        #   in Loop: Header=BB1_1 Depth=1
	i32.add 	$3=, $0, $4
	i32.add 	$4=, $4, $2
	i32.load8_u	$push13=, 0($3)
	i32.const	$push14=, 0
	i32.eq  	$push15=, $pop13, $pop14
	br_if   	$pop15, .LBB1_1
.LBB1_3:                                # %if.then2.i
	call    	abort
	unreachable
.LBB1_4:                                # %bar.exit
	return  	$1
.LBB1_5:                                # %if.then.i
	call    	abort
	unreachable
.Lfunc_end1:
	.size	foo, .Lfunc_end1-foo

	.section	.text.main,"ax",@progbits
	.hidden	main
	.globl	main
	.type	main,@function
main:                                   # @main
	.result 	i32
	.local  	i32, i32, i32, i32, i32
# BB#0:                                 # %entry
	i32.const	$0=, a
	i32.const	$1=, 0
	i32.const	$push0=, 2048
	call    	memset, $0, $1, $pop0
	i32.load8_u	$push2=, .L.str.1+4($1)
	i32.store8	$discard=, a+5($1), $pop2
	i32.load8_u	$push3=, .L.str.1+3($1)
	i32.store8	$discard=, a+4($1), $pop3
	i32.load8_u	$push4=, .L.str.1+2($1)
	i32.store8	$discard=, a+3($1), $pop4
	i32.load8_u	$push5=, .L.str.1+1($1)
	i32.store8	$discard=, a+2($1), $pop5
	i32.load8_u	$push6=, .L.str.1($1)
	i32.store8	$discard=, a+1($1), $pop6
	i32.const	$push1=, 1
	i32.store8	$3=, a($1), $pop1
	i32.const	$4=, 8
	i32.store8	$2=, a+6($1), $3
	block   	.LBB2_5
	i32.const	$push7=, .L.str
	i32.call	$push8=, memcmp, $0, $pop7, $4
	br_if   	$pop8, .LBB2_5
.LBB2_1:                                # %for.cond.i.i
                                        # =>This Inner Loop Header: Depth=1
	block   	.LBB2_4
	loop    	.LBB2_3
	i32.const	$push9=, -7
	i32.add 	$push10=, $4, $pop9
	i32.const	$push11=, 2040
	i32.gt_u	$push12=, $pop10, $pop11
	br_if   	$pop12, .LBB2_4
# BB#2:                                 # %for.cond.i.for.body.i_crit_edge.i
                                        #   in Loop: Header=BB2_1 Depth=1
	i32.add 	$3=, $0, $4
	i32.add 	$4=, $4, $2
	i32.load8_u	$push13=, 0($3)
	i32.const	$push14=, 0
	i32.eq  	$push15=, $pop13, $pop14
	br_if   	$pop15, .LBB2_1
.LBB2_3:                                # %if.then2.i.i
	call    	abort
	unreachable
.LBB2_4:                                # %foo.exit
	return  	$1
.LBB2_5:                                # %if.then.i.i
	call    	abort
	unreachable
.Lfunc_end2:
	.size	main, .Lfunc_end2-main

	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"\001HELLO\001"
	.size	.L.str, 8

	.hidden	a                       # @a
	.type	a,@object
	.section	.bss.a,"aw",@nobits
	.globl	a
a:
	.skip	2048
	.size	a, 2048

	.type	.L.str.1,@object        # @.str.1
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.1:
	.asciz	"HELLO"
	.size	.L.str.1, 6


	.ident	"clang version 3.8.0 "
	.section	".note.GNU-stack","",@progbits