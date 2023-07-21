import networkx as nx
import matplotlib.pyplot as plt
import json

def read_json(filename):
    """ Reads the json file and returns the deserialized object """
    return json.load(open(filename))

def parse_json(j):
    """ Reads the json data and returns the nodelist and edge_list """
    success = j["success_state"]
    fail = j["fail_state"]
    start = j["start_state"]

    nodes = set(range(1, j["num_states"])).union({success, fail, start})
    edges = {tuple(k) : v for [k, v] in j["transition"]}
    edges_start = {data: start for data in j["to_start"]}
    else_edges = {k : v for (k, v) in j["match_all_transitions"]} 
    
    return nodes, edges, else_edges

def draw_graph(nodes, edges, else_edges):
    """ Returns a nx.DiGraph() object with the nodes and edges (as well as some labels) """
    def parse_actions_into_name(action_set):
        return "|".join(map(str, action_set))
    # parse the edges into the following format:
    # dict[(start, end) : {actions}]
    # the else_edge action is called 'else'
    E = {(k[0], v) : set() for k, v in edges.items()}
    for k, v in edges.items():
        E[(k[0], v)].add(k[1])
    for k, v in else_edges.items():
        E[(k, v)] = E.get((k, v), set())
        E[(k, v)].add("else")
    # add the proper labels for the edges
    E_labels = {k:parse_actions_into_name(v) for k, v in E.items()}
    
    # create the graph
    G = nx.DiGraph()
    G.add_nodes_from(nodes)
    G.add_edges_from(E.keys())
    # draw the graph
    nx.draw_networkx(G, pos=nx.kamada_kawai_layout(G), arrows=True)
    nx.draw_networkx_edge_labels(G, pos=nx.kamada_kawai_layout(G), edge_labels=E_labels, font_color="black")
    return G

def show_graph():
    # show the graph
    plt.title("Graph")
    plt.show()

def main():
    data = read_json("fsm/ab.json")
    parsed = parse_json(data)
    graph = draw_graph(*parsed)
    show_graph()

if __name__ == "__main__":
    main()

