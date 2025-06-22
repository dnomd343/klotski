#!/usr/bin/env python3

import igraph as ig
from lxml import etree
from dataclasses import dataclass


class GraphML:
    @dataclass(frozen=True)
    class Node:
        id: str
        code: str
        step: int

    @dataclass(frozen=True)
    class Edge:
        src: str  # node id
        dst: str  # node id

    def __init__(self, gid: str, graph: ig.Graph):
        self.__gid = gid

        self.__nodes = []
        id_len = len(str(graph.vcount() - 1))
        for index in range(graph.vcount()):
            info = graph.vs[index]
            self.__nodes.append(GraphML.Node(f'n{index:0{id_len}d}', info['code'], info['step']))

        self.__edges = []
        for n1, n2 in graph.get_edgelist():
            node_1 = self.__nodes[n1]
            node_2 = self.__nodes[n2]
            if node_1.step < node_2.step:
                node_1, node_2 = node_2, node_1
            self.__edges.append(GraphML.Edge(node_1.id, node_2.id))

    def __dump_node(self, node: Node) -> etree.Element:
        node_xml = etree.Element('node', id=node.id)
        etree.SubElement(node_xml, 'data', key='v_code').text = node.code
        etree.SubElement(node_xml, 'data', key='v_step').text = str(node.step)
        return node_xml

    def __dump_edge(self, edge: Edge) -> etree.Element:
        return etree.Element('edge', source=edge.src, target=edge.dst)

    def __dump_graph(self) -> etree.Element:
        graph_xml = etree.Element('graph', id=self.__gid, edgedefault='undirected')
        for node in self.__nodes:
            graph_xml.append(self.__dump_node(node))
        for edge in self.__edges:
            graph_xml.append(self.__dump_edge(edge))
        return graph_xml

    def build_and_save(self, output: str) -> None:
        root = etree.Element('graphml', nsmap={
            None: 'http://graphml.graphdrawing.org/xmlns',
            'xsi': 'http://www.w3.org/2001/XMLSchema-instance'
        })
        root.set(
            '{http://www.w3.org/2001/XMLSchema-instance}schemaLocation',
            'http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd'
        )

        etree.SubElement(root, 'key', attrib={
            'id': 'v_code',
            'for': 'node',
            'attr.name': 'code',
            'attr.type': 'string'
        })
        etree.SubElement(root, 'key', attrib={
            'id': 'v_step',
            'for': 'node',
            'attr.name': 'step',
            'attr.type': 'int'
        })
        root.append(self.__dump_graph())

        tree = etree.ElementTree(root)
        tree.write(output, pretty_print=True, xml_declaration=True, encoding='utf-8')


def to_graphml(tag: str, input: str, output: str) -> None:
    gml = GraphML(tag, ig.Graph.Read_Pickle(input))
    gml.build_and_save(output)


if __name__ == "__main__":
    to_graphml('0-00M-005X', 'output-ig/0-00M-005X_DAAFE0C.pkl', 'output-gml/0-00M-005X_DAAFE0C.graphml')
