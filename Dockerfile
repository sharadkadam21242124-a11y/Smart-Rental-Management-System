# Stage 1: Build the C++ Application
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    libmysqlclient-dev \
    libssl-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN cmake -B build -S . -G "Ninja" -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --target SmartRentalManagement

# Stage 2: Lightweight Runtime Environment
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libmysqlclient21 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the compiled binary and static assets
COPY --from=builder /app/build/SmartRentalManagement /app/SmartRentalManagement
COPY --from=builder /app/frontend /app/frontend
COPY --from=builder /app/config /app/config

# Copy and prepare cloud entrypoint
COPY docker-entrypoint.sh /app/docker-entrypoint.sh
RUN chmod +x /app/docker-entrypoint.sh

ENV PORT=8080
EXPOSE 8080

ENTRYPOINT ["/app/docker-entrypoint.sh"]

