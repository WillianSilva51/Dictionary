#===============================================================================
# DEFINIÇÕES DO COMPILADOR E OPÇÕES DE COMPILAÇÃO
#===============================================================================

# Compilador C++
CXX = g++

# Opções de compilação para debug e release
CXXFLAGS_DEBUG = -std=c++20 -Wall -Wextra -g -O0 -MMD -MP -Iinclude
CXXFLAGS_RELEASE = -std=c++20 -Wall -Wextra -O3 -DNDEBUG -MMD -MP -Iinclude

# Modo de compilação (debug ou release)
MODE ?= debug

# Altere para "release" para compilar em modo release
ifeq ($(MODE), release)
	CXXFLAGS = $(CXXFLAGS_RELEASE)
else
	CXXFLAGS = $(CXXFLAGS_DEBUG)
endif

#===============================================================================
# DETECÇÃO DO SISTEMA OPERACIONAL E VARIÁVEIS ESPECÍFICAS
#===============================================================================

ifeq ($(OS),Windows_NT)
# Ambiente Windows
	RM             = rmdir /q /s
	FIXPATH        = $(subst /,\,$1)
	MKDIR          = mkdir
	OBJ_MKDIR      = $(MKDIR) "$(dir $@)" 2>nul
	EXT            = .exe
	Cleanup        = cls
	CommandCreate  = if not exist $@ ($(MKDIR) $@)
else
# Ambiente Unix/Linux
	RM             = rm -f
	FIXPATH        = $1
	MKDIR          = mkdir -p
	OBJ_MKDIR       = $(MKDIR) "$(dir $@)" 2>/dev/null || true
	EXT            = 
	Cleanup        = clear
	CommandCreate  = if [ ! -d "$@" ]; then $(MKDIR) "$@"; fi
endif

#===============================================================================
# DIRETÓRIOS E ARQUIVOS-FONTE
#===============================================================================

# Diretórios para código-fonte, objetos e saída
SRC_DIRS = src
BUILD_DIR = build
OUTPUT_DIR = build/bin
TESTS_DIR = tests

# Definir o nome do executável
OUTPUT_NAME = $(notdir $(CURDIR))
OUTPUT = $(OUTPUT_DIR)/$(OUTPUT_NAME)$(EXT)

# Diretórios de inclusão e bibliotecas
INCLUDE_DIRS = include lib/googletest/googletest/include lib/googletest/googletest
INCLUDES = $(patsubst %,-I%, $(INCLUDE_DIRS))
LIB_DIRS = lib
LIBS = $(patsubst %,-L%, $(LIB_DIRS))

