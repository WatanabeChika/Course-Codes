"""拥塞分析：根据 path_selection + routes + TM 计算 ISL 利用率。

关键修正：
- path_selection 的值是路径分配比例，不是 Gbps。
- 正确口径应为：edge_load = sum(path_fraction * demand_Gbps)。
"""

from __future__ import annotations

import argparse
import os
import sys

if __package__ in (None, ""):
    sys.path.append(os.path.dirname(os.path.dirname(__file__)))
import csv
import glob
import json
import re
import statistics
import time
from typing import Iterable

from LEOCraft.dataset import InternetTrafficAcrossCities

from myexample.common import write_csv_rows

DEFAULT_SUMMARY_CSV_PATH = "./mytests/congestion_results.csv"
DEFAULT_ISL_CAPACITY_GBPS = 50.0

SCENARIOS = [
    {
        "name": "High_Population_TM",
        "base_dir": "./Starlink_High_Population_TM",
        "tm_dataset": InternetTrafficAcrossCities.ONLY_POP_100,
    },
    {
        "name": "High_GDP_Population_TM",
        "base_dir": "./Starlink_High_GDP_Population_TM",
        "tm_dataset": InternetTrafficAcrossCities.POP_GDP_100,
    },
    {
        "name": "Country_Capitals_TM",
        "base_dir": "./Starlink_Country_Capitals_TM",
        "tm_dataset": InternetTrafficAcrossCities.COUNTRY_CAPITALS_ONLY_POP,
    },
]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="分析不同 TM 场景下的 ISL 拥塞")
    parser.add_argument("--output-csv", default=DEFAULT_SUMMARY_CSV_PATH, help="结果 CSV 路径")
    parser.add_argument("--isl-capacity-gbps", type=float, default=DEFAULT_ISL_CAPACITY_GBPS, help="单条 ISL 容量")
    parser.add_argument("--limit", type=int, default=None, help="只分析前 N 个场景")
    return parser.parse_args()


def load_json(path: str) -> dict:
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


def infer_num_ground_stations(route_keys: Iterable[str]) -> int:
    max_gid = -1
    for pair in route_keys:
        try:
            src, dst = pair.split("_")
            max_gid = max(max_gid, int(src.split("-")[1]), int(dst.split("-")[1]))
        except (ValueError, IndexError):
            continue
    return max_gid + 1


def build_undirected_demand_metrics(tm_json_path: str, num_gs: int) -> dict[str, float]:
    """复现 Throughput._process_traffic_metrics 的需求聚合逻辑。"""
    content = load_json(tm_json_path)
    demand_metrics: dict[str, float] = {}

    for s in range(num_gs):
        for d in range(s + 1, num_gs):
            s_gs_name = f"G-{s}"
            d_gs_name = f"G-{d}"
            outgoing_key = f"{s_gs_name}_{d_gs_name}"
            incoming_key = f"{d_gs_name}_{s_gs_name}"
            demand_metrics[outgoing_key] = content.get(incoming_key, 0.0) + content.get(outgoing_key, 0.0)

    return demand_metrics


def find_case_files(base_dir: str) -> tuple[str, str]:
    """定位一个场景的 path_selection 与 routes 文件。"""
    ps_candidates = sorted(glob.glob(os.path.join(base_dir, "*", "Performance", "path_selection.json")))
    if not ps_candidates:
        raise FileNotFoundError(f"未找到 path_selection.json: {base_dir}")

    path_selection_path = ps_candidates[0]
    time_dir = os.path.dirname(os.path.dirname(path_selection_path))

    route_candidates = sorted(glob.glob(os.path.join(time_dir, "*_routes.json")))
    if not route_candidates:
        raise FileNotFoundError(f"未找到 *_routes.json: {time_dir}")

    return path_selection_path, route_candidates[0]


