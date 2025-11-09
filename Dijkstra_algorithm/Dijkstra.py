class Graph:
   def __init__(self, graph: dict = {}):
       self.graph = graph  # A dictionary for the adjacency list

   def add_edge(self, node1, node2, weight):
       if node1 not in self.graph:  # Check if the node is already added
           self.graph[node1] = {}  # If not, create the node
       self.graph[node1][node2] = weight  # Else, add a connection to its neighbor

G = Graph()

# Add A and its neighbors
G.add_edge("A", "B", 3)
G.add_edge("A", "C", 3)

# Add B and its neighbors
G.add_edge("B", "A", 3)
G.add_edge("B", "D", 3.5)
G.add_edge("B", "E", 2.8)
 
# Add C and its neighbors
G.add_edge("C", "A", 3)
G.add_edge("C", "E", 2.8)
G.add_edge("C", "F", 3.5)

# Add D and its neighbors
G.add_edge("D", "B", 3.5)
G.add_edge("D", "E", 3.1)
G.add_edge("D", "G", 10)

# Add E and its neighbors
G.add_edge("E", "B", 2.8)
G.add_edge("E", "D", 3.1)
G.add_edge("E", "C", 2.8)
G.add_edge("E", "G", 7)

# Add F and its neighbors
G.add_edge("E", "C", 3.5)
G.add_edge("E", "G", 2.5)

# Add G and its neighbors
G.add_edge("G", "D", 10)
G.add_edge("G", "E", 7)
G.add_edge("G", "F", 2.5)

print(G.graph)  # Print the adjacency list representation of the graph