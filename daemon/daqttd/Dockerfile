FROM ubuntu:22.04 AS base

ARG BRANCH_NAME 

FROM base as dev-tools
RUN apt-get update &&                               \
    apt-get -y install  software-properties-common  \
                        make                        \
                        cmake                       \
                        ninja-build                 \
                        python3                     \
                        python3-pip                 \
                        python3-venv                \
                        unzip                       \
                        curl                        \
                        git                         \
                        libconfig++-dev

FROM dev-tools AS environment
RUN mkdir -p /project
WORKDIR /project

RUN python3 -m venv .venv
ENV PATH="/project/.venv/bin:$PATH"
ENV CONAN_HOME="/project/conan2"
RUN pip3 install conan && conan profile detect

FROM environment AS setup-project
RUN git clone -b $BRANCH_NAME https://github.com/i-ilak/daqtt.git

FROM setup-project AS daqttd
WORKDIR /project/daqtt/daemon/daqttd/
RUN conan install conan -pr:a=default --build=missing -of=$CONAN_HOME/project/daqtt
RUN cmake   -S .                                                                                            \
            -B build/cmake                                                                                  \
            -DCMAKE_BUILD_TYPE=Release                                                                      \
            -DCMAKE_TOOLCHAIN_FILE=$CONAN_HOME/project/daqtt/build/Release/generators/conan_toolchain.cmake
RUN cmake --build build/cmake && cmake --build build/cmake --target install
RUN cmake --build build/cmake --target install

FROM daqttd AS final
CMD ["daqttd", "--config", "/etc/daqttd/daqttd.conf"]
