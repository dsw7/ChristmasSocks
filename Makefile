.PHONY = help lint release test-release full-release test-memory dockertest
.DEFAULT_GOAL = help

LIGHT_PURPLE = "\033[1;1;35m"
NO_COLOR = "\033[0m"

define ECHO_STEP
	@echo -e $(LIGHT_PURPLE)\> $(1)$(NO_COLOR)
endef

define HELP_LIST_TARGETS

Lint C++ source with cppcheck
    $$ make lint
Compile release binary
    $$ make release
Run tests on release binary
    $$ make test-release
Compile release binary then run tests on release binary
    $$ make full-release
Run memory tests on Valgrind binary
    $$ make test-memory
Run end to end tests in Docker
    $$ make dockertest

endef

export HELP_LIST_TARGETS

help:
	@echo "$$HELP_LIST_TARGETS"

lint:
	$(call ECHO_STEP,Linting project using cppcheck static analysis tool)
	@cppcheck ChristmasSocks/src/ --template=gcc --enable=all

release:
	$(call ECHO_STEP,Generating Makefiles for release binary)
	@cmake -DCMAKE_BUILD_TYPE=Release -S ChristmasSocks/ -B ChristmasSocks/bin/
	$(call ECHO_STEP,Compiling project using Makefiles)
	@make --jobs=12 -C ChristmasSocks/bin/

test-release:
	$(call ECHO_STEP,Running tests with marker: release_test)
	@python3 -m pytest ChristmasSocks/tests/ -m release_test --verbose

full-release: release test-release

test-memory:
	$(call ECHO_STEP,Running tests with marker: memory_test)
	@python3 -m pytest ChristmasSocks/tests/ -m memory_test --verbose --capture=no

dockertest:
	$(call ECHO_STEP,Running Docker tests)
	@docker build -t socks .
	@docker run -it --rm socks
