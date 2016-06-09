rm csi*
for i in $(seq 10)
do
	echo "**************************"
	echo "$i"
	./my_log_to_file csi$i.dat
	echo "start dat2csv"
	./my_read_bfee csi$i.dat csi$i.csv
	echo "end dat2csv"
	sleep 2
	echo "**************************"
done