def load_all_isl_edges(base_dir: str) -> set[tuple[str, str]]:
    """从导出的 *.isls.csv 读取全部 ISL（若存在）。"""
    isl_edges: set[tuple[str, str]] = set()
    isl_files = glob.glob(os.path.join(base_dir, "*.isls.csv"))

    for path in isl_files:
        # 文件名示例：PlusGridShell_0_o72n22...isls.csv
        match = re.search(r"PlusGridShell_(\d+)_", os.path.basename(path))
        if not match:
            continue
        shell_id = int(match.group(1))

        with open(path, "r", encoding="utf-8") as f:
            reader = csv.DictReader(f)
            for row in reader:
                sat_1 = row.get("sat_1")
                sat_2 = row.get("sat_2")
                if sat_1 is None or sat_2 is None:
                    continue
                a = f"S{shell_id}-{int(sat_1)}"
                b = f"S{shell_id}-{int(sat_2)}"
                isl_edges.add(tuple(sorted((a, b))))

    return isl_edges


def summarize_utilization(
    link_loads_gbps: dict[tuple[str, str], float],
    all_isl_edges: set[tuple[str, str]],
    isl_capacity_gbps: float,
) -> dict[str, float | int]:
    if isl_capacity_gbps <= 0:
        raise ValueError("ISL 容量必须大于 0")

    # 若没有导出的全量 ISL 列表，则退化为仅统计 active links
    all_edges = all_isl_edges if all_isl_edges else set(link_loads_gbps.keys())

    utilizations_all = []
    utilizations_active = []

    active_count = 0
    # 全 ISL（含 idle）的分桶
    low_all = med_all = high_all = congested_all = 0
    # 仅 active ISL 的分桶
    low_active = med_active = high_active = congested_active = 0

    for edge in all_edges:
        ratio = link_loads_gbps.get(edge, 0.0) / isl_capacity_gbps
        utilizations_all.append(ratio)
        if ratio > 0:
            active_count += 1
            utilizations_active.append(ratio)

        if ratio < 0.20:
            low_all += 1
            if ratio > 0:
                low_active += 1
        elif ratio < 0.60:
            med_all += 1
            if ratio > 0:
                med_active += 1
        elif ratio < 0.80:
            high_all += 1
            if ratio > 0:
                high_active += 1
        else:
            congested_all += 1
            if ratio > 0:
                congested_active += 1

    total_links = len(all_edges)

    if not utilizations_all:
        return {
            "Total_ISLs_Count": 0,
            "Active_ISLs_Count": 0,
            "Idle_ISLs_Count": 0,
            "Low_All_Count": 0,
            "Medium_All_Count": 0,
            "High_All_Count": 0,
            "Congested_All_Count": 0,
            "Low_All_%": 0.0,
            "Medium_All_%": 0.0,
            "High_All_%": 0.0,
            "Congested_All_%": 0.0,
            "Low_Active_Count": 0,
            "Medium_Active_Count": 0,
            "High_Active_Count": 0,
            "Congested_Active_Count": 0,
            "Low_Active_%": 0.0,
            "Medium_Active_%": 0.0,
            "High_Active_%": 0.0,
            "Congested_Active_%": 0.0,
            "Avg_Utilization_All_%": 0.0,
            "P95_Utilization_All_%": 0.0,
            "Max_Utilization_All_%": 0.0,
            "Avg_Utilization_Active_%": 0.0,
            "P95_Utilization_Active_%": 0.0,
            "Max_Utilization_Active_%": 0.0,
        }

    pct_all = lambda x: round((x / total_links) * 100, 1) if total_links else 0.0
    pct_active = lambda x: round((x / active_count) * 100, 1) if active_count else 0.0

    p95_all = statistics.quantiles(utilizations_all, n=20)[-1] if len(utilizations_all) >= 20 else max(utilizations_all)
    p95_active = (
        statistics.quantiles(utilizations_active, n=20)[-1]
        if len(utilizations_active) >= 20
        else (max(utilizations_active) if utilizations_active else 0.0)
    )

    return {
        "Total_ISLs_Count": total_links,
        "Active_ISLs_Count": active_count,
        "Idle_ISLs_Count": total_links - active_count,
        "Low_All_Count": low_all,
        "Medium_All_Count": med_all,
        "High_All_Count": high_all,
        "Congested_All_Count": congested_all,
        "Low_All_%": pct_all(low_all),
        "Medium_All_%": pct_all(med_all),
        "High_All_%": pct_all(high_all),
        "Congested_All_%": pct_all(congested_all),
        "Low_Active_Count": low_active,
        "Medium_Active_Count": med_active,
        "High_Active_Count": high_active,
        "Congested_Active_Count": congested_active,
        "Low_Active_%": pct_active(low_active),
        "Medium_Active_%": pct_active(med_active),
        "High_Active_%": pct_active(high_active),
        "Congested_Active_%": pct_active(congested_active),
        "Avg_Utilization_All_%": round(statistics.mean(utilizations_all) * 100, 1),
        "P95_Utilization_All_%": round(p95_all * 100, 1),
        "Max_Utilization_All_%": round(max(utilizations_all) * 100, 1),
        "Avg_Utilization_Active_%": round((statistics.mean(utilizations_active) if utilizations_active else 0.0) * 100, 1),
        "P95_Utilization_Active_%": round(p95_active * 100, 1),
        "Max_Utilization_Active_%": round((max(utilizations_active) if utilizations_active else 0.0) * 100, 1),
    }


