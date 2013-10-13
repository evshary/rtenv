#!/bin/zsh

GDBFILE="gdb_auto.in"
CFILE="shell.h"
GDBREAD=
GDBSKIP=

if [ -e $GDBFILE ];then
	echo "Remove the $GDBFILE"
	rm $GDBFILE
fi
echo "file main.elf" >> $GDBFILE
echo "target remote :3333" >> $GDBFILE
#Setting breaks
cat -n $CFILE | while read LINE
do
	if echo $LINE | grep -Eq 'GDBLABEL:READ'
	then
		GDBREAD=`expr $(echo $LINE | awk '{print $1}') + 1`
		echo "b shell.h:$GDBREAD" >> $GDBFILE
	elif echo $LINE | grep -Eq 'GDBLABEL:SKIP_DONE'
	then
		GDBSKIP=`expr $(echo $LINE | awk '{print $1}') + 1`
		echo "b shell.h:$GDBSKIP" >> $GDBFILE
	elif echo $LINE | grep -Eq 'GDBLABEL:STOP_TEST'
	then
		linenum=`expr $(echo $LINE | awk '{print $1}') + 1`
		echo "b shell.h:$linenum" >> $GDBFILE
	elif echo $LINE | grep -Eq 'GDBLABEL:TEST'
	then
		linenum=`expr $(echo $LINE | awk '{print $1}') + 1`
		echo "b shell.h:$linenum" >> $GDBFILE
	fi
done

#Move to read
echo "c" >> $GDBFILE

#Testing now
cat -n $CFILE | while read LINE
do
	if echo $LINE | grep -Eq 'GDBLABEL:TEST'
	then
		echo "j $GDBSKIP" >> $GDBFILE
		echo "c" >> $GDBFILE
		linenum=`expr $(echo $LINE | awk '{print $1}') + 1`
		echo "j $linenum" >> $GDBFILE
		echo "c" >> $GDBFILE
	fi
done
