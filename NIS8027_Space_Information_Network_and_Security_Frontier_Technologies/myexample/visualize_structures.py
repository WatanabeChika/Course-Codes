"""结构可视化脚本（非数据对比图）。

对应关系：
- 需要结构可视化：architecture / time_evolution
- 不需要结构可视化：altitude / inclination / traffic_matrix / congestion

当前策略：
- architecture: 3D HTML（保留全部卫星点 + 少量壳骨架链路 + 少量路由）
- time_evolution: 2D 叠加 PNG（多时刻卫星地面投影与同一路由叠加）
"""

from __future__ import annotations

import argparse
import os
import sys
from dataclasses import dataclass

if __package__ in (None, ""):
    sys.path.append(os.path.dirname(os.path.dirname(__file__)))

import plotly.graph_objects as go
import plotly.io as pio

from LEOCraft.constellations.LEO_constellation import LEOConstellation
from LEOCraft.dataset import GroundStationAtCities
from LEOCraft.satellite_topology.plus_grid_shell import PlusGridShell
from LEOCraft.user_terminals.ground_station import GroundStation
from LEOCraft.utilities import k_shortest_paths
from LEOCraft.visuals.sat_view_3D import SatView3D

from myexample.common import (
    BASE_ALTITUDE_M,
    BASE_ELEVATION_DEG,
    BASE_INCLINATION_DEG,
    BASE_ORBITS,
    BASE_PHASE_OFFSET,
    BASE_SATS_PER_ORBIT,
)


@dataclass(frozen=True)
class Shell:
    shell_id: int
    orbits: int
    sats_per_orbit: int
    altitude_m: float
    inclination_deg: float


def build_constellation(shells: list[Shell], time_second: int = 0, verbose: bool = False) -> LEOConstellation:
    leo_con = LEOConstellation("LEOCraft-Visual")
    leo_con.v.verbose = verbose
    leo_con.add_ground_stations(GroundStation(GroundStationAtCities.TOP_100))

    for s in shells:
        leo_con.add_shells(
            PlusGridShell(
                id=s.shell_id,
                orbits=s.orbits,
                sat_per_orbit=s.sats_per_orbit,
                altitude_m=s.altitude_m,
                inclination_degree=s.inclination_deg,
                angle_of_elevation_degree=BASE_ELEVATION_DEG,
                phase_offset=BASE_PHASE_OFFSET,
            )
        )

    leo_con.set_time(second=time_second)
    leo_con.set_loss_model(None)
    leo_con.build()
    leo_con.create_network_graph()
    return leo_con


def setup_arch_style(view: SatView3D) -> None:
    """架构图样式：点保留，线减负。"""
    view._DEFAULT_SAT_SIZE = 8
    view._SPECIAL_SAT_SIZE = 10
    view._DEFAULT_GS_SIZE = 6
    view._DEFAULT_WIDTH = 5
    view._THICK_WIDTH = 8
    view._ISL_COLOR = "rgba(80,80,80,0.40)"
    view._R_ISL_COLOR = "rgba(255,20,80,1.0)"
    # 使用高饱和颜色，确保不同壳层可分辨
    view._shell_colors = [
        "rgb(255,0,0)",
        "rgb(0,112,255)",
        "rgb(0,170,0)",
        "rgb(255,140,0)",
        "rgb(170,0,255)",
        "rgb(0,200,200)",
        "rgb(255,0,180)",
        "rgb(210,210,0)",
    ]


def init_routes_store(leo_con: LEOConstellation) -> None:
    leo_con.routes = {}
    leo_con.link_load = {}
    leo_con.no_path_found = set()
    leo_con.k_path_not_found = set()


def is_sat_only_path(path: list[str]) -> bool:
    return len(path) >= 2 and all(not hop.startswith("G-") for hop in path[1:-1])


def add_selected_routes(leo_con: LEOConstellation, gs_pairs: list[tuple[str, str]], k: int = 1) -> list[str]:
    """只生成少量代表路由并写入 leo_con.routes。"""
    if k <= 0:
        raise ValueError("k must be >= 1")

    if not hasattr(leo_con, "routes"):
        init_routes_store(leo_con)

    selected_flows: list[str] = []
    for src, dst in gs_pairs:
        leo_con.connect_ground_station(src, dst)
        compute_status, flow, k_paths = k_shortest_paths(
            leo_con.sat_net_graph,
            src,
            dst,
            k=max(leo_con.k, k),
        )

        if compute_status and k_paths:
            k_paths = [p for p in k_paths if is_sat_only_path(p)]

        leo_con._add_route(compute_status, flow, k_paths)
        if compute_status and k_paths and len(k_paths) >= k:
            selected_flows.append(flow)

    return selected_flows