def analyze_one_scenario(name: str, base_dir: str, tm_dataset: str, isl_capacity_gbps: float) -> dict:
    path_sel_file, routes_file = find_case_files(base_dir)

    path_selection = load_json(path_sel_file)
    routes = load_json(routes_file)

    num_gs = infer_num_ground_stations(routes.keys())
    demand_metrics = build_undirected_demand_metrics(tm_dataset, num_gs)

    link_loads_gbps: dict[tuple[str, str], float] = {}
    carried_throughput_gbps = 0.0

    for flow_pair, allocations in path_selection.items():
        if flow_pair not in routes:
            continue

        demand_gbps = demand_metrics.get(flow_pair, 0.0)
        if demand_gbps <= 0:
            continue

        available_paths = routes[flow_pair]
        carried_throughput_gbps += sum(float(x) for x in allocations.values()) * demand_gbps
        for path_idx_str, path_fraction in allocations.items():
            path_idx = int(path_idx_str)
            if path_idx >= len(available_paths):
                continue

            flow_gbps = float(path_fraction) * demand_gbps
            if flow_gbps <= 0:
                continue

            path = available_paths[path_idx]
            for i in range(len(path) - 1):
                a = str(path[i])
                b = str(path[i + 1])
                # 仅统计 ISL，排除 GSL
                if a.startswith("G-") or b.startswith("G-"):
                    continue
                edge = tuple(sorted((a, b)))
                link_loads_gbps[edge] = link_loads_gbps.get(edge, 0.0) + flow_gbps

    all_isl_edges = load_all_isl_edges(base_dir)
    util_stats = summarize_utilization(link_loads_gbps, all_isl_edges, isl_capacity_gbps)

    return {
        "Traffic_Matrix": name,
        "ISL_Capacity_Gbps": isl_capacity_gbps,
        "TM_Pairs_With_Demand": len(demand_metrics),
        "TM_Total_Demand_Gbps": round(sum(demand_metrics.values()), 2),
        "Carried_Throughput_Gbps": round(carried_throughput_gbps, 2),
        **util_stats,
    }


def main() -> None:
    args = parse_args()
    start_time = time.perf_counter()

    scenarios = SCENARIOS[: args.limit] if args.limit else SCENARIOS
    all_results: list[dict] = []

    for scenario in scenarios:
        print(f"Analyzing congestion for: {scenario['name']}")
        try:
            result = analyze_one_scenario(
                name=scenario["name"],
                base_dir=scenario["base_dir"],
                tm_dataset=scenario["tm_dataset"],
                isl_capacity_gbps=args.isl_capacity_gbps,
            )
            all_results.append(result)
            print(
                f"  done: carried={result['Carried_Throughput_Gbps']} Gbps, "
                f"active_links={result['Active_ISLs_Count']}, "
                f"max_active_util={result['Max_Utilization_Active_%']}%"
            )
        except FileNotFoundError as exc:
            print(f"  skipped: {exc}")

    write_csv_rows(args.output_csv, all_results)
    elapsed_min = round((time.perf_counter() - start_time) / 60, 2)

    print(f"Done. Cases={len(all_results)}")
    print(f"Total time taken: {elapsed_min} minutes")
    print(f"Summary data saved to: {args.output_csv}")


if __name__ == "__main__":
    main()
