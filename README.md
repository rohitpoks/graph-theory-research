# graph-theory-research

Installing boost: 
brew install boost

Installing graphviz: 
brew install graphviz

Running the script: 
g++ -std=c++17 -I /opt/homebrew/Cellar/boost/1.86.0/include graphSketcher.cpp graphProcessor.cpp originalToColoring.cpp driver.cpp

Visualizing the graph: dot -Tsvg k4_example.dot > k4_example.svg 
