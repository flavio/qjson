DOCKER_CONTAINER = qt5
DOCKER_VOLUMES   = -v "$(CURDIR)/../:/qjson"

build_container ::
	docker build -t $(DOCKER_CONTAINER) .

test ::
	@echo "Building qjson and running its tests inside of $(DOCKER_CONTAINER) container"
	docker run -rm=true $(DOCKER_VOLUMES) $(DOCKER_CONTAINER) /qjson/docker/run_inside_container.sh

docker_shell ::
	docker run -rm=true -t -i $(DOCKER_VOLUMES) $(DOCKER_CONTAINER) /bin/bash
