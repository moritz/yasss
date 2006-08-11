#!/bin/sh
REF=canonical_test_ref
cd ok_tests
for i in * ;
do
	if [ -e  ../"$REF"/"$i" ] ; then
		../../yasss -C < "$i" | diff - ../"$REF"/"$i";
		if [ $? != "0" ]; then 
			echo -n 'output of `../../yasss` < '"$i"' and its refernce'
			echo " file differ!"
			exit 1
		fi
	fi
done
