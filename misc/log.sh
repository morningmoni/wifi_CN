rm csi*.dat
for i in $(seq 10)
do
	echo "$i"
	./a.out csi$i.dat
	sleep 2
done
