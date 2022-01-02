EXEC = polynomial

run1:
	mpirun -np 2 $(EXEC) a1.txt
	# correct is 7

run2:
	mpirun -np 14 --oversubscribe $(EXEC) a2.txt
	# correct is 6720838283.435946

run3:
	mpirun -np 7 --oversubscribe $(EXEC) a3.txt
	# correct is 197965

%:
	make EXEC="$(EXEC)" -f ../Makefile $@