# Encontra todos os arquivos .cpp (incluindo subdiretórios) (/*.cpp para apenas o diretório atual)
SOURCES := $(wildcard $(SRC_DIRS)/**/*.cpp)
# Cria lista de objetos a partir dos fontes
OBJECTS := $(patsubst $(SRC_DIRS)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
# Arquivos de dependência gerados
DEPS    := $(OBJECTS:.o=.d)

#===============================================================================
# REGRAS PRINCIPAIS
#===============================================================================

.PHONY: all clean run test docs init

# Target principal
all: $(OUTPUT)

# Regra para gerar o executável (linka todos os objetos)
$(OUTPUT): $(OBJECTS) | $(OUTPUT_DIR)
	@echo "Linkando executavel $@ com os arquivos: $^"
	@$(CXX) -o $@ $^ $(LIBS)
	@echo "Compilacao concluida com sucesso!"
	@echo Executavel criado em: $(OUTPUT_DIR)/$(OUTPUT_NAME)$(EXT)

#===============================================================================
# REGRA PARA CRIAR OS DIRETÓRIOS (usando o comando específico de cada SO)
#===============================================================================
$(OUTPUT_DIR) $(BUILD_DIR):
	@$(CommandCreate)

#===============================================================================
# REGRA PARA COMPILAR OS ARQUIVOS .cpp
#===============================================================================
$(BUILD_DIR)/%.o: $(SRC_DIRS)/%.cpp | $(BUILD_DIR)
	@$(OBJ_MKDIR)
	@echo "Compilando $<..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(@F): $<" > $(@:.o=.d)

#===============================================================================
# INCLUSÃO AUTOMÁTICA DE DEPENDÊNCIAS GERADAS
#===============================================================================
-include $(DEPS)

#===============================================================================
# TARGETS AUXILIARES (clean, run, docs, init, etc.)
#===============================================================================

# Limpeza dos arquivos objetos, dependências e diretórios de saída
clean:
	@echo "Deletando arquivos objeto..."
ifeq ($(OS),Windows_NT)
	@if exist "$(BUILD_DIR)\*.o" (del "$(BUILD_DIR)\*.o")
	@echo "Deletando arquivos de dependencia..."
	@if exist "$(BUILD_DIR)\*.d" (del "$(BUILD_DIR)\*.d")
	@echo "Deletando a pasta $(OUTPUT_DIR)..."
	@if exist "$(OUTPUT_DIR)" ($(RM) "$(OUTPUT_DIR)")
	@echo "Deletando a pasta $(BUILD_DIR)..."
	@if exist "$(BUILD_DIR)" ($(RM) "$(BUILD_DIR)")
else
	@$(RM) $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d
	@echo "Deletando arquivos de dependencia e objetos..."
	@$(RM) -r $(BUILD_DIR) $(OUTPUT_DIR)
	@echo "Deletando a pasta $(BUILD_DIR) e $(OUTPUT_DIR)..."
endif
	@echo "Limpeza concluida com sucesso!"

# Regra para executar o programa
run: all
	@$(Cleanup)
	@echo "Executando o programa..."
	@$(OUTPUT)

# Regra para gerar documentação
docs:
	@$(Cleanup)
	@echo "Deletando documentacao antiga..."
ifeq ($(OS),Windows_NT)
	@if exist "docs" ($(RM) "docs") || if exist "docs" then ($(RM) "docs")
else
	@if [ -d "docs" ]; then $(RM) -r docs; fi
endif
	@echo "Documentacao antiga deletada com sucesso!"
	@echo "Gerando documentacao..."
	@doxygen Doxyfile
	@echo "Documentacao gerada com sucesso!"

## Regra para inicializar a estrutura de diretórios
init:
ifeq ($(OS),Windows_NT)
	@echo "Criando diretorios..."
	@if not exist "$(SRC_DIRS)" ($(MKDIR) "$(SRC_DIRS)/main")
	@if not exist "$(INCLUDE_DIRS)" ($(MKDIR) "$(INCLUDE_DIRS)")
	@if not exist "$(LIB_DIRS)" ($(MKDIR) "$(LIB_DIRS)")
	@if not exist "$(TESTS_DIR)" ($(MKDIR) "$(TESTS_DIR)")
	@if not exist "$(TESTS_DIR)\log" ($(MKDIR) "$(TESTS_DIR)\log")
	@echo. > "$(SRC_DIRS)\main\main.cpp"
	@echo "Diretorios criados com sucesso!"
else
	@echo "Criando diretorios..."
	@$(MKDIR) "$(SRC_DIRS)/main" "$(INCLUDE_DIRS)" "$(LIB_DIRS)" "$(TESTS_DIR)" "$(TESTS_DIR)/log" 2>/dev/null || echo "Alguns diretórios já existem."
	@touch "$(SRC_DIRS)/main/main.cpp"
	@echo "Diretorios criados com sucesso!"
endif
	@$(Cleanup)

#===============================================================================
# REGRAS PARA TESTES
#===============================================================================

# Variáveis para testes (tudo que estiver na pasta tests)
TEST_SOURCES := $(wildcard $(TESTS_DIR)/*.cpp)
TEST_OBJECTS := $(patsubst $(TESTS_DIR)/%.cpp,$(TESTS_DIR)/%.o,$(TEST_SOURCES))
# Nome do executável de teste; você pode ajustá-lo conforme desejar
TEST_EXECUTABLE := $(TESTS_DIR)/test$(EXT)

# Variáveis para o Google Test
# Certifique-se de que o Google Test esteja na pasta lib/googletest
GTEST_SRC = lib/googletest/googletest/src/gtest-all.cc lib/googletest/googletest/src/gtest_main.cc
GTEST_OBJECTS = $(patsubst %.cc,%.o,$(GTEST_SRC))

# Verifica se há arquivos de teste
ifneq ($(strip $(TEST_OBJECTS)),)
	TEST_AVAILABLE := 1
else
	TEST_AVAILABLE := 0
endif

lib/googletest/googletest/src/%.o: lib/googletest/googletest/src/%.cc
	@echo "Compilando Google Test $<..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Regra para compilar os arquivos de teste
$(TESTS_DIR)/%.o: $(TESTS_DIR)/%.cpp
	@echo "Compilando teste $<..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Regra para linkar o executável de teste
$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(GTEST_OBJECTS)
ifeq ($(TEST_AVAILABLE),1)
	@echo "Linkando executavel de teste $@ com GTest..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ -pthread
	@echo "Teste compilado com sucesso!"
endif

# Regra para compilar os testes
build-tests: $(TEST_EXECUTABLE)

test: $(TEST_EXECUTABLE)
ifeq ($(TEST_AVAILABLE),1)
	@echo "Executando os testes..."
	@$(TEST_EXECUTABLE)
	@echo "Testes concluídos com sucesso!"
else
	@echo "Nenhum teste encontrado. Crie arquivos .cpp em '$(TESTS_DIR)' para rodar testes com Google Test."
endif

# Regra para limpar os testes
clean-test:
ifeq ($(TEST_AVAILABLE),1)
	@echo "Limpando arquivos de teste..."
	@$(RM) $(TESTS_DIR)/*.o $(TESTS_DIR)/*.d $(TEST_EXECUTABLE)
	@echo "Arquivos de teste limpos com sucesso!"
else
	@echo "Nenhum teste para limpar."
endif