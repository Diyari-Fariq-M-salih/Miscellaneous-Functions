class Graph:
   def __init__(self, graph: dict = {}):
       self.graph = graph  # A dictionary for the adjacency list

   def add_edge(self, node1, node2, weight):
       if node1 not in self.graph:  # Check if the node is already added
           self.graph[node1] = {}  # If not, create the node
       self.graph[node1][node2] = weight  # Else, add a connection to its neighbor

