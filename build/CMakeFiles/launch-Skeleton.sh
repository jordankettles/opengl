#!/bin/sh
bindir=$(pwd)
cd /home/jordankettles/Documents/Github/opengl/opengl/Skeleton/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/jordankettles/Documents/Github/opengl/opengl/build/Skeleton 
	else
		"/home/jordankettles/Documents/Github/opengl/opengl/build/Skeleton"  
	fi
else
	"/home/jordankettles/Documents/Github/opengl/opengl/build/Skeleton"  
fi
