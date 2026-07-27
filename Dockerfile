FROM ubuntu:24.04

RUN apt update && apt install -y \
    build-essential \
    cmake \
    git \
    libopencv-dev \
    && rm -rf /var/lib/apt/lists/*
    
WORKDIR /app

COPY . .

RUN mkdir build && cd build && cmake .. && cmake --build .

CMD ["bash"]