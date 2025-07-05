#!/usr/bin/env python3

import os
from typing import override
from dataclasses import dataclass

import igraph as ig
from lxml import etree
from graphml import INode, IEdge, Config, GraphML


@dataclass(frozen=True)
class Node(INode):
    tag: str
    step: int
    codes: list[str]

    @staticmethod
    @override
    def add_keys(graphml: etree.Element, cfg: Config) -> None:
        etree.SubElement(graphml, 'key', attrib={
            'id': 'step',
            'for': 'node',
            'attr.name': 'step',
            'attr.type': 'int'
        })
        etree.SubElement(graphml, 'key', attrib={
            'id': 'codes',
            'for': 'node',
            'attr.name': 'codes',
            'attr.type': 'string'
        })
        if cfg.is_yed:
            INode._add_yed_key(graphml)

    @override
    def render(self, cfg: Config) -> etree.Element:
        node_xml = etree.Element('node', id=self.tag)
        etree.SubElement(node_xml, 'data', key='step').text = str(self.step)
        etree.SubElement(node_xml, 'data', key='codes').text = '+'.join(self.codes)

        if cfg.is_yed:
            color = cfg.colors[self.step]
            label = f'{self.tag}&#10;({self.step})'
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
    def render(self, is_yed: bool) -> etree.Element:
        return etree.Element('edge', source=self.src, target=self.dst)


def to_graphml(tag: str, file: str, output: str, is_yed: bool) -> None:
    print(f'Convert graph {file} into {output}')

    g = ig.Graph.Read_Pickle(file)

    nodes = []
    for v in g.vs:
        node = Node(v['tag'], v['step'], v['codes'])
        nodes.append(node)

    edges = []
    for n1, n2 in g.get_edgelist():
        node_1 = nodes[n1]
        node_2 = nodes[n2]
        if node_1.step < node_2.step:
            node_1, node_2 = node_2, node_1
        edges.append(Edge(node_1.tag, node_2.tag))

    gml = GraphML(tag, nodes, edges)
    colors = GraphML.build_colors(max(x.step for x in nodes) + 1, ['#0000ff', '#e8daef', '#ff0000'])
    cfg = Config(is_yed=is_yed, colors=colors)
    gml.save_graphml(output, cfg)


if __name__ == '__main__':
    os.makedirs('output-combine-gml', exist_ok=True)
    for name in sorted(os.listdir('output-combine')):
        name = name.removesuffix('.pkl')
        tag = name.split('_')[0] if '_' in name else name
        to_graphml(tag, f'output-combine/{name}.pkl', f'output-combine-gml/{name}.graphml', False)
        to_graphml(tag, f'output-combine/{name}.pkl', f'output-combine-gml/{name}-yed.graphml', True)
