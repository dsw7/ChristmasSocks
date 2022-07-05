.PHONY = dockertest help
.DEFAULT_GOAL = help

LIGHT_PURPLE = "\033[1;1;35m"
NO_COLOR = "\033[0m"

define ECHO_STEP
	@echo -e $(LIGHT_PURPLE)\> $(1)$(NO_COLOR)
endef

define HELP_LIST_TARGETS

Run end to end tests in Docker
    $$ make requirements
endef

export HELP_LIST_TARGETS

help:
	@echo "$$HELP_LIST_TARGETS"

dockertest:
	$(call ECHO_STEP,Running Docker tests)
	@docker build -t socks .
	@docker run -it --rm socks
