# Este Dockerfile utiliza uma construção multi-stage para criar uma imagem de contêiner
# otimizada para uma aplicação C++ de análise de frequência de palavras.

FROM docker.io/gcc:12-bookworm AS builder
# ### Estágio 1: `builder`
#
# Este estágio é responsável por compilar o código-fonte da aplicação.

WORKDIR /Dictionary

COPY makefile .

COPY src/ ./src/
COPY include/ ./include/

RUN make MODE=release

FROM docker.io/debian:bookworm-slim AS runner
# ### Estágio 2: Imagem Final
#
# Este estágio cria a imagem de produção, que é leve e contém apenas o necessário para executar a aplicação.

WORKDIR /app

COPY --from=builder ./Dictionary/build/bin/ ./build/bin/

COPY freq.sh .
COPY files ./files/

ENTRYPOINT [ "./freq.sh" ]

CMD [ "all", "domcasmurro.txt" ]

LABEL org.opencontainers.image.title="dictionary" \
    org.opencontainers.image.version="1.0.0"