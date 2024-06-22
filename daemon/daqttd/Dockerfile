FROM ubuntu:22.04 AS base

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
                        git

FROM dev-tools AS environment
RUN mkdir -p /project
WORKDIR /project

RUN python3 -m venv .venv
ENV PATH="/project/.venv/bin:$PATH"
ENV CONAN_HOME="/project/conan2"
RUN pip3 install conan && conan profile detect

FROM environment AS setup-project
RUN git clone git@github.com:i-ilak/daqtt.git

FROM setup-project AS libdaqtt
WORKDIR /project/daqtt/daemon/lib/daqtt
RUN conan install . -pr:a=default --build=missing -of=$CONAN_HOME/project/lib/daqtt
RUN cmake   -S .                                                                                            \
            -B build/cmake                                                                                  \
            -DCMAKE_BUILD_TYPE=Release                                                                      \
            -DCMAKE_TOOLCHAIN_FILE=$CONAN_HOME/project/lib/daqtt/build/Release/generators/conan_toolchain.cmake
RUN cmake --build build/cmake
RUN cmake --build build/cmake --target install

FROM libdaqtt AS daqtt
WORKDIR /project/daqtt/daemon/daqttd/
RUN cmake   -S .                                                                                            \
            -B build/cmake                                                                                  \
            -DCMAKE_BUILD_TYPE=Release                                                                      \
            -DCMAKE_TOOLCHAIN_FILE=$CONAN_HOME/project/build/Release/generators/conan_toolchain.cmake
RUN cmake --build build/cmake
RUN cmake --build build/cmake --target install

FROM daqttd AS final
CMD ["daqttd"]
