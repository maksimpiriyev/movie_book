# base image
FROM ubuntu:24.04 as build-env
WORKDIR /app
RUN apt update -y; \
    apt upgrade -y; \
    apt install -y build-essential python3-pip cmake; \
    apt clean -y;

RUN pip install conan --break-system-packages;

# Copy everything
COPY . ./
# Restore as packages
RUN conan profile detect --force; \
    conan install . -s compiler.cppstd=gnu20 --build=missing;

# Build and publish a release  
RUN cd build/; \
#    cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release; \
    cmake .. -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./Release/generators/conan_toolchain.cmake -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=Release; \
    cmake --build . --config Release;



FROM ubuntu:24.04
WORKDIR /app
RUN apt update -y; \
    apt install -y libstdc++6

COPY --from=build-env /app/build/movie_book ./movie_book
COPY --from=build-env /app/build/movies.json ./movies.json
COPY --from=build-env /app/build/theatres.json ./theatres.json
EXPOSE 9001
ENTRYPOINT ["./movie_book"]