def shell_skeleton_isls(shell: PlusGridShell, orbit_stride: int = 12, sat_stride: int = 8) -> list[tuple[str, str]]:
    """抽取极少量 ISL 作为壳骨架线。"""
    edges: list[tuple[str, str]] = []
    for sid_a, sid_b in shell.isls:
        oa = shell._get_orbit_num(sid_a)
        ob = shell._get_orbit_num(sid_b)
        na = shell._get_sat_num_in_orbit(sid_a)
        nb = shell._get_sat_num_in_orbit(sid_b)

        # 保留两类“经线/纬线”骨架
        keep_orbit_line = (oa == ob) and (oa % orbit_stride == 0) and (min(na, nb) % sat_stride == 0)
        keep_sat_line = (na == nb) and (na % sat_stride == 0) and (min(oa, ob) % orbit_stride == 0)
        if keep_orbit_line or keep_sat_line:
            edges.append((shell.encode_sat_name(sid_a), shell.encode_sat_name(sid_b)))
    return edges


def export_architecture_views(output_dir: str, verbose: bool) -> None:
    os.makedirs(output_dir, exist_ok=True)

    cases = {
        "multi_shell": [
            Shell(0, 24, 22, 540_000.0, 53.0),
            Shell(1, 24, 22, 550_000.0, 53.2),
            Shell(2, 24, 22, 560_000.0, 70.0),
        ],
        "single_shell": [Shell(0, 72, 22, 550_000.0, 53.0)],
    }

    gs_pairs = [("G-0", "G-36"), ("G-2", "G-52"), ("G-18", "G-75")]

    for case_name, shells in cases.items():
        print(f"[architecture] building {case_name}")
        leo_con = build_constellation(shells=shells, verbose=verbose)

        init_routes_store(leo_con)
        selected_flows = add_selected_routes(leo_con, gs_pairs, k=1)

        # 版本 1：所有卫星点 + 壳骨架
        mesh_view = SatView3D(leo_con, title=f"Architecture Shell Points: {case_name}", lat=20.0, long=20.0)
        setup_arch_style(mesh_view)
        mesh_view.add_all_satellites()

        skeleton_edges: list[tuple[str, str]] = []
        # for shell in leo_con.shells:
        #     skeleton_edges.extend(shell_skeleton_isls(shell, orbit_stride=12, sat_stride=8))
        # if skeleton_edges:
        #     mesh_view.add_ISLs(tuple(skeleton_edges))

        mesh_view.build()
        mesh_path = os.path.join(output_dir, f"architecture_{case_name}_mesh.html")
        mesh_view.export_html(mesh_path)
        print(f"  exported: {mesh_path}")

        # 版本 2：所有卫星点 + 壳骨架 + 少量路由
        route_view = SatView3D(leo_con, title=f"Architecture Shell+Routes: {case_name}", lat=20.0, long=20.0)
        setup_arch_style(route_view)
        route_view.add_all_satellites()
        if skeleton_edges:
            route_view.add_ISLs(tuple(skeleton_edges))
        for flow in selected_flows:
            route_view.add_routes(flow, k=1)
        route_view.add_ground_stations(*(gs for pair in gs_pairs for gs in pair))
        route_view.highlight_satellites(["S0-0"])
        route_view.build()

        route_path = os.path.join(output_dir, f"architecture_{case_name}_mesh_routes.html")
        route_view.export_html(route_path)
        print(f"  exported: {route_path}")


def node_latlon(leo_con: LEOConstellation, node: str) -> tuple[float, float]:
    if node.startswith("S"):
        sat = leo_con.sat_info(node)
        return float(sat.nadir_latitude_deg), float(sat.nadir_longitude_deg)
    gid = leo_con.ground_stations.decode_name(node)
    gs = leo_con.ground_stations.terminals[gid]
    return float(gs.latitude_degree), float(gs.longitude_degree)


def first_valid_path(paths: list[list[str]]) -> list[str] | None:
    for p in paths:
        if is_sat_only_path(p):
            return p
    return None


