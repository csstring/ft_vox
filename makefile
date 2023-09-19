CXX = c++ 
# 컴파일 플래그
#CXXFLAGS = -Wall -Wextra -Werror -std=c++17
CXXFLAGS = -std=c++17
# 링크 플래그
LDFLAGS = 
# 소스코드 파일 경로
SRC_DIR = ./src
# 오브젝트 파일 경로
OBJ_DIR = ./obj
# 헤더 파일 경로
INC_DIR = ./include
# 라이브러리 파일 경로
LIB_DIR = ./lib-universal

# GLFW 라이브러리 파일 경로
GLFW_LIB_DIR = $(LIB_DIR)

# 바이너리 파일
TARGET = ./ft_vox
# 소스 파일 리스트. Searching in src, src/math
SOURCES = $(wildcard $(SRC_DIR)/*.cpp )
# 오브젝트 파일 리스트
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ -L$(GLFW_LIB_DIR) -lglfw3 -lGLEW -framework Cocoa -framework OpenGL -framework IOKit

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/Math/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o

fclean: clean
	rm -f $(TARGET)

re : 
	${MAKE} fclean 
	${MAKE} all

.PHONY: all clean fclean