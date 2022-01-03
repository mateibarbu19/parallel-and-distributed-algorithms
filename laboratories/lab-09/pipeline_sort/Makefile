EXEC = pipeline_sort

run1:
	mpirun -np 2 pipeline_sort > a1.txt

run2:
	mpirun -np 14 --oversubscribe pipeline_sort > a2.txt

clean:
	rm -f $(EXEC) *.txt

%:
	make EXEC="$(EXEC)" -f ../Makefile $@
