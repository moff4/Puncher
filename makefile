G=gcc
g=g++
s=src/
i=include/
o=obj/
f=-I $s -I $i
b=build/
	
all: $bpucher

$bpucher: $omain.o $opuncher.o $olord.o $obytes.o $otypes.o $ostack.o
	$g    $omain.o $opuncher.o $olord.o $obytes.o $otypes.o $ostack.o -o $bpucher

$omain.o: $smain.cpp
	$g -c $smain.cpp -o $omain.o $f

$opuncher.o: $spuncher.cpp $ipuncher.h
	$g -c $spuncher.cpp -o $opuncher.o $f

$olord.o: $slord.cpp $ilord.h
	$g -c $slord.cpp -o $olord.o $f

$obytes.o: $sbytes.cpp $ibytes.h
	$g -c $sbytes.cpp -o $obytes.o $f

$otypes.o: $stypes.cpp $itypes.h
	$g -c  $stypes.cpp -o $otypes.o $f

$ostack.o: $sstack.cpp $istack.h
	$g -c  $sstack.cpp -o $ostack.o $f

clean:
	rm -rf $o*.o $bpucher