def export_time_evolution_overlay_png(
    output_dir: str,
    verbose: bool,
    step_hours: int = 6,
) -> None:
    """2D 叠加图：带世界底图的多时刻卫星投影 + 同一路由，输出 PNG。"""
    os.makedirs(output_dir, exist_ok=True)

    frame_seconds = list(range(0, 24 * 3600 + 1, step_hours * 3600))
    src, dst = "G-0", "G-36"

    colors = [
        "#e41a1c",
        "#377eb8",
        "#4daf4a",
        "#984ea3",
        "#ff7f00",
        "#a65628",
        "#f781bf",
        "#999999",
    ]
    fig = go.Figure()

    for idx, t in enumerate(frame_seconds):
        print(f"[time_evolution] collect t={t}s")
        leo_con = build_constellation(
            shells=[Shell(0, BASE_ORBITS, BASE_SATS_PER_ORBIT, BASE_ALTITUDE_M, BASE_INCLINATION_DEG)],
            time_second=t,
            verbose=verbose,
        )

        # 所有卫星点（满足你“保留全部卫星点”的要求）
        lats: list[float] = []
        lons: list[float] = []
        shell = leo_con.shells[0]
        for sid in range(len(shell.satellites)):
            sname = shell.encode_sat_name(sid)
            sat = leo_con.sat_info(sname)
            lats.append(float(sat.nadir_latitude_deg))
            lons.append(float(sat.nadir_longitude_deg))

        color = colors[idx % len(colors)]
        label = f"t={t // 3600}h"
        fig.add_trace(
            go.Scattergeo(
                lon=lons,
                lat=lats,
                mode="markers",
                marker=dict(size=5, color=color, opacity=0.65),
                name=label,
                hoverinfo="skip",
            )
        )

        # 同一路由叠加，观察时变拓扑/路径漂移
        leo_con.connect_ground_station(src, dst)
        ok, flow, k_paths = k_shortest_paths(leo_con.sat_net_graph, src, dst, k=20)
        if ok and k_paths:
            path = first_valid_path(k_paths)
            if path:
                route_lats: list[float] = []
                route_lons: list[float] = []
                for node in path:
                    lat, lon = node_latlon(leo_con, node)
                    route_lats.append(lat)
                    route_lons.append(lon)
                fig.add_trace(
                    go.Scattergeo(
                        lon=route_lons,
                        lat=route_lats,
                        mode="lines",
                        line=dict(color=color, width=2.3),
                        name=f"{label} route",
                        hoverinfo="skip",
                        showlegend=False,
                    )
                )

    # 标注观测端点
    base_con = build_constellation(
        shells=[Shell(0, BASE_ORBITS, BASE_SATS_PER_ORBIT, BASE_ALTITUDE_M, BASE_INCLINATION_DEG)],
        time_second=0,
        verbose=False,
    )
    s_lat, s_lon = node_latlon(base_con, src)
    d_lat, d_lon = node_latlon(base_con, dst)
    fig.add_trace(
        go.Scattergeo(
            lon=[s_lon, d_lon],
            lat=[s_lat, d_lat],
            mode="markers+text",
            text=[src, dst],
            textposition="top center",
            marker=dict(size=12, color="#000000", symbol="star"),
            name="Route endpoints",
        )
    )

    fig.update_layout(
        title="Time Evolution Overlay (2D Map): Satellite Topology and Route Drift",
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="left", x=0),
        margin=dict(l=10, r=10, t=60, b=10),
        geo=dict(
            projection_type="equirectangular",
            showland=True,
            landcolor="rgb(235, 241, 248)",
            showocean=True,
            oceancolor="rgb(210, 229, 246)",
            showcountries=True,
            countrycolor="rgb(140, 140, 140)",
            showcoastlines=True,
            coastlinecolor="rgb(110, 110, 110)",
            lataxis=dict(showgrid=True, gridcolor="rgba(120,120,120,0.2)"),
            lonaxis=dict(showgrid=True, gridcolor="rgba(120,120,120,0.2)"),
        ),
    )

    png_path = os.path.join(output_dir, f"time_evolution_overlay_{step_hours}h.png")
    pio.write_image(fig, png_path, width=2200, height=1200, scale=1)
    print(f"  exported: {png_path}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="LEOCraft 结构可视化（非数据对比图）")
    parser.add_argument(
        "--tasks",
        nargs="+",
        choices=["architecture", "time_evolution"],
        default=["architecture", "time_evolution"],
        help="要生成的结构可视化任务",
    )
    parser.add_argument("--output-dir", default="./mytests/structure_visuals", help="输出目录")
    parser.add_argument("--time-step-hours", type=int, default=6, help="时序叠加步长（小时）")
    parser.add_argument("--verbose", action="store_true", help="打印 LEOCraft 详细日志")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    if args.time_step_hours <= 0:
        raise ValueError("--time-step-hours 必须大于 0")

    for task in args.tasks:
        task_dir = os.path.join(args.output_dir, task)
        if task == "architecture":
            export_architecture_views(task_dir, args.verbose)
        elif task == "time_evolution":
            export_time_evolution_overlay_png(task_dir, args.verbose, args.time_step_hours)

    print("Done. Structure visualizations generated.")


if __name__ == "__main__":
    main()
