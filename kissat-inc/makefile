all:
	$(MAKE) -C "/home/chenzh/solvers/cloud-nobug/cloud-sat/kissat-inc/build"
kissat:
	$(MAKE) -C "/home/chenzh/solvers/cloud-nobug/cloud-sat/kissat-inc/build" kissat
tissat:
	$(MAKE) -C "/home/chenzh/solvers/cloud-nobug/cloud-sat/kissat-inc/build" tissat
clean:
	rm -f "/home/chenzh/solvers/cloud-nobug/cloud-sat/kissat-inc"/makefile
	-$(MAKE) -C "/home/chenzh/solvers/cloud-nobug/cloud-sat/kissat-inc/build" clean
	rm -rf "/home/chenzh/solvers/cloud-nobug/cloud-sat/kissat-inc/build"
coverage:
	$(MAKE) -C "/home/chenzh/solvers/cloud-nobug/cloud-sat/kissat-inc/build" coverage
indent:
	$(MAKE) -C "/home/chenzh/solvers/cloud-nobug/cloud-sat/kissat-inc/build" indent
test:
	$(MAKE) -C "/home/chenzh/solvers/cloud-nobug/cloud-sat/kissat-inc/build" test
.PHONY: all clean coverage indent kissat test tissat
