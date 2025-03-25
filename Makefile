CXX = g++
CXXFLAGS = -I/usr/include/x86_64-linux-gnu/qt5 \
           -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets \
           -I/usr/include/x86_64-linux-gnu/qt5/QtCharts \
           -I/usr/include/x86_64-linux-gnu/qt5/QtCore \
           -I/usr/include/x86_64-linux-gnu/qt5/QtGui \
           -fPIC
LDFLAGS = -lQt5Widgets -lQt5Charts -lQt5Core -lQt5Gui

TARGET = myApp
SRC = main.cpp file_analyzer.cpp visualizer.cpp removeComment.cpp lexer.cpp halsteadmetrics.cpp cyclomatic.cpp

all:
	$(CXX) -o $(TARGET) $(SRC) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
