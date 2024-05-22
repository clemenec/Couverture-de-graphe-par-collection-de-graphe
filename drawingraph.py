import tkinter as tk
import math

class DrawingApp:
    def __init__(self, master):
        self.master = master
        self.master.title("Graph Drawing App")
        self.master.geometry("800x600")
        
        self.canvas = tk.Canvas(self.master, bg="white", width=800, height=600)
        self.canvas.pack(fill=tk.BOTH, expand=True)
        self.setup_navbar()
        
        self.vertex_radius = 10
        self.vertices = []
        self.edges = []
        self.selected_vertices = []
        self.graphs = []
        self.next_id = 0
        
        self.canvas.bind("<Button-1>", self.add_vertex)
        self.canvas.bind("<Button-3>", self.select_vertex)

    def setup_navbar(self):
        menubar = tk.Menu(self.master)
        file_menu = tk.Menu(menubar, tearoff=0)
        file_menu.add_command(label="in F", command=self.save_family)
        file_menu.add_command(label="as G", command=self.save_graph)
        menubar.add_cascade(label="Save graph", menu=file_menu)
        menubar.add_command(label="Quit", command=self.write_family)
        self.master.config(menu=menubar)

    def is_near_existing_vertex(self, x, y):
        for vertex in self.vertices:
            v1_x, v1_y = self.canvas.coords(vertex[0])[:2]
            distance = math.sqrt((x - v1_x)**2 + (y - v1_y)**2)
            if distance < 30:
                return True
        return False
    
    def add_vertex(self, event):
        x, y = event.x, event.y
        if not self.is_near_existing_vertex(x, y):
            vertex = self.canvas.create_oval(x - self.vertex_radius, y - self.vertex_radius, 
                                            x + self.vertex_radius, y + self.vertex_radius, 
                                            fill="black")
            self.vertices.append([vertex, self.next_id])
            self.next_id += 1
        
    def select_vertex(self, event):
        if len(self.selected_vertices) < 2:
            x, y = event.x, event.y
            item_id = self.canvas.find_closest(x, y)
            found = False
            for vertex in self.vertices:
                if item_id[0] == vertex[0]:
                    item = vertex
                    found = True
            if found:
                if item not in self.selected_vertices:
                    self.selected_vertices.append(item)
                    self.canvas.itemconfig(item_id[0], fill="red")
                else:
                    self.canvas.itemconfig(item_id[0], fill="black")
                    self.selected_vertices.clear()
        if len(self.selected_vertices) == 2:
            self.draw_edge()
            for vertex in self.selected_vertices:
                self.canvas.itemconfig(vertex[0], fill="black")
            self.selected_vertices.clear()
        
    def draw_edge(self):
        v1_x, v1_y = self.canvas.coords(self.selected_vertices[0][0])[:2]
        v2_x, v2_y = self.canvas.coords(self.selected_vertices[1][0])[:2]
        self.edges.append([self.selected_vertices[0][1], self.selected_vertices[1][1]])
        self.canvas.create_line(v1_x + 10, v1_y + 10, v2_x + 10, v2_y + 10, width=3 ,fill="black")

    def save_family(self):
        self.graphs.append([self.vertices[:], self.edges[:]])
        self.canvas.delete("all")
        self.vertices.clear()
        self.edges.clear()
        self.selected_vertices.clear()
        self.next_id = 0

    def save_graph(self):
        with open("InputG.txt", "w") as file:
            file.write(f"sommets : {len(self.vertices)}\n")
            file.write("arêtes :\n")
            for o in range(len(self.edges)):
                if o < len(self.edges)-1:
                    file.write(f"[{self.edges[o][0]},{self.edges[o][1]}];")
                else:
                    file.write(f"[{self.edges[o][0]},{self.edges[o][1]}]")
            self.canvas.delete("all")
            self.vertices.clear()
            self.edges.clear()
            self.selected_vertices.clear()
            self.next_id = 0

    def write_family(self):
        with open("InputF.txt", "w") as file:
            file.write(f"graphes : {len(self.graphs)}")
            for graph in self.graphs:
                file.write(f"\n\n")
                file.write(f"sommets : {len(graph[0])}\n")
                file.write("arêtes :\n")
                for o in range(len(graph[1])):
                    if o < len(graph[1])-1:
                        file.write(f"[{graph[1][o][0]},{graph[1][o][1]}];")
                    else:
                        file.write(f"[{graph[1][o][0]},{graph[1][o][1]}]")
        self.master.destroy()

def main():
    root = tk.Tk()
    app = DrawingApp(root)
    root.mainloop()

if __name__ == "__main__":
    main()
