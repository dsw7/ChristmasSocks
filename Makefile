.PHONY = help lint dockertest
.DEFAULT_GOAL = help

LIGHT_PURPLE = "\033[1;1;35m"
NO_COLOR = "\033[0m"

define ECHO_STEP
	@echo -e $(LIGHT_PURPLE)\> $(1)$(NO_COLOR)
endef

define HELP_LIST_TARGETS

Lint C++ source with cppcheck
    $$ make lint
Run end to end tests in Docker
    $$ make dockertest

endef

export HELP_LIST_TARGETS

help:
	@echo "$$HELP_LIST_TARGETS"

lint:
	$(call ECHO_STEP,Linting project using cppcheck static analysis tool)
	@cppcheck ChristmasSocks/src/ --template=gcc --enable=all

dockertest:
	$(call ECHO_STEP,Running Docker tests)
	@docker build -t socks .
	@docker run -it --rm socks
