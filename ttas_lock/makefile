CFLAGS = -Wall -pedantic -static -I ./../Phikimon_bench/neganov/seminar_3//benchmark -shared -std=c++11
task_name = kichyr_ttas

compile: phikimon_adapter.cpp
	g++ $(CFLAGS) -c $^ -o lock.o

runLockTesting: compile
	cp -r -f ./lock.o ./../Phikimon_bench/neganov/seminar_3/implementations/$(task_name)
	cd ./../Phikimon_bench/neganov/seminar_3/; \
	./bench.sh $(task_name); \
	cp -f ./res/$(task_name).png ../../../bench_results

