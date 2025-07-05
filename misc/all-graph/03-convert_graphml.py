#!/usr/bin/env python3

import os
from typing import override
from dataclasses import dataclass

import igraph as ig
from lxml import etree
from graphml import INode, IEdge, Config, GraphML


@dataclass(frozen=True)
class Node(INode):
    id: str
    step: int
    code: str

    @staticmethod
    @override
    def add_keys(graphml: etree.Element, cfg: Config) -> None:
        etree.SubElement(graphml, 'key', id='code', attrib={
            'id': 'code',
            'for': 'node',
            'attr.name': 'code',
            'attr.type': 'string'
        })
        etree.SubElement(graphml, 'key', id='step', attrib={
            'for': 'node',
            'attr.name': 'step',
            'attr.type': 'int'
        })
        if cfg.is_yed:
            INode._add_yed_key(graphml)

    @override
    def render(self, cfg: Config) -> etree.Element:
        node_xml = etree.Element('node', id=self.id)
        etree.SubElement(node_xml, 'data', key='code').text = self.code
        etree.SubElement(node_xml, 'data', key='step').text = str(self.step)

        if cfg.is_yed:
            color = cfg.colors[self.step]
            label = f'{self.code}&#10;({self.step})'
            node_xml.append(self._yed_render(cfg, color, label))

        return node_xml


@dataclass(frozen=True)
class Edge(IEdge):
    src: str
    dst: str

    @staticmethod
    @override
    def add_keys(graphml: etree.Element, cfg: Config) -> None:
        pass

    @override
    def render(self, cfg: Config) -> etree.Element:
        return etree.Element('edge', source=self.src, target=self.dst)


def load_graph(tag: str, graph: ig.Graph) -> tuple[GraphML, int]:
    nodes = []
    id_len = len(str(graph.vcount() - 1))
    for index in range(graph.vcount()):
        info = graph.vs[index]
        nodes.append(Node(f'n{index:0{id_len}d}', info['step'], info['code']))

    edges = []
    for n1, n2 in graph.get_edgelist():
        node_1 = nodes[n1]
        node_2 = nodes[n2]
        if node_1.step < node_2.step:
            node_1, node_2 = node_2, node_1
        edges.append(Edge(node_1.id, node_2.id))

    return GraphML(tag, nodes, edges), max(x.step for x in nodes)


def to_graphml(tag: str, input: str, output: str, is_yed: bool) -> None:
    print(f'Convert graph {input} into {output}')
    gml, max_step = load_graph(tag, ig.Graph.Read_Pickle(input))
    colors = GraphML.build_colors(max_step + 1, ['#0000ff', '#e8daef', '#ff0000'])
    cfg = Config(is_yed=is_yed, colors=colors)
    gml.save_graphml(output, cfg)


if __name__ == '__main__':
    os.makedirs('output-gml', exist_ok=True)
    os.makedirs('output-yed', exist_ok=True)
    for name in sorted(os.listdir('output-ig')):
        name = name.removesuffix('.pkl')
        tag = name.split('_')[0] if '_' in name else name
        to_graphml(tag, f'output-ig/{name}.pkl', f'output-gml/{name}.graphml', False)
        to_graphml(tag, f'output-ig/{name}.pkl', f'output-yed/{name}.graphml', True)
