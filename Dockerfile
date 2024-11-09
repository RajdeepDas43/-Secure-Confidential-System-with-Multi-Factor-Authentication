# Use an official Ubuntu base image
FROM ubuntu:20.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Etc/UTC

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    wget \
    curl \
    git \
    vim \
    libssl-dev \
    libmysqlcppconn-dev \
    libboost-all-dev \
    mysql-client \
    pkg-config \
    doxygen \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install MySQL Connector/C++
RUN wget https://dev.mysql.com/get/Downloads/Connector-C++/mysql-connector-c++-8.0.27-linux-glibc2.12-x86_64.tar.gz && \
    tar -xvf mysql-connector-c++-8.0.27-linux-glibc2.12-x86_64.tar.gz && \
    cd mysql-connector-c++-8.0.27-linux-glibc2.12-x86_64 && \
    cp -r include/* /usr/local/include/ && \
    cp -r lib64/* /usr/local/lib/

# Set up the working directory
WORKDIR /usr/src/app

# Copy the current project files into the container
COPY . .

# Install dependencies using a package manager if required (like conan, vcpkg, etc.)
# RUN conan install . --build=missing
# RUN ./vcpkg install

# Build the project
RUN cmake . && make

# Default command to run when starting the container
CMD ["/bin/bash"]
