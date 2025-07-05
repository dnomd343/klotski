#!/usr/bin/env python3

import numpy as np
import igraph as ig
import plotly.graph_objs as go
import matplotlib.colors as mcolors


def build_colors(max_step: int) -> list[str]:
    bwr = ['#0000ff', '#e8daef', '#ff0000']
    cmap = mcolors.LinearSegmentedColormap.from_list('custom_bwr', bwr)
    return [mcolors.to_hex(cmap(x)) for x in np.linspace(0, 1, max_step + 1)]


def build_3d_graph(graph: ig.Graph, colors: list[str], output: str) -> None:
    layout = graph.layout('kk', dim=3)

    Xn = [x[0] for x in layout]
    Yn = [x[1] for x in layout]
    Zn = [x[2] for x in layout]

    Xe, Ye, Ze = [], [], []
    for x, y in graph.get_edgelist():
        Xe += [layout[x][0], layout[y][0], None]
        Ye += [layout[x][1], layout[y][1], None]
        Ze += [layout[x][2], layout[y][2], None]

    edge_trace = go.Scatter3d(
        x=Xe, y=Ye, z=Ze,
        mode='lines',
        line=dict(color='gray', width=2),
        hoverinfo='none'
    )

    node_trace = go.Scatter3d(
        x=Xn, y=Yn, z=Zn,
        mode='markers',
        marker=dict(
            symbol='circle',
            size=6,
            color=[colors[x['step']] for x in graph.vs]
        ),
        hoverinfo='text',
        text=[f'{x['tag']} ({x['step']}) [{'/'.join(x['codes'])}]' for x in graph.vs]
    )

    fig = go.Figure(data=[edge_trace, node_trace])
    fig.update_layout(
        showlegend=False,
        margin=dict(l=0, r=0, b=0, t=0),
        scene=dict(
            xaxis=dict(showbackground=False),
            yaxis=dict(showbackground=False),
            zaxis=dict(showbackground=False),
        )
    )
    fig.write_html(output)


if __name__ == '__main__':
    g = ig.Graph.Read_Pickle('1-00M-000X_DAAF4CC-core.pkl')
    build_3d_graph(g, build_colors(93), '1-00M-000X_3d-core.html')
