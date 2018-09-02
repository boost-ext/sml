CXX?=clang++
CXXFLAGS?=-std=c++2a -fcoroutines-ts -stdlib=libc++
CXXFLAGS_RELASE?=${CXXFLAGS} -O3 -march=native -DNDEBUG -fno-exceptions
CXXFLAGS_DEBUG?=${CXXFLAGS} -g -O0
OBJS:=naive_if_else.cpp \
			naive_switch.cpp \
			naive_state_pattern.cpp \
			stl_variant.cpp \
			stl_coroutine_loop.cpp \
			stl_coroutine_goto.cpp \
			stl_coroutine_fun_variant.cpp \
			boost_statechart.cpp \
			boost_msm.cpp \
			boost_sml.cpp \

all: clean performance_release compilation_time_debug compilation_time_release executable_size_debug executable_size_release

google_benchmark: $(patsubst %.cpp, %.gb.out, $(OBJS))
%.gb.out:
	@echo google_benchmark:$*
	@${CXX} ${CXXFLAGS_RELASE} $*.cpp -o $*.out -lbenchmark -lpthread -DTEST_GBENCH && ./$*.out

performance_release: $(patsubst %.cpp, %.pr.out, $(OBJS))
%.pr.out:
	@echo performance_release:$*
	@${CXX} ${CXXFLAGS_RELASE} $*.cpp -o $*.pr.out -DTEST_PERF
	@perf stat ./$*.pr.out 2>&1 | grep "seconds time elapsed" | sed "s/ *\(.*\)seconds time elapsed/\"$*\" \1/" >> performance_time[s]_release.dat
	@perf stat ./$*.pr.out 2>&1 | grep "cycles:" | sed "s/ *\(.*\).*cycles:.*/\"$*\" \1/" >> performance_cycles_release.dat
	@perf stat ./$*.pr.out 2>&1 | grep "instructions:" | sed "s/ *\(.*\).*instructions:.*/\"$*\" \1/" >> performance_instructions_release.dat
	@perf stat ./$*.pr.out 2>&1 | grep "insn per cycle" | sed "s/.*# *\(.*\).*insn per cycle.*/\"$*\" \1/" >> performance_instruction-per-cycle_release.dat
	@perf stat ./$*.pr.out 2>&1 | grep "branches:" | sed "s/ *\(.*\).*branches:.*/\"$*\" \1/" >> performance_branches_release.dat
	@perf stat ./$*.pr.out 2>&1 | grep "branch-misses:" | sed "s/ *\(.*\).*branch-misses:.*/\"$*\" \1/" >> performance_branch-misses_release.dat

compilation_time_debug: $(patsubst %.cpp, %.ctd.out, $(OBJS))
%.ctd.out:
	@echo compilation_time_debug:$*
	@perf stat ${CXX} ${CXXFLAGS_DEBUG} $*.cpp -o $*.ctd.out -DTEST_ASM 2>&1 | grep "seconds time elapsed" | sed "s/ *\(.*\)seconds time elapsed/\"$*\" \1/" >> compilation_time[s]_debug.dat

compilation_time_release: $(patsubst %.cpp, %.ctr.out, $(OBJS))
%.ctr.out:
	@echo compilation_time_release:$*
	@perf stat ${CXX} ${CXXFLAGS_RELEASE} $*.cpp -o $*.ctr.out -DTEST_ASM 2>&1 | grep "seconds time elapsed" | sed "s/ *\(.*\)seconds time elapsed/\"$*\" \1/" >> compilation_time[s]_release.dat

executable_size_release: $(patsubst %.cpp, %.esr.out, $(OBJS))
%.esr.out:
	@echo executable_size_release:$*
	@${CXX} ${CXXFLAGS_RELASE} $*.cpp -o $*.esr.out -DTEST_ASM && ls -s --block-size=k $*.esr.out | sed "s/\(.*\) \(.*\).esr.out/\"\2\"\ \1/" >> executable_size[kb]_release.dat

executable_size_debug: $(patsubst %.cpp, %.esd.out, $(OBJS))
%.esd.out:
	@echo executable_size_debug:$*
	@${CXX} ${CXXFLAGS_DEBUG} $*.cpp -o $*.esd.out -DTEST_ASM && ls -s --block-size=k $*.esd.out | sed "s/\(.*\) \(.*\).esd.out/\"\2\"\ \1/" >> executable_size[kb]_debug.dat

plot: $(patsubst %.dat, %.plot, $(wildcard *.dat))
%.plot:
	@sed -i "s/_/ /g" "$*.dat"
	@echo -e "\
		set terminal png transparent size 1024,650\n\
		set output '$*.png'\n\
		set boxwidth 0.8\n\
		set style fill solid noborder\n\
		set yrange [0:*]\n\
		set offsets 0, 0, 1, 0\n\
		set grid ytics\n\
		set xtics rotate by 45 right\n\
		unset key\n\
		set xtics textcolor rgb \"white\"\n\
		set ytics textcolor rgb \"white\"\n\
		set xlabel textcolor rgb \"white\"\n\
		set ylabel textcolor rgb \"white\"\n\
		set ylabel \"$(shell echo $* | cut -d'_' -f2)\"\n\
		plot '$*.dat' using 0:2:(\$$0+1):xtic(1) with boxes linecolor variable\n" | gnuplot

clean:
	@rm -f *.out *.dat *.png
